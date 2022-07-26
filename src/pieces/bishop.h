#pragma once

#include "piece.h"

class Bishop : public Piece {
public:
    //methods
    Bishop(bool, int);
    std::vector<int> pseudo_legal_moves(Board*);
};
