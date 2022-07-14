#pragma once

#include <stdlib.h>
#include <string>

class Piece;
class King;

class Board {
public:
    Board(std::string);

    // setters
    void set_initial_position();
    void set_from_fen(std::string);
    void set_en_passant_file(int);
    void set_king(bool, King*);
    void move_piece_to(int, Piece*);

    // getters
    Piece** get_board();
    Piece* piece_at(int);
    King* get_king(bool);
    int get_en_passant_file();

    // Chess I/O
    std::string get_fen();
    char* print_board(bool unicode = true);

    bool is_move_legal(Piece*, int);
    void move(std::string);

private:
    King* white_king;
    King* black_king;
    int board_size = 64;
    int en_passant_file = -1;
    Piece** board;
    void fen_to_board(std::string);
    std::string board_to_fen();
};
