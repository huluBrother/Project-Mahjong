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

#include <random>
#include "RandomPlayer.h"

using mahjong::Action;
using mahjong::Player;
using mahjong::Tile;
using mahjong::TileGroup;
using mahjong::RandomPlayer;

Action RandomPlayer::onTurn(int playerID, Tile tile) {
    if (playerID == getID()) {
        if (getHand().testWin()) {
            return Action(Win, Tile());
        }

        std::random_device rd;
        std::mt19937 urd(rd());
        std::uniform_int_distribution<int> tileDistribution(0, static_cast<int>(getHand().getData().size()));
        int index = tileDistribution(urd);
        return Action(Discard, index == getHand().getData().size() ? tile :
                               getHand().getTile(index));
    } else {
        return Action();
    }
}

Action RandomPlayer::onOtherPlayerMakeAction(int playerID, std::string playerName, Action action) {
#ifdef SIMPLE_MAHJONG
    Hand copyHand(getHand().getData());
    copyHand.pickTile(action.getTile());
    if (copyHand.testWin()) {
        return Action(Win, action.getTile());
    } else {
        return Action();
    }
#else
    throw std::runtime_error("No games are defined for this player.");
#endif
}
