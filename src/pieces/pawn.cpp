#include "pawn.h"

Pawn::Pawn(bool side, int position) {
    Pawn::side = side;
    Pawn::position = position;
    if (side == true) {
        Pawn::appearance = "P";
    }
    else {
        Pawn::appearance = 'p';
    }
    Pawn::agnostic_appearance = "P";
}

std::vector<int> Pawn::pseudo_legal_moves(Board* board) {
    std::vector<int> moves = Piece::pawn_movement(board, Piece::position, Piece::side);
    return moves;
}