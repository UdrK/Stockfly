#include "queen.h"

Queen::Queen(bool side, int position) {
    Queen::side = side;
    Queen::position = position;
    if (side == true) {
        Queen::appearance = 'Q';
    }
    else {
        Queen::appearance = 'q';
    }
}

std::vector<int> Queen::pseudo_legal_moves(Board* board) {
    std::vector<int> moves = Piece::piece_movement(board, false, true, true);
    return moves;
}