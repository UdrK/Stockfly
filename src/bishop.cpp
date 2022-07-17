#include "bishop.h"

Bishop::Bishop(bool side, int position) {
    Bishop::side = side;
    Bishop::position = position;
    if (side == true) {
        Bishop::appearance = 'B';
    }
    else {
        Bishop::appearance = 'b';
    }
}

std::vector<int> Bishop::pseudo_legal_moves(Board* board) {
    std::vector<int> moves = Piece::piece_movement(board, Piece::position, Piece::side, false, true, false);
    return moves;
}