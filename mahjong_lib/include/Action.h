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

#ifndef MAHJONG_LIB_ACTION_H
#define MAHJONG_LIB_ACTION_H

#include "Tile.h"

namespace mahjong {
enum ActionState {
    Pass,
    Win,
    Chi,
    Pong,
    Kang,
    ConcealedKang,
    Richii
};

class Action {
 public:
    Action() {
        mActionState = Pass;
    }
    Action(ActionState actionState, Tile tile) : mActionState(actionState), mTile(tile) {}

    ActionState getActionState() { return mActionState; }
    Tile getTile() { return mTile; }

 private:
    ActionState mActionState;
    Tile mTile;
};
} // namespace mahjong.

#endif // MAHJONG_LIB_ACTION_H