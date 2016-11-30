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

#ifndef MAHJONG_LIB_CONSTANTS_H
#define MAHJONG_LIB_CONSTANTS_H

#include <iostream>

// Uncomment/Comment the following line to Enable/Disable debug output.
// #define DEBUG_BUILD

namespace mahjong {
enum Wind {
    East = 0,
    South = 1,
    West = 2,
    North = 3
};

const Wind Winds[] = {East, South, West, North};

/**
 * Classes.
 */
class Action;
class Board;
class Game;
class Hand;
class Player;
class Tile;
class TileGroup;
class TileStack;

/**
 * Logger.
 */
#ifdef DEBUG_BUILD
#define LOGI(TAG, MESSAGE) std::cout << TAG << ":\t" << MESSAGE << '\n';
#define LOGE(TAG, MESSAGE) std::cout << "\033[1;31m" << TAG << ":\t" << MESSAGE << "\033[0m\n";
#else
#define LOGI(TAG, MESSAGE)
#define LOGE(TAG, MESSAGE)
#endif
} // namespace mahjong.

#endif // MAHJONG_LIB_CONSTANTS_H
