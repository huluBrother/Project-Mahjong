//
//  Copyright © 2016 Project Mahjong. All rights reserved.
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//

#include "SimpleGame.h"

#include <iostream>

using std::cout;

using mahjong::Board;
using mahjong::SimpleGame;

SimpleGame::SimpleGame(Player *p1, Player *p2, Player *p3, Player *p4, int roundCount) :
        Game(p1, p2, p3, p4, roundCount) {
    mBoard = new Board(this, p1, p2, p3, p4, false, 0);
}

void SimpleGame::startGame() {
    mCurrentRound = 1;
    mRoundFinished = false;
    mBoard->setup(mahjong::JAPANESE_MAHJONG_TILE_SET, mahjong::East);
}

// Callback implementations.
void SimpleGame::onRoundStart() {
    system("clear");
    cout << "Round "<< mCurrentRound <<" start.\n";
    while (!mRoundFinished) {
        mBoard->proceedToNextPlayer();
    }
}

void SimpleGame::onBeforePlayerPickTile(Player *player, Tile tile) {

}

void SimpleGame::onAfterPlayerPickTile(Player *player, Tile tile) {

}

void SimpleGame::onPlayerDiscardTile(Player *player, Tile tile) {

}

void SimpleGame::onPlayerPass(Player *player) {

}

void SimpleGame::onRoundFinished(bool drained, Player *winner) {
    if (drained) {
        cout << "Tile Stack Drained. Tie!\n";
    } else {
        cout << winner->getPlayerName() << " wins!\n";
    }
    mRoundFinished = true;
}

// Rule implementation.
int SimpleGame::calculateScore(Hand mHand) {
    return mHand.testWin();
}

void SimpleGame::printPlayer(Player *p) {
    cout << MAHJONG_SPECIAL[p->getSeatPosition()] << ": "
         << p->getPlayerName() << " ID" << p->getID() << '\n'
         << p->getHand().getPrintable();
}
void SimpleGame::printBoard() {
    auto players = mBoard->getPlayers();
    for (auto it = players->begin(); it < players->end(); it++) {
        printPlayer(*it);
        cout << '\n';
    }
}