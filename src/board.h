#pragma once

#include <stdlib.h>
#include <string>
#include <vector>

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
    void set_castle_rights(bool, bool, bool, bool);

    // getters
    Piece** get_board();
    Piece* piece_at(int);
    King* get_king(bool);
    int get_en_passant_file();
    bool* get_castle_rights();

    // Chess I/O
    std::string get_fen();
    char* print_board(bool unicode = true);

    bool is_square_attacked(int, bool);
    bool is_move_legal(Piece*, int);
    bool is_mate(bool);
    bool is_draw_by_insufficient_material();
    bool is_stalemate(bool);
    bool is_threefold_repetition();
    void move(std::string, bool);
    void castle(std::string, bool);

private:
    King* white_king;
    King* black_king;
    std::vector<Piece*> white_pieces;
    std::vector<Piece*> black_pieces;
    std::vector<std::string> positions_fens;
    bool white_king_side_castle, white_queen_side_castle;
    bool black_king_side_castle, black_queen_side_castle;
    int board_size = 64;
    int en_passant_file = -1;
    Piece** board;
    void fen_to_board(std::string);
    std::string board_to_fen();
};