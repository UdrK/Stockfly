#pragma once

#include "piece.h"

class Queen : public Piece {
public:
    //methods
    Queen(bool, int);
    std::vector<int> pseudo_legal_moves(Board*);
};
