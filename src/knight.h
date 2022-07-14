#pragma once
#include "piece.h"

class Knight : public Piece {
public:
    //methods
    Knight(bool, int);
    std::vector<int> pseudo_legal_moves(Board*);
};
