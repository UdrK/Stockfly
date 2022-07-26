#include "king.h"

King::King(bool side, int position) {
    King::side = side;
    King::position = position;
    if (side == true) {
        King::appearance = 'K';
    }
    else {
        King::appearance = 'k';
    }
}

std::vector<int> King::pseudo_legal_moves(Board* board) {
    std::vector<int> moves = Piece::piece_movement(board, Piece::position, Piece::side, true, true, true);
    return moves;
}
