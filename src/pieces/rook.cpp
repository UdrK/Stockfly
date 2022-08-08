#include "rook.h"

Rook::Rook(bool side, int position) {
    Rook::side = side;
    Rook::position = position;
    if (side == true) {
        Rook::appearance = 'R';
    }
    else {
        Rook::appearance = 'r';
    }
    Rook::agnostic_appearance = 'R';
}

std::vector<int> Rook::pseudo_legal_moves(Board* board) {
    std::vector<int> moves = Piece::piece_movement(board, Piece::position, Piece::side, false, false, true);
    return moves;
}