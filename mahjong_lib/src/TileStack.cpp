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

#include <algorithm>

#include "TileStack.h"

using std::vector;

using mahjong::TileStack;
using mahjong::Tile;

TileStack::TileStack(TileSetType tileSetType, bool enableDora, int notPlayingCount) {
    setup(tileSetType, enableDora, notPlayingCount);
}

void TileStack::setup(TileSetType tileSetType, bool enableDora, int notPlayingCount) {
    mTileSetType = tileSetType;
    mEnableDora = enableDora;
    mNonPlayingTileCount = notPlayingCount;

    switch (tileSetType) {
        case JAPANESE_MAHJONG_TILE_SET:
            for (int i = 1; i <= 9; ++i) {
                for (int j = 0; j < 4; ++j) {
                    mRemainTiles.push_back(Tile(mahjong::Handed, mahjong::Character, i));
                    mRemainTiles.push_back(Tile(mahjong::Handed, mahjong::Dot, i));
                    mRemainTiles.push_back(Tile(mahjong::Handed, mahjong::Bamboo, i));
                    if (i <= 7) {
                        mRemainTiles.push_back(Tile(mahjong::Handed, mahjong::Special, i));
                    }
                }
            }
            break;
        case COMPETITIVE_MAHJONG_TILE_SET:
            // Not implemented.
            break;
        default:
            throw std::invalid_argument("Tile Set Type not recognised.");
    }

    std::random_shuffle(mRemainTiles.begin(), mRemainTiles.end());
}

void TileStack::reset() {
    setup(mTileSetType, mEnableDora, mNonPlayingTileCount);
}

int TileStack::throwDice() {
    std::uniform_int_distribution<int> diceDistribution(1, 6);
    return diceDistribution(mRandomDevice);
}

Tile TileStack::drawTile() {
    Tile retTile = mRemainTiles.back();
    mRemainTiles.pop_back();
    return retTile;
}

bool TileStack::isEmpty() {
    return mRemainTiles.size() == mNonPlayingTileCount;
}