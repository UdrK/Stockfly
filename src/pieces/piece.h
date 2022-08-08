#pragma once
#include <string>
#include <vector>

class Board;    // forward declaration of Board to avoid circular dependencies

class Piece {
public:
    // state
    int position;
    bool side;
    std::string appearance;
    std::string agnostic_appearance;

    // methods
    // generic movement methods
    std::vector<int> static piece_movement(Board*, int, bool, bool, bool, bool);
    std::vector<int> static knight_movement(Board*, int, bool);
    std::vector<int> static pawn_movement(Board*, int, bool);

    std::vector<int> is_attacked(Board*);
    virtual std::vector<int> pseudo_legal_moves(Board*) = 0;  // method that returns a list of indeces of squares on which the piece can move

    // Chess I/O
    std::string get_appearance(bool);
};
