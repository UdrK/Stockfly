#pragma once

#include "piece.h"

class King : public Piece {
public:
    //methods
    King(bool, int);
    std::vector<int> pseudo_legal_moves(Board*);
};

