#!/usr/bin/env python3

#  Copyright 2017 Project Mahjong. All rights reserved.
#
#  Licensed under the Apache License, Version 2.0 (the "License");
#  you may not use this file except in compliance with the License.
#  You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
#  Unless required by applicable law or agreed to in writing, software
#  distributed under the License is distributed on an "AS IS" BASIS,
#  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#  See the License for the specific language governing permissions and
#  limitations under the License.

from keras.models import Sequential
from keras.layers import Conv2D, Dense, Activation, Flatten, Dropout, MaxPooling2D
from keras.optimizers import Adam
from keras import backend

NETWORK_TYPE_REGRESSION = 100
NETWORK_TYPE_CLASSIFICATION = 200

TM_DEFAULT_INPUT_SHAPE = (5, 18, 1)
TM_DEFAULT_OUTPUT_NUMBER = 5
TM_DEFAULT_LAYER_CONFIGURATIONS = (32, 64, 256)
TM_DEFAULT_OPTIMIZER = Adam
TM_DEFAULT_LEARNING_RATE = 0.00025

SM_DEFAULT_INPUT_SHAPE = (14, 16, 1)
SM_DEFAULT_OUTPUT_NUMBER = 14
SM_DEFAULT_LAYER_CONFIGURATIONS = (64, 64, 512)
SM_DEFAULT_OPTIMIZER = Adam
SM_DEFAULT_LEARNING_RATE = 0.00025


def tiny_mahjong_dqn_model():
    return _two_layer_cnn_model(input_shape=TM_DEFAULT_INPUT_SHAPE,
                                output_number=TM_DEFAULT_OUTPUT_NUMBER,
                                layer_configurations=TM_DEFAULT_LAYER_CONFIGURATIONS,
                                optimizer=TM_DEFAULT_OPTIMIZER,
                                lr=TM_DEFAULT_LEARNING_RATE,
                                network_type=NETWORK_TYPE_REGRESSION)


def simple_mahjong_dqn_model():
    return _two_layer_cnn_model(input_shape=SM_DEFAULT_INPUT_SHAPE,
                                output_number=SM_DEFAULT_OUTPUT_NUMBER,
                                layer_configurations=SM_DEFAULT_LAYER_CONFIGURATIONS,
                                optimizer=SM_DEFAULT_OPTIMIZER,
                                lr=SM_DEFAULT_LEARNING_RATE,
                                network_type=NETWORK_TYPE_REGRESSION)


def _two_layer_cnn_model(input_shape, output_number, layer_configurations, optimizer, lr,
                         network_type, dim_ordering="tf"):
    backend.set_image_dim_ordering(dim_ordering)

    model = Sequential()
    model.add(Conv2D(layer_configurations[0], kernel_size=(3, 3),
                     padding='same', input_shape=input_shape))
    model.add(Activation('relu'))
    model.add(Conv2D(layer_configurations[1], kernel_size=(3, 3)))
    model.add(Activation('relu'))
    model.add(MaxPooling2D(pool_size=(2, 2)))
    model.add(Dropout(0.25))

    model.add(Flatten())
    model.add(Dense(layer_configurations[2]))
    model.add(Activation('relu'))
    model.add(Dropout(0.5))

    if network_type == NETWORK_TYPE_REGRESSION:
        model.add(Dense(output_number, activation='linear'))

        model.compile(loss='mean_squared_error',
                      optimizer=optimizer(lr=lr),
                      metrics=['accuracy'])
    elif network_type == NETWORK_TYPE_CLASSIFICATION:
        model.add(Dense(output_number, activation='softmax'))

        model.compile(loss='categorical_crossentropy',
                      optimizer=optimizer(lr=lr),
                      metrics=['accuracy'])
    else:
        raise ValueError("Unrecognised network network_type.")

    return model
