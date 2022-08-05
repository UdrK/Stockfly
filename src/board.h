#pragma once

#include <stdlib.h>
#include <string>
#include <vector>

class Piece;
class King;
class Ply;

class Board {
public:
    // constructor from fen
    Board(std::string);

    // setters
    void set_initial_position();
    void set_from_fen(std::string, bool overwrite = false);
    void set_en_passant_file(int);
    void set_king(bool, King*);
    void set_castle_rights(bool, bool, bool, bool);
    void set_player(bool);

    // getters
    Piece** get_board();
    Piece* piece_at(int);
    bool get_player();
    King* get_king(bool);
    std::vector<Piece*> get_pieces(bool);
    int get_en_passant_file();
    bool* get_castle_rights();

    // Chess I/O
    std::string get_fen(bool verbose = false);
    char* print_board(bool unicode = true);

    // Chess logic
    bool is_promotion(std::string, int);
    std::vector<int> is_square_attacked_by(int, bool);
    bool is_move_legal(Piece*, int, bool);
    bool is_mate(bool);
    bool is_draw_by_insufficient_material();
    bool is_stalemate(bool);
    bool is_threefold_repetition();
    bool can_castle(char, bool);
    void update_castling_rights(std::string, std::string, int, int);
    void move_piece_to_empty_square(int, Piece*);
    Piece* move_piece_to(int, Piece*);
    void un_castle(std::string, bool);
    void castle(std::string);
    Piece* promotion(std::string, bool);
    void force_move(Ply*);
    void move(Ply*);
    void undo_move(Ply*);

private:
    // board state
    int board_size = 64;
    Piece** board;
    std::vector<Piece*> white_pieces;
    std::vector<Piece*> black_pieces;

    // king handles to simplify code and better performance
    King* white_king;
    King* black_king;

    // castle rights
    bool white_king_side_castle, white_queen_side_castle;
    bool black_king_side_castle, black_queen_side_castle;
    
    // en passant flag (indicates file of pawn that can be taken en passant)
    // 8 is out of bounds value ( no en passant possible )
    int en_passant_file = 8;
    
    // true: white's turn; false: black's turn
    bool side_turn;     

    // history of positions played on the board (used to check for 3fold repetition draw)
    std::vector<std::string> positions_fens;

    // helper methods
    void fen_to_board(std::string, bool overwrite = false);
    std::string board_to_fen(bool verbose = false);
};
