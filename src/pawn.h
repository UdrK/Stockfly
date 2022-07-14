#pragma once

#include "piece.h"

class Pawn : public Piece {
public:
    //methods
    Pawn(bool, int);
    std::vector<int> pseudo_legal_moves(Board*);
};