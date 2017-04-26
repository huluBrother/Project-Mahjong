#!/usr/bin/env python
# -*- coding: utf-8 -*-

import random
from collections import deque
from datetime import datetime

import tensorflow as tf
from helper import *
from libgames import *
from libmahjong import *
from libplayers import *
from model_generator import simple_mahjong_dqn_model

from mahjong_hand_converter import *

TRAIN = 100
PLAY = 200
EVAL = 300
DEBUG = 400
EPSILON_INITIAL = 1.0
EPSILON_FINAL = 0.01
EPSILON_DECAY_STEP = 10000
REPLAY_MEMORY_SIZE = 100000
BATCH_SIZE = 32
TARGET_UPDATE_INTERVAL = 100
GAMMA = 0.99
LEARNING_RATE = 0.00025
DQCNN_WEIGHTS_FILE = "sm_dqcnn_weights.h5"


class DQCNNPlayer(Player):
    def __init__(self, name, mode):
        super(DQCNNPlayer, self).__init__(name)
        self._mode = mode
        self._model = load_keras_model(SM_DQN_MODEL_NAME, simple_mahjong_dqn_model)

        if os.path.isfile(DQCNN_WEIGHTS_FILE):
            self._model.load_weights(DQCNN_WEIGHTS_FILE)

        self._target_model = simple_mahjong_dqn_model()
        self._target_model.set_weights(self._model.get_weights())

        self._current_episode = 0
        self._epsilon = EPSILON_INITIAL
        self._last_hand = None
        self._last_discard = None
        self._reward = None
        self._this_hand = None
        self._done = None
        self.is_discard = False
        self._replay_memory = deque()
        self._step = 0
        self._total_step = 0

        self._writer = tf.summary.FileWriter("./logs/" + str(datetime.now()))

        self._max_q_history = []
        self._win_round = 0
        self._lost_round = 0
        self._drain_round = 0
        self._total_reward = 0

    @staticmethod
    def get_q_values(model, hand):
        return model.predict(transform_one_hot_to_cnn_matrix(transform_hand_to_one_hot(hand)))[0]

    def make_epsilon_greedy_choice(self, hand):
        if random.uniform(0, 1) < self._epsilon and self._mode == TRAIN:
            return random.randint(0, 13)
        else:
            q_values = self.get_q_values(self._model, hand)
            self._max_q_history.append(np.max(q_values))
            choice = np.random.choice(
                np.array([i for i, j in enumerate(q_values) if j == max(q_values)]))
            return choice

    def append_memory_and_train(self, observation, action, reward, observation_next, done):
        observation = transform_one_hot_to_cnn_matrix(transform_hand_to_one_hot(observation))[0]
        observation_next = transform_one_hot_to_cnn_matrix(transform_hand_to_one_hot(observation_next))[0]
        self._replay_memory.append((observation, action, reward, observation_next, done))
        if len(self._replay_memory) > REPLAY_MEMORY_SIZE:
            self._replay_memory.popleft()

        # Mini batch train.
        if len(self._replay_memory) > BATCH_SIZE and self._total_step % 4 == 0:
            mini_batch = random.sample(list(self._replay_memory), BATCH_SIZE)
            observation_batch = np.array([m[0] for m in mini_batch])
            action_batch = [m[1] for m in mini_batch]
            reward_batch = [m[2] for m in mini_batch]
            observation_next_batch = np.array([m[3] for m in mini_batch])

            q_values = self._model.predict(observation_batch)
            next_q_values = self._model.predict(observation_next_batch)
            next_q_values_target = self._target_model.predict(observation_next_batch)
            for i in range(BATCH_SIZE):
                if mini_batch[i][4]:  # done.
                    q_values[i][action_batch[i]] = reward_batch[i]
                else:
                    q_values[i][action_batch[i]] = \
                            reward_batch[i] + \
                            GAMMA * np.max(next_q_values_target[i])
            self._model.train_on_batch(observation_batch, q_values)

        # Periodically update target network.
        if self._total_step % TARGET_UPDATE_INTERVAL == 0:
            self._target_model.set_weights(self._model.get_weights())

    def onTurn(self, this, playerID, tile):
        if playerID == this.getID():
            self._this_hand = list(this.getHand().getData())
            # Process result of last discard.
            if self._last_hand is not None and self._mode == TRAIN:
                self._reward = 0
                self._done = False
                self.append_memory_and_train(self._last_hand,
                                             self._last_discard,
                                             self._reward,
                                             self._this_hand,
                                             self._done)
            self._step += 1
            # Test if hand can win.
            if this.getHand().testWin():
                # Player win.
                self._reward = 1.0
                self._done = True
                self.game_ends(True, False)
                return Action(ActionState.Win, Tile())
            else:
                it = self.make_epsilon_greedy_choice(
                    this.getHand().getData())
                if self._epsilon > EPSILON_FINAL:
                    self._epsilon -= (EPSILON_INITIAL - EPSILON_FINAL) / EPSILON_DECAY_STEP
                self.is_discard = True
                self._total_step += 1
                self._last_hand = self._this_hand
                self._last_discard = it
                return Action(ActionState.Discard, self._this_hand[it])
        else:
            return Action()

    def onOtherPlayerMakeAction(self, this, playerID, playerName, action):
        # Tile stack drained.
        if playerID == -1 and playerName == "":
            self._reward = 0.1
            self._done = True
            self.game_ends(False, False, True)
            return Action()

        if action.getActionState() == ActionState.Win:
            if self.is_discard:
                # Player lost.
                self._reward = -1.0
                self._done = True
                self.game_ends(False, True)
            else:
                # Player safe.
                self._reward = 0.1
                self._done = True
                self.game_ends(False, False)
        elif action.getActionState() == ActionState.Discard:
            if this.getHand().testWin(action.getTile()):
                # Player win.
                self._reward = 1.0
                self._done = True
                self.game_ends(True, False)
                return Action(ActionState.Win, action.getTile())
        else:
            self.is_discard = False

        return Action()

    def game_ends(self, win, lose, drain=False):
        self._current_episode += 1
        if win:
            self._win_round += 1
        if lose:
            self._lost_round += 1
        if drain:
            self._drain_round += 1
        if self._last_hand is not None and self._mode == TRAIN:
            assert self._last_discard is not None
            assert self._reward is not None
            assert self._this_hand is not None
            assert self._done is not None
            self._total_reward += self._reward
            self.append_memory_and_train(self._last_hand,
                                         self._last_discard,
                                         self._reward,
                                         self._this_hand,
                                         self._done)

        # Summary.
        if self._mode == TRAIN:
            if len(self._max_q_history) > 0:
                average_max_q = sum(self._max_q_history) / len(self._max_q_history)
            else:
                average_max_q = 0
            summary = tf.Summary()
            summary.value.add(tag="Average Max Q", simple_value=average_max_q)
            summary.value.add(tag="Steps per episode", simple_value=self._step)
            summary.value.add(tag="Win rate",
                              simple_value=self._win_round * 1.0 / self._current_episode)
            summary.value.add(tag="Lose rate",
                              simple_value=self._lost_round * 1.0 / self._current_episode)
            summary.value.add(tag="Drain rate",
                              simple_value=self._drain_round * 1.0 / self._current_episode)
            self._writer.add_summary(summary, self._current_episode)
            self._writer.flush()
            print("Epsilon:", self._epsilon, "Average max Q:", average_max_q)

        # Reset.
        self._max_q_history = []
        self.is_discard = False
        self._last_hand = None
        self._last_discard = None
        self._reward = None
        self._this_hand = None
        self._done = None
        self.is_discard = False
        self._step = 0
        if self._mode == TRAIN:
            if self._current_episode % 100 == 0:
                print("Finished", self._current_episode, "episodes.")
                self._model.save_weights(DQCNN_WEIGHTS_FILE)
