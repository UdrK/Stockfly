#pragma once

#include "piece.h"

class Rook : public Piece {
public:
    //methods
    Rook(bool, int);
    std::vector<int> pseudo_legal_moves(Board*);
};
