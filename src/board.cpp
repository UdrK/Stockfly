#include <stdexcept>
#include "board.h"
#include "piece.h"
#include "king.h"
#include "queen.h"
#include "rook.h"
#include "bishop.h"
#include "knight.h"
#include "pawn.h"
#include "utils.h"

Board::Board(std::string fen) {
    Board::board = new Piece * [64];
    if (!fen.empty())
        set_from_fen(fen);
    else
        set_initial_position();
}

// private utility methods  --------
void Board::fen_to_board(std::string fen) {
    int index = 0;

    for (int i = 0; i < 64; i++) {
        Board::board[i] = NULL;
    }

    for (auto c : fen) {
        if (c == ' ') break;
        if (c != '/') {
            if (int(c) > 48 && int(c) < 57) {
                int empty_squares = int(c) - 48;
                for (int i = 0; i < empty_squares; i++) {
                    Board::board[index + i] = NULL;
                }
                index += empty_squares;
            }
            else {
                switch (c) {
                case 'K': case 'k':
                    if (c == 'K') {
                        Board::white_king = new King(c == 'K', index);
                        Board::board[index] = Board::white_king;
                    }
                    else {
                        Board::black_king = new King(c == 'K', index);
                        Board::board[index] = Board::black_king;
                    }
                    break;
                case 'Q': case 'q':
                    Board::board[index] = new Queen(c == 'Q', index);
                    break;
                case 'R': case 'r':
                    Board::board[index] = new Rook(c == 'R', index);
                    break;
                case 'N': case 'n':
                    Board::board[index] = new Knight(c == 'N', index);
                    break;
                case 'B': case 'b':
                    Board::board[index] = new Bishop(c == 'B', index);
                    break;
                case 'P': case 'p':
                    Board::board[index] = new Pawn(c == 'P', index);
                    break;
                }
                index += 1;
            }
        }
    }
}

std::string Board::board_to_fen() {
    std::string fen = "";
    int empty_counter = 0;
    int rank_counter = 0;
    for (int i = 0; i < Board::board_size; i += 1) {
        std::string c = "-";
        if (Board::board[i] != NULL)
            c = Board::board[i]->get_appearance();
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
    return fen;
}

// setters --------
void Board::set_initial_position() {
    std::string fen_ip = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    Board::set_from_fen(fen_ip);
    Board::set_castle_rights(true, true, true, true);
}

void Board::set_en_passant_file(int file) {
    Board::en_passant_file = file;
}

void Board::set_from_fen(std::string fen) {
    Board::fen_to_board(fen);
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

void Board::move_piece_to(int position, Piece* piece) {
    
    // remove piece from original square
    int original_piece_position = piece->position;
    Board::board[original_piece_position] = NULL;

    // if there is already a piece in -position- then set its position to -1 (piece has been taken)
    if (Board::board[position])
        Board::board[position]->position = -1;

    // piece goes to position
    Board::board[position] = piece;

    // piece has moved, updating pieces internal position
    piece->position = position;

    std::string str = piece->get_appearance();
    char piece_type = std::toupper(char(str[0]));

    // dealing with en passant ------------------
    // if en passant is possible
    if (Board::en_passant_file != -1) {

        // useful values
        int side_multiplier = piece->side ? -1 : 1;
        int file = original_piece_position % 8;
        int rank = ((original_piece_position - file) / 8);
        int file_distance = file - Board::en_passant_file;

        // if piece is pawn and adjacent to en passant file
        if (piece_type == 'P' && abs(file_distance) == 1) {
            // if pawn tries to take en passant
            if (position == original_piece_position + side_multiplier * (8 - (side_multiplier * file_distance))) {
                // pawn is taking en passant, therefore we remove the enemy pawn
                Board::board[position - side_multiplier * 8] = NULL;
            }
        }
    } 
    // if pawn moves 2 forward en passant is possible
    if (piece_type == 'P' && abs(position - original_piece_position) > 15) {
        int file = original_piece_position % 8;
        Board::en_passant_file = file;
    }
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
std::string Board::get_fen() {
    return Board::board_to_fen();
}

char* Board::print_board(bool unicode) {
    std::string to_be_printed = "";
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            std::string c = "-";
            if (Board::board[i * 8 + j] != NULL) {
                c = Board::board[i * 8 + j]->get_appearance();
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

// is square_index attacked by side player
bool Board::is_square_attacked(int square_index, bool side) {

    // side is used in the code to verify if !side attacks the square, but the method is called to verify is side attacks the square
    // maybe????
    side = !side;
    // if piece was a queen/rook would it see an enemy queen/rook ? if yes piece is attacked
    std::vector<int> rook_movement_squares = Piece::piece_movement(this, square_index, side, false, false, true);
    for (int square : rook_movement_squares) {
        if (Board::piece_at(square)) {
            std::string str = Board::piece_at(square)->get_appearance();
            char piece_type = std::toupper(char(str[0]));
            if (Board::piece_at(square)->side != side && piece_type == 'Q' || piece_type == 'R')
                return true;
        }
    }

    // if piece was a queen/bishop would it see an enemy queen/bishop ? if yes piece is attacked
    std::vector<int> bishop_movement_squares = Piece::piece_movement(this, square_index, side, false, true, false);
    for (int square : bishop_movement_squares) {
        if (Board::piece_at(square)) {
            std::string str = Board::piece_at(square)->get_appearance();
            char piece_type = std::toupper(char(str[0]));
            if (Board::piece_at(square)->side != side && piece_type == 'Q' || piece_type == 'B')
                return true;
        }
    }

    // if piece was a knight would it see an enemy knight ? if yes piece is attacked
    std::vector<int> knight_movement_squares = Piece::knight_movement(this, square_index, side);
    for (int square : knight_movement_squares) {
        if (Board::piece_at(square)) {
            std::string str = Board::piece_at(square)->get_appearance();
            char piece_type = std::toupper(char(str[0]));
            if (Board::piece_at(square)->side != side && piece_type == 'N')
                return true;
        }
    }

    // if piece was a pawn would it attack an enemy pawn ? if yes piece is attacked
    int side_multiplier = side ? -1 : 1;
    int take_square_one = square_index + side_multiplier * 7;
    int take_square_two = square_index + side_multiplier * 9;
    std::vector<int> pawn_attacked_squares = { take_square_one, take_square_two };
    for (int square : pawn_attacked_squares) {
        if (Board::piece_at(square)) {
            std::string str = Board::piece_at(square)->get_appearance();
            char piece_type = std::toupper(char(str[0]));
            if (Board::piece_at(square)->side != side && piece_type == 'P')
                return true;
        }
    }

    return false;
}

bool Board::is_move_legal(Piece* piece, int destination_square) {

    bool is_legal = false;

    if (!piece) {
        return is_legal;
    }

    std::vector<int> pseudo_legal_move_list = piece->pseudo_legal_moves(this);
    // if move is pseudolegal, i check if the king would be in check after the move
    if (std::count(pseudo_legal_move_list.begin(), pseudo_legal_move_list.end(), destination_square)) {

        // Code to "simulate" the move in order to verify if the king would end in check    ----------
        // handles
        int my_piece_original_position = piece->position;
        Piece* destination_square_piece = Board::piece_at(destination_square);
        // "making" the move
        Board::move_piece_to(destination_square, piece);

        // if AFTER THE MOVE the king is NOT in check, then move is LEGAL
        if ( ! Board::get_king(piece->side)->is_attacked(this))
            is_legal = true;

        // "unmaking" the move
        Board::move_piece_to(my_piece_original_position, piece);

        if (!destination_square_piece) {
            Board::board[destination_square] = NULL;
        }
        else {
            Board::move_piece_to(destination_square, destination_square_piece);
        }
        
    }
    return is_legal;
}

void Board::castle(std::string move, bool side_turn) {

    if ((side_turn && (move == "O-O" || move == "O-O-O")) || (!side_turn && (move == "o-o" || move == "o-o-o"))) {

        // WHITE ==============

        // white king side castles and has the right
        if (side_turn && move == "O-O" && Board::white_king_side_castle) {
            // if no pieces in the way
            if (Board::board[61] == NULL && Board::board[62] == NULL) {
                // if squares aren't attacked (including king isn't in check)
                if (!Board::is_square_attacked(60, !side_turn) && !Board::is_square_attacked(61, !side_turn) && !Board::is_square_attacked(62, !side_turn)) {
                    Board::move_piece_to(62, Board::piece_at(60));
                    Board::move_piece_to(61, Board::piece_at(63));
                    Board::white_king_side_castle = false;
                    Board::white_queen_side_castle = false;
                    return;
                }
                else {
                    throw std::invalid_argument("Illegal move, castle square is attacked");
                }
            }
            else {
                throw std::invalid_argument("Illegal move, can't castle with pieces in the way");
            }
        }
        else if (side_turn && move == "O-O") {
            throw std::invalid_argument("Illegal move, the player does not have the right to castle");
        }
        
        // white queen side castles and has the right
        if (side_turn && move == "O-O-O" && Board::white_queen_side_castle) {
            // if no pieces in the way
            if (Board::board[59] == NULL && Board::board[58] == NULL && Board::board[57] == NULL) {
                // if squares aren't attacked (including king isn't in check)
                if (!Board::is_square_attacked(60, !side_turn) && !Board::is_square_attacked(59, !side_turn) && !Board::is_square_attacked(58, !side_turn)) {
                    Board::move_piece_to(58, Board::piece_at(60));
                    Board::move_piece_to(59, Board::piece_at(56));
                    Board::white_king_side_castle = false;
                    Board::white_queen_side_castle = false;
                    return;
                }
                else {
                    throw std::invalid_argument("Illegal move, castle square is attacked");
                }
            }
            else {
                throw std::invalid_argument("Illegal move, can't castle with pieces in the way");
            }
        }
        else if (side_turn && move == "O-O-O") {
            throw std::invalid_argument("Illegal move, the player does not have the right to castle");
        }
        
        // BLACK ==============

        // black king side castles and has the right
        if (!side_turn && move == "o-o" && Board::black_king_side_castle) {
            // if no pieces in the way
            if (Board::board[5] == NULL && Board::board[6] == NULL) {
                // if squares aren't attacked (including king isn't in check)
                if (!Board::is_square_attacked(4, !side_turn) && !Board::is_square_attacked(5, !side_turn) && !Board::is_square_attacked(6, !side_turn)) {
                    Board::move_piece_to(6, Board::piece_at(4));
                    Board::move_piece_to(5, Board::piece_at(7));
                    Board::black_king_side_castle = false;
                    Board::black_queen_side_castle = false;
                    return;
                }
                else {
                    throw std::invalid_argument("Illegal move, castle square is attacked");
                }
            }
            else {
                throw std::invalid_argument("Illegal move, can't castle with pieces in the way");
            }
        }
        else if (!side_turn && move == "o-o") {
            throw std::invalid_argument("Illegal move, the player does not have the right to castle");
        }
        
        // black queen side castles and has the right
        if (!side_turn && move == "o-o-o" && Board::black_queen_side_castle) {
            // if no pieces in the way
            if (Board::board[3] == NULL && Board::board[2] == NULL && Board::board[1] == NULL) {
                // if squares aren't attacked (including king isn't in check)
                if (!Board::is_square_attacked(4, !side_turn) && !Board::is_square_attacked(3, !side_turn) && !Board::is_square_attacked(2, !side_turn)) {
                    Board::move_piece_to(2, Board::piece_at(4));
                    Board::move_piece_to(3, Board::piece_at(0));
                    Board::black_king_side_castle = false;
                    Board::black_queen_side_castle = false;
                    return;
                }
                else {
                    throw std::invalid_argument("Illegal move, castle square is attacked");
                }
            }
            else {
                throw std::invalid_argument("Illegal move, can't castle with pieces in the way");
            }
        }
        else {
            throw std::invalid_argument("Illegal move, the player does not have the right to castle");
        }
    }
    else if (!side_turn && move == "o-o-o") {
        throw std::invalid_argument("Illegal move");
    }
}

// Easy Move Notation EMN
// basically normal algebraic notation but indicating origin square and piece to make it easier to parse
// also doesnt distinguish between takes and just move
// expected notation example: Bf4-d6
// short castle O-O long castle O-O-O for white, for black same but small
void Board::move(std::string move, bool side_turn) {

    // check if move is legal
    // if it is just move the piece to the position BUT
    // if en-passant possible then not possible anymore
    // if pawn forward 2 then en-passant is possible
    try {
        if (move == "O-O" || move == "O-O-O" || move == "o-o" || move == "o-o-o") {
            Board::castle(move, side_turn);
        }
        else {
            int origin_square_index = coordinates_to_board_index(move.substr(1, 2));
            int destination_square_index = coordinates_to_board_index(move.substr(4, 5));

            Piece* piece = Board::board[origin_square_index];
            if (piece == NULL) {
                throw std::invalid_argument("Nothing at origin coordinates");
            }

            std::string str = piece->get_appearance();
            char piece_type = std::toupper(char(str[0]));

            if (piece_type != char(move[0])) {
                throw std::invalid_argument("Origin coordinates do not correspond with given piece");
            }
            else {
                if (piece_at(origin_square_index)->side != side_turn) {
                    throw std::invalid_argument("Piece isn't on the player's side");
                }
                else {
                    if (is_move_legal(piece_at(origin_square_index), destination_square_index)) {
                        Board::move_piece_to(destination_square_index, piece);

                        // updating castling rights --------
                        // if king moves, remove all castling rights
                        if (piece_type == 'K' && side_turn && (Board::white_king_side_castle || Board::white_queen_side_castle)) {
                            Board::white_king_side_castle = false;
                            Board::white_queen_side_castle = false;
                        }
                        else if (piece_type == 'K' && !side_turn && (Board::black_king_side_castle || Board::black_queen_side_castle)) {
                            Board::black_king_side_castle = false;
                            Board::black_queen_side_castle = false;
                        }
                        // if rook moves, remove corresponding castling right
                        if (piece_type == 'R' && side_turn && Board::white_king_side_castle && origin_square_index == 63) {
                            Board::white_king_side_castle = false;
                        }
                        else if (piece_type == 'R' && side_turn && Board::white_queen_side_castle && origin_square_index == 56) {
                            Board::white_queen_side_castle = false;
                        }
                        else if (piece_type == 'R' && !side_turn && Board::black_king_side_castle && origin_square_index == 7) {
                            Board::black_king_side_castle = false;
                        }
                        else if (piece_type == 'R' && !side_turn && Board::black_queen_side_castle && origin_square_index == 0) {
                            Board::black_queen_side_castle = false;
                        }
                        // if rook is taken, remove corresponding castling right
                        Piece* taken_piece = Board::board[destination_square_index];
                        if (taken_piece) {
                            std::string taken_str = taken_piece->get_appearance();
                            char taken_piece_type = std::toupper(char(taken_str[0]));

                            if (taken_piece_type == 'R' && side_turn && Board::black_king_side_castle && destination_square_index == 7) {
                                Board::black_king_side_castle = false;
                            }
                            else if (taken_piece_type == 'R' && side_turn && Board::black_queen_side_castle && destination_square_index == 0) {
                                Board::black_queen_side_castle = false;
                            }
                            else if (taken_piece_type == 'R' && !side_turn && Board::white_king_side_castle && destination_square_index == 63) {
                                Board::white_king_side_castle = false;
                            }
                            else if (taken_piece_type == 'R' && !side_turn && Board::white_queen_side_castle && destination_square_index == 56) {
                                Board::white_queen_side_castle = false;
                            }
                        }
                    }
                    else {
                        throw std::invalid_argument("Illegal move");
                    }
                }
            }
        }
    }
    catch (const std::invalid_argument& e) {
        throw e;
    }
}
