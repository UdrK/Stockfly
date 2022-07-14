#pragma once
#include <string>
#include <vector>

class Board;    // forward declaration of Board to avoid circular dependencies

class Piece {
protected:
    // generic movement methods
    std::vector<int> piece_movement(Board*, bool, bool, bool);
    std::vector<int> knight_movement(Board*);
    std::vector<int> pawn_movement(Board*);

public:
    // state
    int position;
    bool side;
    std::string appearance;

    // methods
    bool is_attacked(Board*);
    virtual std::vector<int> pseudo_legal_moves(Board*) = 0;  // method that returns a list of indeces of squares on which the piece can move

    // Chess I/O
    std::string get_appearance();
};
