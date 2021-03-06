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

#ifndef MAHJONG_LIB_HAND_H
#define MAHJONG_LIB_HAND_H

#include <vector>

#include "Tile.h"
#include "TileGroup.h"

namespace mahjong {
class Hand : public TileGroup {
 public:
    /**
     * Constructor for an empty hand.
     */
    Hand() {}

    /**
     * Constructor for initialise with a vector of Tiles.
     */
    Hand(std::vector<Tile> hand) : TileGroup(hand) {}

    Hand(const std::string tileString) : TileGroup(tileString) {}

    //
    // Game Actions.
    //

    /**
     * Pick one tile from stack.
     *
     * @param t The picked tile.
     */
    void pickTile(Tile t);

    /**
     * Discard one tile from hand.
     *
     * Recommended to use index rather than tile.
     *
     * @param index Index of tile to discard.
     */
    void discardTile(int index);

    /**
     * Discard one tile from hand.
     *
     * @param tile Tile to discard.
     */
    void discardTile(Tile tile);

    // All the following should be performed after sorting (which is default).
    /**
     * Perform a Chi.
     *
     * @param tile Tile to be eaten.
     * @param combination The rest 2 tiles to form a combination.
     */
    void chi(Tile tile, std::vector<int> combination);

    /**
     * Perform a Pong.
     *
     * @param tile Tile to be ponged.
     */
    void pong(Tile tile);

    /**
     * Perform a Kang.
     *
     * @param tile Tile to be kanged.
     */
    void kang(Tile tile);

    /**
     * Perform a Concealed Kang.
     *
     * @param tile Tile to be concealed kanged.
     */
    void concealedKang(Tile tile);

    // Data Processing.
    /**
     * Sort this hand.
     */
    void sort();

    /**
     * Get whether this hand can win or not.
     *
     * Be careful when use this function as it is slow.
     *
     * @return Win or not.
     */
    bool testWin();

    bool testWin(Tile pickedTile);

    void clear();

    // Set states.
    void setTsumo() { mTsumo = true; }

    std::vector<Tile> canChi(Tile tile) const;

    bool canPong(Tile tile) const;

    bool canKang(Tile tile) const;

    bool canRichii(Tile tile) const;

 private:
    bool mTsumo = false;

    bool testWin(std::vector<Tile> hand);
};
} // namespace mahjong.

#endif // MAHJONG_LIB_HAND_H
