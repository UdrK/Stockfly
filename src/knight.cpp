#include "knight.h"

Knight::Knight(bool side, int position) {
    Knight::side = side;
    Knight::position = position;
    if (side == true) {
        Knight::appearance = 'N';
    }
    else {
        Knight::appearance = 'n';
    }
}

std::vector<int> Knight::pseudo_legal_moves(Board* board) {
    std::vector<int> moves = Piece::knight_movement(board);
    return moves;
}