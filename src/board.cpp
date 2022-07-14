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
    set_from_fen(fen_ip);
}

void Board::set_en_passant_file(int file) {
    Board::en_passant_file = file;
}

void Board::set_from_fen(std::string fen) {
    fen_to_board(fen);
}

void Board::set_king(bool side, King* king) {
    if (side) {
        Board::white_king = king;
    }
    else {
        Board::black_king = king;
    }
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

// Easy Move Notation EMN
// basically normal algebraic notation but indicating origin square and piece to make it easier to parse
// also doesnt distinguish between takes and just move
// expected notation example: Bf4-d6
// short castle O-O long castle O-O-O for white, for black same but small
void Board::move(std::string move) {

    // check if move is legal
    // if it is just move the piece to the position BUT
    // if en-passant possible then not possible anymore
    // if pawn forward 2 then en-passant is possible
    try {
        int origin_square_index = coordinates_to_board_index(move.substr(1, 2));
        int destination_square_index = coordinates_to_board_index(move.substr(3, 4));

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
            if (is_move_legal(piece_at(origin_square_index), destination_square_index)) {
                Board::move_piece_to(destination_square_index, piece);
            }
            else {
                throw std::invalid_argument("Illegal move");
            }
        }
    }
    catch (const std::invalid_argument& e) {
        throw e;
    }
}
