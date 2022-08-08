#include <stdexcept>
#include "board.h"
#include "ply.h"
#include "pieces/piece.h"
#include "pieces/king.h"
#include "pieces/queen.h"
#include "pieces/rook.h"
#include "pieces/bishop.h"
#include "pieces/knight.h"
#include "pieces/pawn.h"
#include "utils.h"

#include <iostream>

Board::Board(std::string fen) {

    // empty board and pieces lists, these will be set_from_fen
    Board::board = new Piece * [Board::board_size];

    Board::white_pieces = std::vector<Piece*>();
    Board::black_pieces = std::vector<Piece*>();

    Board::white_king_side_castle = false;
    Board::white_queen_side_castle = false;
    Board::black_king_side_castle = false;
    Board::black_queen_side_castle = false;

    Board::en_passant_file = 8;

    // list of position reached (updated at every move) in fen format
    Board::positions_fens = std::vector<std::string>();

    if (!fen.empty())
        set_from_fen(fen);
    else
        set_initial_position();
    
    positions_fens.push_back(Board::get_fen(true));
}

// private utility methods  --------
void Board::fen_to_board(std::string fen, bool overwrite) {

    if (overwrite) {
        if (Board::white_pieces.size() > 0) {
            for (Piece* p : white_pieces)
                delete p;
            Board::white_pieces.clear();
            std::vector<Piece*>().swap(Board::white_pieces);

            for (Piece* p : black_pieces)
                delete p;
            Board::black_pieces.clear();
            std::vector<Piece*>().swap(Board::black_pieces);

            Board::white_king_side_castle = false;
            Board::white_queen_side_castle = false;
            Board::black_king_side_castle = false;
            Board::black_queen_side_castle = false;
            Board::en_passant_file = 8;
        }
    }

    int spaces = 0;
    int index = 0;
    for (auto c : fen) {
        if (c == ' ') 
            spaces++;
        if (c != '/') {
            // if number [1-8] and no spaces counted, numbers are empty squares on the board
            if (spaces == 0 && int(c) > 48 && int(c) < 57) {
                int empty_squares = int(c) - 48;
                for (int i = 0; i < empty_squares; i++) {
                    Board::board[index + i] = NULL;
                }
                index += empty_squares;
            }
            else {
                if (spaces == 0) {
                    switch (c) {
                        case 'K': case 'k':
                        {
                            if (c == 'K') {
                                Board::white_king = new King(c == 'K', index);
                                Board::board[index] = Board::white_king;
                                Board::white_pieces.push_back(Board::white_king);
                            }
                            else {
                                Board::black_king = new King(c == 'K', index);
                                Board::board[index] = Board::black_king;
                                Board::black_pieces.push_back(Board::black_king);
                            }
                            break;
                        }
                        case 'Q': case 'q':
                        {
                            Piece* queen = new Queen(c == 'Q', index);
                            Board::board[index] = queen;
                            if (c == 'Q') {
                                Board::white_pieces.push_back(queen);
                            }
                            else {
                                Board::black_pieces.push_back(queen);
                            }
                            break;
                        }
                        case 'R': case 'r':
                        {
                            Piece* rook = new Rook(c == 'R', index);
                            Board::board[index] = rook;
                            if (c == 'R') {
                                Board::white_pieces.push_back(rook);
                            }
                            else {
                                Board::black_pieces.push_back(rook);
                            }
                            break;
                        }
                        case 'N': case 'n':
                        {
                            Piece* knight = new Knight(c == 'N', index);
                            Board::board[index] = knight;
                            if (c == 'N') {
                                Board::white_pieces.push_back(knight);
                            }
                            else {
                                Board::black_pieces.push_back(knight);
                            }
                            break;
                        }
                        case 'B': case 'b':
                        {
                            Piece* bishop = new Bishop(c == 'B', index);
                            Board::board[index] = bishop;
                            if (c == 'B') {
                                Board::white_pieces.push_back(bishop);
                            }
                            else {
                                Board::black_pieces.push_back(bishop);
                            }
                            break;
                        }
                        case 'P': case 'p':
                        {
                            Piece* pawn = new Pawn(c == 'P', index);
                            Board::board[index] = pawn;
                            if (c == 'P') {
                                Board::white_pieces.push_back(pawn);
                            }
                            else {
                                Board::black_pieces.push_back(pawn);
                            }
                            break;
                        }
                    }
                    index += 1;
                }
                if (spaces == 1) {
                    if (c == 'w') {
                        Board::side_turn = true;
                    }
                    else {
                        Board::side_turn = false;
                    }
                }
                if (spaces == 2) {
                    switch (c) {
                        case 'K':
                            Board::white_king_side_castle = true;
                            break;
                        case 'Q':
                            Board::white_queen_side_castle = true;
                            break;
                        case 'k':
                            Board::black_king_side_castle = true;
                            break;
                        case 'q':
                            Board::black_queen_side_castle = true;
                            break;
                    }
                }
                if (spaces == 3) {
                    // if number [0-7]
                    if (int(c) > 47 && int(c) < 56) {
                        Board::en_passant_file = int(c) - 48;
                    }
                }
            }
        }
    }
}

std::string Board::board_to_fen(bool verbose) {
    std::string fen = "";
    int empty_counter = 0;
    int rank_counter = 0;
    for (int i = 0; i < Board::board_size; i++) {
        std::string c = "-";
        if (Board::board[i] != NULL)
            c = Board::board[i]->get_appearance(false);
        if (c != "-") {
            rank_counter += 1;
            fen.push_back(c[0]);
        }
        else {
            while (Board::board[i + empty_counter] == NULL && empty_counter + rank_counter < 8) {
                empty_counter += 1;
            }
            fen.push_back(char(empty_counter + 48));
            i += empty_counter - 1; // adds one less as one more will be added with the iteration
            rank_counter += empty_counter;
            empty_counter = 0;
        }
        if (rank_counter == 8) {
            fen.push_back('/');
            rank_counter = 0;
        }
    }

    fen.pop_back();

    if (verbose) {
        fen.push_back(' ');
        char side_char = Board::side_turn ? 'w' : 'b';
        fen.push_back(side_char);
        fen.push_back(' ');
        if (Board::white_king_side_castle) {
            fen.push_back('K');
        }
        if (Board::white_queen_side_castle) {
            fen.push_back('Q');
        }
        if (Board::black_king_side_castle) {
            fen.push_back('k');
        }
        if (Board::black_queen_side_castle) {
            fen.push_back('q');
        }
        std::string en_passant_file_str = std::to_string(Board::en_passant_file);
        fen.push_back(' ');
        fen += en_passant_file_str;
    }
    
    return fen;
}

// setters --------
void Board::set_initial_position() {
    std::string fen_ip = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq 8";
    Board::set_from_fen(fen_ip);
    Board::set_castle_rights(true, true, true, true);
}

void Board::set_en_passant_file(int file) {
    Board::en_passant_file = file;
}

void Board::set_from_fen(std::string fen, bool overwrite) {
    Board::fen_to_board(fen, overwrite);
}

void Board::set_king(bool side, King* king) {
    if (side) {
        Board::white_king = king;
    }
    else {
        Board::black_king = king;
    }
}

void Board::set_castle_rights(bool white_king_side, bool white_queen_side, bool black_king_side, bool black_queen_side) {
    Board::white_king_side_castle = white_king_side;
    Board::white_queen_side_castle = white_queen_side;
    Board::black_king_side_castle = black_king_side;
    Board::black_queen_side_castle = black_queen_side;
}

void Board::set_player(bool side_turn) {
    Board::side_turn = side_turn;
}

// getters --------
Piece** Board::get_board() {
    return board;
}

King* Board::get_king(bool side) {
    if (side) {
        return Board::white_king;
    }
    else {
        return Board::black_king;
    }
}

bool Board::get_player() {
    return Board::side_turn;
}

std::vector<Piece*> Board::get_pieces(bool side) {
    if (side)
        return Board::white_pieces;
    else
        return Board::black_pieces;
}

Piece* Board::piece_at(int position) {
    return Board::board[position];
}

int Board::get_en_passant_file() {
    return Board::en_passant_file;
}

bool* Board::get_castle_rights() {
    bool rights[] = {Board::white_king_side_castle, Board::white_queen_side_castle, Board::black_king_side_castle, Board::black_queen_side_castle};
    return rights;
}

// Chess I/O --------
std::string Board::get_fen(bool verbose) {
    return Board::board_to_fen(verbose);
}

char* Board::print_board(bool unicode) {
    std::string to_be_printed = "";
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            std::string c = "-";
            if (Board::board[i * 8 + j] != NULL) {
                c = Board::board[i * 8 + j]->get_appearance(false);
                if (unicode)
                    c = unicode_pieces[c];
            }
            to_be_printed.append(c);
            to_be_printed.append(" ");
        }
        to_be_printed.append("\n");
    }

    char* char_arr_board = new char[to_be_printed.length() + 1];
    strcpy(char_arr_board, to_be_printed.c_str());

    return char_arr_board;
}

// Chess Logic --------------

bool Board::is_promotion(std::string players_piece_type, int destination_square_index) {
    return players_piece_type == "P" && ((side_turn && destination_square_index <= 7) || (!side_turn && destination_square_index >= 56));
}

// is square_index attacked by side player
std::vector<int> Board::is_square_attacked_by(int square_index, bool side) {

    std::vector<int> attackers_positions;

    // position = (8 * rank) + file
    int file = square_index % 8;
    int rank = ((square_index - file) / 8);

    // how many ranks up/down files left/right ?
    int ranks_up = rank;
    int ranks_down = (7 - rank);
    int files_left = file;
    int files_right = (7 - file);

    bool nwe_n_diag = true;     // north-west to east - nort diagonal
    bool n_orth = true;         // north orthogonal
    bool new_n_diag = true;     // north-east to west - north diagonal
    bool west_orth = true;      // west orthogonal
    bool east_orth = true;      // east orthogonal
    bool nwe_s_diag = true;     // nort-west to east - south diagonal
    bool s_orth = true;         // south orthogonal
    bool new_s_diag = true;     // nort-east to west - south diagonal

    // finding out if Kings, Queens, Rooks, or Pawns attack the square
    // first we calculate sections of "north-bound" pieces trajectories
    for (int i = 0; i < ranks_up; i++) {

        // first the diagonal from the top left towards the piece
        if (i < files_left && nwe_n_diag) {
            int nort_west_east_diag = square_index - ((i + 1) * 9);
            // if there is a piece
            if (Board::board[nort_west_east_diag] != NULL) {
                // no point in looking the squares behind it
                nwe_n_diag = false;
                Piece* piece = Board::piece_at(nort_west_east_diag);
                std::string piece_type = piece->get_appearance(true);

                // if enemy piece
                if (piece->side == side) {
                    // since diagonal only Queen Bishop and Pawn or King (if close) can attack the square
                    if (piece_type == "Q" || piece_type == "B") {
                        attackers_positions.push_back(nort_west_east_diag);
                    } else if (i==0 && ((!side && piece_type == "P") || piece_type == "K"))
                        attackers_positions.push_back(nort_west_east_diag);
                }
            }
        }

        // then the orthogonal "file" from up to the piece
        if (n_orth) {
            int north_file = square_index - ((i + 1) * 8);

            if (Board::board[north_file] != NULL) {
                n_orth = false;
                Piece* piece = Board::piece_at(north_file);
                std::string piece_type = piece->get_appearance(true);

                // if enemy piece
                if (piece->side == side) {
                    // since diagonal only Queen, Rook can attack the square
                    if (piece_type == "Q" || piece_type == "R") {
                        attackers_positions.push_back(north_file);
                    } else if (i == 0 && piece_type == "K")
                        attackers_positions.push_back(north_file);
                }
            }
        }

        // then the diagonal from the top right to the piece
        if (i < files_right && new_n_diag) {
            int north_east_west_diag = square_index - ((i + 1) * 7);

            if (Board::board[north_east_west_diag] != NULL) {
                // no point in looking the squares behind it
                new_n_diag = false;
                Piece* piece = Board::piece_at(north_east_west_diag);
                std::string piece_type = piece->get_appearance(true);

                // if enemy piece
                if (piece->side == side) {
                    // since diagonal only Queen Bishop and Pawn or King (if close) can attack the square
                    if (piece_type == "Q" || piece_type == "B") {
                        attackers_positions.push_back(north_east_west_diag);
                    }
                    else if (i == 0 && ((!side && piece_type == "P") || piece_type == "K"))
                        attackers_positions.push_back(north_east_west_diag);
                }
            }
        }
    }

    // (left of piece)
    for (int i = 0; i < files_left && west_orth; i++) {
        int west_file = square_index - ((i + 1) * 1);

        if (Board::board[west_file] != NULL) {
            west_orth = false;
            Piece* piece = Board::piece_at(west_file);
            std::string piece_type = piece->get_appearance(true);

            // if enemy piece
            if (piece->side == side) {
                // since diagonal only Queen, Rook can attack the square
                if (piece_type == "Q" || piece_type == "R") {
                    attackers_positions.push_back(west_file);
                }
                else if (i == 0 && piece_type == "K")
                    attackers_positions.push_back(west_file);
            }
        }
    }

    // (right of piece)
    for (int i = 0; i < files_right && east_orth; i++) {
        int east_file = square_index + ((i + 1) * 1);

        if (Board::board[east_file] != NULL) {
            east_orth = false;
            Piece* piece = Board::piece_at(east_file);
            std::string piece_type = piece->get_appearance(true);

            // if enemy piece
            if (piece->side == side) {
                // since diagonal only Queen, Rook can attack the square
                if (piece_type == "Q" || piece_type == "R") {
                    attackers_positions.push_back(east_file);
                }
                else if (i == 0 && piece_type == "K")
                    attackers_positions.push_back(east_file);
            }
        }
    }

    // same as up, but down
    for (int i = 0; i < ranks_down; i++) {

        if (i < files_left && nwe_s_diag) {
            int sotuh_west_east_diag = square_index + ((i + 1) * 7);

            if (Board::board[sotuh_west_east_diag] != NULL) {
                // no point in looking the squares behind it
                nwe_s_diag = false;
                Piece* piece = Board::piece_at(sotuh_west_east_diag);
                std::string piece_type = piece->get_appearance(true);

                // if enemy piece
                if (piece->side == side) {
                    // since diagonal only Queen Bishop and Pawn or King (if close) can attack the square
                    if (piece_type == "Q" || piece_type == "B") {
                        attackers_positions.push_back(sotuh_west_east_diag);
                    }
                    else if (i == 0 && ((side && piece_type == "P") || piece_type == "K"))
                        attackers_positions.push_back(sotuh_west_east_diag);
                }
            }
        }

        if (s_orth) {
            int south_file = square_index + ((i + 1) * 8);

            if (Board::board[south_file] != NULL) {
                s_orth = false;
                Piece* piece = Board::piece_at(south_file);
                std::string piece_type = piece->get_appearance(true);

                // if enemy piece
                if (piece->side == side) {
                    // since diagonal only Queen, Rook can attack the square
                    if (piece_type == "Q" || piece_type == "R") {
                        attackers_positions.push_back(south_file);
                    }
                    else if (i == 0 && piece_type == "K")
                        attackers_positions.push_back(south_file);
                }
            }
        }

        if (i < files_right && new_s_diag) {
            int south_east_west_diag = square_index + ((i + 1) * 9);

            if (Board::board[south_east_west_diag] != NULL) {
                // no point in looking the squares behind it
                new_s_diag = false;
                Piece* piece = Board::piece_at(south_east_west_diag);
                std::string piece_type = piece->get_appearance(true);

                // if enemy piece
                if (piece->side == side) {
                    // since diagonal only Queen Bishop and Pawn or King (if close) can attack the square
                    if (piece_type == "Q" || piece_type == "B") {
                        attackers_positions.push_back(south_east_west_diag);
                    }
                    else if (i == 0 && ((side && piece_type == "P") || piece_type == "K"))
                        attackers_positions.push_back(south_east_west_diag);
                }
            }
        }
    }

    // finding out if knights attack the square
    // if at least 2 ranks north of the piece
    if (rank > 1) {

        // if enough room (at least 1 file) to the left
        if (file > 0) {
            Piece* piece = Board::board[square_index - 17];
            if (piece && piece->side == side && piece->get_appearance(true) == "N")
                attackers_positions.push_back(square_index - 17);
        }

        // same but right
        if (file < 7) {
            Piece* piece = Board::board[square_index - 15];
            if (piece && piece->side == side && piece->get_appearance(true) == "N")
                attackers_positions.push_back(square_index - 15);
        }
    }

    // if at least 1 rank north of the piece
    if (rank > 0) {
        if (file > 1) {
            Piece* piece = Board::board[square_index - 10];
            if (piece && piece->side == side && piece->get_appearance(true) == "N")
                attackers_positions.push_back(square_index - 10);
        }
        if (file < 6) {
            Piece* piece = Board::board[square_index - 6];
            if (piece && piece->side == side && piece->get_appearance(true) == "N")
                attackers_positions.push_back(square_index - 6);
        }
    }

    //  same but south
    if (rank < 7) {
        if (file > 1) {
            Piece* piece = Board::board[square_index + 6];
            if (piece && piece->side == side && piece->get_appearance(true) == "N")
                attackers_positions.push_back(square_index + 6);
        }
        if (file < 6) {
            Piece* piece = Board::board[square_index + 10];
            if (piece && piece->side == side && piece->get_appearance(true) == "N")
                attackers_positions.push_back(square_index + 10);
        }
    }

    if (rank < 6) {
        if (file > 0) {
            Piece* piece = Board::board[square_index + 15];
            if (piece && piece->side == side && piece->get_appearance(true) == "N")
                attackers_positions.push_back(square_index + 15);
        }
        if (file < 7) {
            Piece* piece = Board::board[square_index + 17];
            if (piece && piece->side == side && piece->get_appearance(true) == "N")
                attackers_positions.push_back(square_index + 17);
        }
    }

    return attackers_positions;
}

bool Board::is_move_legal(Piece* piece, int destination_square, bool check_pseudo_legality) {

    bool is_legal = false;

    if (!piece) {
        return is_legal;
    }

    std::vector<int> pseudo_legal_move_list = piece->pseudo_legal_moves(this);
    // if move is pseudolegal, i check if the king would be in check after the move
    if (check_pseudo_legality) {
        if (!std::count(pseudo_legal_move_list.begin(), pseudo_legal_move_list.end(), destination_square))
            return false;
    }

    // Code to "simulate" the move in order to verify if the king would end in check    ----------
    int backup_en_passant_file = Board::en_passant_file;
    int backup_piece_original_position = piece->position;

    Piece* backup_taken_piece = Board::move_piece_to(destination_square, piece);

    int king_square = Board::get_king(piece->side)->position;
    // if AFTER THE MOVE the king is NOT in check, then move is LEGAL
    
    if(Board::is_square_attacked_by(king_square, !piece->side).size()==0)
        is_legal = true;
    
    // undoing move
    Board::en_passant_file = backup_en_passant_file;

    // moving piece back
    Board::board[backup_piece_original_position] = piece;
    piece->position = backup_piece_original_position;

    // moving taken piece back
    Board::board[destination_square] = NULL;
    if (backup_taken_piece) {
        Board::board[backup_taken_piece->position] = backup_taken_piece;
        std::vector<Piece*>& enemy_pieces = piece->side ? Board::black_pieces : Board::white_pieces;
        enemy_pieces.push_back(backup_taken_piece);
    }

    return is_legal;
}

// side just gave a check, is it mate?
bool Board::is_mate(bool side) {

    Piece* king = side ? Board::black_king : Board::white_king;
    std::vector<Piece*>& pieces = side ? Board::black_pieces : Board::white_pieces;

    if (king->is_attacked(this).size()!=0) {
        for (Piece* piece : pieces) {
            std::vector<int> moves = piece->pseudo_legal_moves(this);
            for (int move : moves) {
                if (is_move_legal(piece, move, false)) {
                    return false;
                }
            }
        }
        return true;
    }
    return false;
}

bool Board::is_draw_by_insufficient_material() {
    int pieces = 0;

    if (Board::white_pieces.size() < 3 && Board::black_pieces.size() < 3) {
        for (Piece* p : Board::white_pieces) {
            std::string piece_type = p->get_appearance(true);
            if (piece_type == "R" || piece_type == "Q" || piece_type == "P") {
                return false;
            }
        }
        for (Piece* p : Board::black_pieces) {
            std::string piece_type = p->get_appearance(true);
            if (piece_type == "R" || piece_type == "Q" || piece_type == "P") {
                return false;
            }
        }
        return true;
    }
    return false;
}

// side just made a move, is it stalemate?
bool Board::is_stalemate(bool side) {
    Piece* king = side ? Board::black_king : Board::white_king;
    std::vector<Piece*>& pieces = side ? Board::black_pieces : Board::white_pieces;

    // if enemy king not in check
    if (king->is_attacked(this).size()==0) {
        // if legal moves, not stalemate, else stalemate
        for (Piece* piece : pieces) {
            std::vector<int> moves = piece->pseudo_legal_moves(this);
            if (moves.size()>0) {
                for (int move : moves) {
                    if (is_move_legal(piece, move, false)) {
                        return false;
                    }
                }
            }
        }
        return true;
    }
    return false;
}

bool Board::is_threefold_repetition() {
    for (std::string f : Board::positions_fens) {
        if (std::count(Board::positions_fens.begin(), Board::positions_fens.end(), f) == 3)
            return true;
    }
    return false;
}

// looks for pieces in the way or squares attacked
bool Board::can_castle(char castle_type, bool side_turn) {

    if (castle_type == 'K' && side_turn) {
        if (Board::white_king_side_castle) {
            // if no pieces in the way
            if (Board::board[61] == NULL && Board::board[62] == NULL) {
                // if squares aren't attacked (including king isn't in check)
                if (Board::is_square_attacked_by(60, !side_turn).size()==0 && Board::is_square_attacked_by(61, !side_turn).size() == 0 && Board::is_square_attacked_by(62, !side_turn).size() == 0) {
                    return true;
                }
            }
        }
    }

    if (castle_type == 'Q' && side_turn) {
        if (Board::white_queen_side_castle) {
            // if no pieces in the way
            if (Board::board[59] == NULL && Board::board[58] == NULL && Board::board[57] == NULL) {
                // if squares aren't attacked (including king isn't in check)
                if (Board::is_square_attacked_by(60, !side_turn).size() == 0 && Board::is_square_attacked_by(59, !side_turn).size() == 0 && Board::is_square_attacked_by(58, !side_turn).size() == 0) {
                    return true;
                }
            }
        }
    }

    if (castle_type == 'K' && !side_turn) {
        if (Board::black_king_side_castle) {
            if (Board::board[5] == NULL && Board::board[6] == NULL) {
                if (Board::is_square_attacked_by(4, !side_turn).size() == 0 && Board::is_square_attacked_by(5, !side_turn).size() == 0 && Board::is_square_attacked_by(6, !side_turn).size() == 0) {
                    return true;
                }
            }
        }
    }

    if (castle_type == 'Q' && !side_turn) {
        if (Board::black_queen_side_castle) {
            if (Board::board[3] == NULL && Board::board[2] == NULL && Board::board[1] == NULL) {
                if (Board::is_square_attacked_by(4, !side_turn).size() == 0 && Board::is_square_attacked_by(3, !side_turn).size() == 0 && Board::is_square_attacked_by(2, !side_turn).size() == 0) {
                    return true;
                }
            }
        }
    } 
            
    return false;
}

void Board::update_castling_rights(std::string players_piece_type, std::string taken_piece_type, int origin_square_index, int destination_square_index) {

    // if king moves, remove all castling rights
    if (players_piece_type == "K" && side_turn && (Board::white_king_side_castle || Board::white_queen_side_castle)) {
        Board::white_king_side_castle = false;
        Board::white_queen_side_castle = false;
    }
    else if (players_piece_type == "K" && !side_turn && (Board::black_king_side_castle || Board::black_queen_side_castle)) {
        Board::black_king_side_castle = false;
        Board::black_queen_side_castle = false;
    }
    // if rook moves, remove corresponding castling right
    if (players_piece_type == "R" && side_turn && Board::white_king_side_castle && origin_square_index == 63) {
        Board::white_king_side_castle = false;
    }
    else if (players_piece_type == "R" && side_turn && Board::white_queen_side_castle && origin_square_index == 56) {
        Board::white_queen_side_castle = false;
    }
    else if (players_piece_type == "R" && !side_turn && Board::black_king_side_castle && origin_square_index == 7) {
        Board::black_king_side_castle = false;
    }
    else if (players_piece_type == "R" && !side_turn && Board::black_queen_side_castle && origin_square_index == 0) {
        Board::black_queen_side_castle = false;
    }

    // if rook is taken, remove corresponding castling right
    if (taken_piece_type == "R") {

        if (side_turn && Board::black_king_side_castle && destination_square_index == 7) {
            Board::black_king_side_castle = false;
        }
        else if (side_turn && Board::black_queen_side_castle && destination_square_index == 0) {
            Board::black_queen_side_castle = false;
        }
        else if (!side_turn && Board::white_king_side_castle && destination_square_index == 63) {
            Board::white_king_side_castle = false;
        }
        else if (!side_turn && Board::white_queen_side_castle && destination_square_index == 56) {
            Board::white_queen_side_castle = false;
        }
    }
}

// assumes the square is empty
void Board::move_piece_to_empty_square(int position, Piece* piece) {

    int original_position = piece->position;

    // remove piece from original position
    Board::board[original_position] = NULL;

    // put piece in position
    Board::board[position] = piece;

    // piece has moved, updating pieces internal position
    piece->position = position;
}

// equivalent of taking the piece and moving it into position (removing other piece if there are any)
Piece* Board::move_piece_to(int position, Piece* piece) {

    // if there is a piece where the piece moves to
    // remove piece (from board and piece list)
    // put new piece

    int original_piece_position = piece->position;
    std::string piece_type = piece->get_appearance(true);

    bool takes_a_piece = false;
    int taken_piece_position = -1;

    Piece* taken_piece = NULL;

    // if piece where piece lands, is a take
    if (Board::board[position]) {
        takes_a_piece = true;
        taken_piece_position = position;
    } 
    // if en passant, is a take
    else if (Board::en_passant_file != 8) {
        // useful values
        int side_multiplier = piece->side ? -1 : 1;
        int file = original_piece_position % 8;
        int rank = ((original_piece_position - file) / 8);
        int file_distance = file - Board::en_passant_file;

        // if piece is pawn and adjacent to en passant file
        if (piece_type == "P" && abs(file_distance) == 1 && ((rank == 3 && side_multiplier == -1) || (rank == 4 && side_multiplier == 1))) {
            // if pawn tries to take en passant
            if (position == original_piece_position + side_multiplier * (8 - (side_multiplier * file_distance))) {
                takes_a_piece = true;
                taken_piece_position = position - side_multiplier * 8;
            }
        }
    }

    // take piece
    if (takes_a_piece) {
        // remove taken piece from list, no deleting as a pointer to the piece is saved in Ply and can be used to undo the move
        taken_piece = Board::board[taken_piece_position];
        std::vector<Piece*>& enemy_pieces = piece->side ? Board::black_pieces : Board::white_pieces;
        for (int i = 0; i < enemy_pieces.size(); i++) {
            if (enemy_pieces[i] == taken_piece) {
                enemy_pieces.erase(enemy_pieces.begin() + i);
                break;
            }
        }

        // remove taken piece from square
        Board::board[taken_piece_position] = NULL;
    }

    // move piece
    Board::move_piece_to_empty_square(position, piece);

    return taken_piece;
}

void Board::castle(std::string move) {

    bool side_turn = Board::side_turn;

    if ((side_turn && (move == "O-O" || move == "O-O-O")) || (!side_turn && (move == "o-o" || move == "o-o-o"))) {

        // WHITE ==============
        if (side_turn) {
            if (move == "O-O" && can_castle('K', true)) {
                Board::move_piece_to_empty_square(62, Board::piece_at(60));
                Board::move_piece_to_empty_square(61, Board::piece_at(63));
                Board::white_king_side_castle = false;
                Board::white_queen_side_castle = false;
                return;
            }
            else if (side_turn && move == "O-O") {
                throw std::invalid_argument("Illegal move, the player does not have the right to castle");
            }

            if (move == "O-O-O" && can_castle('Q', true)) {
                Board::move_piece_to_empty_square(58, Board::piece_at(60));
                Board::move_piece_to_empty_square(59, Board::piece_at(56));
                Board::white_king_side_castle = false;
                Board::white_queen_side_castle = false;
                return;
            }
            else if (side_turn && move == "O-O-O") {
                throw std::invalid_argument("Illegal move, the player does not have the right to castle");
            }
        }
        else {
            // BLACK ==============
            // black king side castles and has the right
            if (move == "o-o" && can_castle('K', false)) {
                Board::move_piece_to_empty_square(6, Board::piece_at(4));
                Board::move_piece_to_empty_square(5, Board::piece_at(7));
                Board::black_king_side_castle = false;
                Board::black_queen_side_castle = false;
                return;
            }
            else if (!side_turn && move == "o-o") {
                throw std::invalid_argument("Illegal move, the player does not have the right to castle");
            }

            // black queen side castles and has the right
            if (move == "o-o-o" && can_castle('Q', false)) {
                Board::move_piece_to_empty_square(2, Board::piece_at(4));
                Board::move_piece_to_empty_square(3, Board::piece_at(0));
                Board::black_king_side_castle = false;
                Board::black_queen_side_castle = false;
                return;
            }
            else if (!side_turn && move == "o-o-o") {
                throw std::invalid_argument("Illegal move, the player does not have the right to castle");
            }
        }
    }
    else {
        throw std::invalid_argument("Illegal move");
    }
}

void Board::un_castle(std::string move, bool side_turn) {

    if ((side_turn && (move == "O-O" || move == "O-O-O")) || (!side_turn && (move == "o-o" || move == "o-o-o"))) {

        // WHITE ==============
        if (side_turn) {
            if (move == "O-O") {
                Board::move_piece_to_empty_square(60, Board::piece_at(62));
                Board::move_piece_to_empty_square(63, Board::piece_at(61));
                Board::white_king_side_castle = true;
                Board::white_queen_side_castle = true;
                return;
            }
            else if (side_turn && move == "O-O") {
                throw std::invalid_argument("Illegal move, the player does not have the right to castle");
            }

            if (move == "O-O-O") {
                Board::move_piece_to_empty_square(60, Board::piece_at(58));
                Board::move_piece_to_empty_square(56, Board::piece_at(59));
                Board::white_king_side_castle = true;
                Board::white_queen_side_castle = true;
                return;
            }
            else if (side_turn && move == "O-O-O") {
                throw std::invalid_argument("Illegal move, the player does not have the right to castle");
            }
        }
        else {
            // BLACK ==============
            // black king side castles and has the right
            if (move == "o-o") {
                Board::move_piece_to_empty_square(4, Board::piece_at(6));
                Board::move_piece_to_empty_square(7, Board::piece_at(5));
                Board::black_king_side_castle = true;
                Board::black_queen_side_castle = true;
                return;
            }
            else if (!side_turn && move == "o-o") {
                throw std::invalid_argument("Illegal move, the player does not have the right to castle");
            }

            // black queen side castles and has the right
            if (move == "o-o-o") {
                Board::move_piece_to_empty_square(4, Board::piece_at(2));
                Board::move_piece_to_empty_square(0, Board::piece_at(3));
                Board::black_king_side_castle = true;
                Board::black_queen_side_castle = true;
                return;
            }
            else if (!side_turn && move == "o-o-o") {
                throw std::invalid_argument("Illegal move, the player does not have the right to castle");
            }
        }
    }
    else {
        throw std::invalid_argument("Illegal move");
    }
}

Piece* Board::promotion(std::string move, bool player) {
    
    int destination_square_index = coordinates_to_board_index(move.substr(4, 2));

    Piece* promote_to;
    if (move.size() == 8) {
        if (move.substr(7, 1) == "R") {
            promote_to = new Rook(player, destination_square_index);
        }
        else if (move.substr(7, 1) == "B") {
            promote_to = new Bishop(player, destination_square_index);
        }
        else if (move.substr(7, 1) == "N") {
            promote_to = new Knight(player, destination_square_index);
        }
        else {
            promote_to = new Queen(player, destination_square_index);
        }
    }
    else {
        promote_to = new Queen(player, destination_square_index);
    }

    // promotion is called after the pawn is put in its destination square
    // update piece list by removing pawn and inserting new piece, updating board subsequently
    std::vector<Piece*>& pieces = player ? (Board::white_pieces) : (Board::black_pieces);
    int i = 0;
    for (int i = 0; i < pieces.size(); i++) {
        if (pieces[i]->position == destination_square_index) {
            pieces.erase(pieces.begin() + i);
            break;
        }
    }
    // updating list: removing pawn, adding piece
    pieces.push_back(promote_to);

    // not deleting the pawn as it might be unmoved into the board
    // putting pience on the board
    Board::board[destination_square_index] = promote_to;

    return promote_to;
}

void Board::force_move(Ply* ply) {
    std::string move_notation = ply->get_move_notation();

    Piece* ply_moving_piece = NULL;
    Piece* ply_taken_piece = NULL;
    Piece* ply_promotion_piece = NULL;
    int ply_origin_square = -1;
    int ply_destination_square = -1;
    int ply_taken_square = -1;
    bool castle = false;
    bool promotion = false;

    // these need to be saved before making the move:
    ply->set_castle_rights(Board::white_king_side_castle, Board::white_queen_side_castle, Board::black_king_side_castle, Board::black_queen_side_castle);
    ply->set_en_passant_file(Board::en_passant_file);

    try {
        //move_notation == "O-O" || move_notation == "O-O-O" || move_notation == "o-o" || move_notation == "o-o-o"
        if (move_notation[0] == 'O' || move_notation[0] == 'o') {
            Board::castle(move_notation);
            // updating en passant
            Board::en_passant_file = 8;
            // setting ply parameters
            castle = true;
        }
        else {
            int origin_square_index = coordinates_to_board_index(move_notation.substr(1, 2));
            int destination_square_index = coordinates_to_board_index(move_notation.substr(4, 2));

            ply_origin_square = origin_square_index;
            ply_destination_square = destination_square_index;

            Piece* players_piece = Board::board[origin_square_index];
            Piece* taken_piece = Board::board[destination_square_index];
            ply_moving_piece = players_piece;

            std::string players_piece_type = players_piece->get_appearance(true);
            std::string taken_piece_type = taken_piece != NULL ? Board::board[destination_square_index]->get_appearance(true) : "";

            // en passant is dealt with in here
            ply_taken_piece = Board::move_piece_to(destination_square_index, players_piece);

            if (ply_taken_piece)
                ply_taken_square = ply_taken_piece->position;

            // if move is promotion, promote, update of pieces list is made in promotion
            if (Board::is_promotion(players_piece_type, destination_square_index)) {
                ply_promotion_piece = Board::promotion(move_notation, side_turn);
                promotion = true;
            }

            // updating castling rights --------
            // done only here since castling updates the rights on its own
            // once both players can't castle, there's no need to try to update their rights
            if (Board::white_king_side_castle || Board::black_king_side_castle || Board:: white_queen_side_castle || Board::black_queen_side_castle)
                Board::update_castling_rights(players_piece_type, taken_piece_type, origin_square_index, destination_square_index);

            // updating en passant
            // if move is pawn 2 forward then en passant is possible
            if (players_piece_type == "P" && abs(destination_square_index - origin_square_index) > 15) {
                int file = origin_square_index % 8;
                Board::en_passant_file = file;
            }
            else {
                Board::en_passant_file = 8;
            }
        }
        // after a move has been made: change sides_turn set ply's parameters
        Board::side_turn = !side_turn;
        ply->set_moving_piece(ply_moving_piece);
        ply->set_taken_piece(ply_taken_piece);
        ply->set_promotion_piece(ply_promotion_piece);
        ply->set_origin_square(ply_origin_square);
        ply->set_destination_square(ply_destination_square);
        ply->set_taken_square(ply_taken_square);
        ply->set_castle(castle);
        ply->set_promotion(promotion);

        // this has to be done only when the move is actually made, not when the move is "tried" by the AI
        // positions_fens.push_back(Board::get_fen(true));
    }
    catch (const std::invalid_argument& e) {
        throw e;
    }
}

// Easy Move Notation EMN
// basically normal algebraic notation but indicating origin square and piece to make it easier to parse
// also doesnt distinguish between takes and just move
// expected notation example: Bf4-d6
// short castle O-O long castle O-O-O for white, for black same but small
void Board::move(Ply* ply) {
    std::string move_notation = ply->get_move_notation();

    Piece* ply_moving_piece = NULL;
    Piece* ply_taken_piece = NULL;
    Piece* ply_promotion_piece = NULL;
    int ply_origin_square = -1;
    int ply_destination_square = -1;
    int ply_taken_square = -1;
    bool castle = false;
    bool promotion = false;

    // these need to be saved before making the move:
    ply->set_castle_rights(Board::white_king_side_castle, Board::white_queen_side_castle, Board::black_king_side_castle, Board::black_queen_side_castle);
    ply->set_en_passant_file(Board::en_passant_file);

    try {
        //move_notation == "O-O" || move_notation == "O-O-O" || move_notation == "o-o" || move_notation == "o-o-o"
        if (move_notation[0] == 'O' || move_notation[0] == 'o') {
            Board::castle(move_notation);
            // updating en passant
            Board::en_passant_file = 8;
            // setting ply parameters
            castle = true;
        }
        else {
            int origin_square_index = coordinates_to_board_index(move_notation.substr(1, 2));
            int destination_square_index = coordinates_to_board_index(move_notation.substr(4, 2));

            ply_origin_square = origin_square_index;
            ply_destination_square = destination_square_index;

            Piece* players_piece = Board::board[origin_square_index];
            Piece* taken_piece = Board::board[destination_square_index];
            ply_moving_piece = players_piece;

            std::string players_piece_type = players_piece->get_appearance(true);
            std::string taken_piece_type = taken_piece != NULL ? Board::board[destination_square_index]->get_appearance(true) : "";

            // early exit if move notation is incorrect (no piece at coordinates)
            if (players_piece == NULL)
                throw std::invalid_argument("Nothing at origin coordinates");
            // early exit if move notation is incorrect (wrong piece indicated)
            if (players_piece_type != std::string(1, move_notation[0]))
                throw std::invalid_argument("Origin coordinates do not correspond with given piece");
            // early exit if piece isn't on players side
            if (players_piece->side != Board::side_turn)
                throw std::invalid_argument("Piece isn't on the player's side");
            // exit if move is illegal
            if (!is_move_legal(players_piece, destination_square_index, true))
                throw std::invalid_argument("Illegal move (king must be in check ?)");

            // en passant is dealt with in here
            ply_taken_piece = Board::move_piece_to(destination_square_index, players_piece);

            if (ply_taken_piece)
                ply_taken_square = ply_taken_piece->position;

            // if move is promotion, promote, update of pieces list is made in promotion
            if (Board::is_promotion(players_piece_type, destination_square_index)) {
                ply_promotion_piece = Board::promotion(move_notation, side_turn);
                promotion = true;
            }

            // updating castling rights --------
            // done only here since castling updates the rights on its own
            Board::update_castling_rights(players_piece_type, taken_piece_type, origin_square_index, destination_square_index);

            // updating en passant
            // if move is pawn 2 forward then en passant is possible
            if (players_piece_type == "P" && abs(destination_square_index - origin_square_index) > 15) {
                int file = origin_square_index % 8;
                Board::en_passant_file = file;
            }
            else {
                Board::en_passant_file = 8;
            }
        }
        // after a move has been made: change sides_turn set ply's parameters
        Board::side_turn = !side_turn;        
        ply->set_moving_piece(ply_moving_piece);
        ply->set_taken_piece(ply_taken_piece);
        ply->set_promotion_piece(ply_promotion_piece);
        ply->set_origin_square(ply_origin_square);
        ply->set_destination_square(ply_destination_square);
        ply->set_taken_square(ply_taken_square);
        ply->set_castle(castle);
        ply->set_promotion(promotion);

        // this has to be done only when the move is actually made, not when the move is "tried" by the AI
        // positions_fens.push_back(Board::get_fen(true));
    }
    catch (const std::invalid_argument& e) {
        throw e;
    }
}

// ply is the move to undo
void Board::undo_move(Ply* ply) {

    bool* rights = ply->get_castle_rights();
    Board::set_castle_rights(rights[0], rights[1], rights[2], rights[3]);
    Board::set_en_passant_file(ply->get_en_passant_file());

    if (ply->is_castle()) {
        Board::un_castle(ply->get_move_notation(), ply->get_player());
    }
    else {
        int origin_square = ply->get_origin_square();
        Piece* players_piece = ply->get_moving_piece();
        Board::board[origin_square] = players_piece;
        players_piece->position = origin_square;
        
        bool en_passant = ply->is_en_passant();

        if (en_passant) {
            // if en passant the pawn is in a different square
            int taken_square = ply->get_taken_square();
            Piece* taken_piece = ply->get_taken_piece();
            Board::board[taken_square] = taken_piece;
            taken_piece->position = taken_square;
        }

        // this puts the taken piece where it was if there was any, otherwise NULL
        int destination_square = ply->get_destination_square();
        Piece* taken_piece = ply->get_taken_piece();
        Board::board[destination_square] = en_passant ? NULL : taken_piece;
        if (taken_piece) {
            std::vector<Piece*>& enemy_pieces = ply->get_player() ? Board::black_pieces : Board::white_pieces;
            enemy_pieces.push_back(taken_piece);
        }
        
        if (ply->is_promotion()) {
            // if promotion remove the new piece from the list of pieces and add the pawn
            Piece* promotion_piece = ply->get_promotion_piece();
            std::vector<Piece*>& pieces = ply->get_player() ? Board::white_pieces : Board::black_pieces;
            for (int i = 0; i < pieces.size(); i++) {
                if (pieces[i] == promotion_piece) {
                    pieces.erase(pieces.begin() + i);
                    break;
                }
            }
            delete promotion_piece;

            pieces.push_back(players_piece);
        }
    }
    Board::side_turn = !Board::side_turn;
}
