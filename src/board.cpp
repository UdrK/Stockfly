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
            Board::white_pieces.clear();
            Board::black_pieces.clear();
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
        char en_passant_file_char = Board::en_passant_file + 49;
        fen.push_back(' ');
        fen.push_back(en_passant_file_char);
    }
    
    return fen;
}

// removes side's piece in position from list of pieces (to be called when a piece is taken)
void Board::update_pieces_list(int position, bool side) {
    std::vector<Piece*>* pieces = side ? &(Board::white_pieces) : &(Board::black_pieces);
    Piece* to_be_removed;

    int i = 0;
    for (Piece* p : *pieces) {
        if (p->position == position) {
            to_be_removed = p;
            break;
        }
        i++;
    }
    pieces->erase(pieces->begin() + i);

    if (to_be_removed)
        delete to_be_removed;
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

void Board::get_side_turn(bool side_turn) {
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

bool Board::get_side_turn() {
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

// is square_index attacked by side player
bool Board::is_square_attacked(int square_index, bool side) {

    // side is used in the code to verify if !side attacks the square, but the method is called to verify is side attacks the square
    // maybe????
    side = !side;

    // if piace was a king, would it see an enemy king or queen? if yes piece is attacked
    std::vector<int> king_movement_squares = Piece::piece_movement(this, square_index, side, true, true, true);
    for (int square : king_movement_squares) {
        if (Board::piece_at(square)) {
            std::string piece_type = Board::piece_at(square)->get_appearance(true);
            if (Board::piece_at(square)->side != side && piece_type == "Q" || piece_type == "K")
                return true;
        }
    }

    // if piece was a queen/rook would it see an enemy queen/rook ? if yes piece is attacked
    std::vector<int> rook_movement_squares = Piece::piece_movement(this, square_index, side, false, false, true);
    for (int square : rook_movement_squares) {
        if (Board::piece_at(square)) {
            std::string piece_type = Board::piece_at(square)->get_appearance(true);
            if (Board::piece_at(square)->side != side && piece_type == "Q" || piece_type == "R")
                return true;
        }
    }

    // if piece was a queen/bishop would it see an enemy queen/bishop ? if yes piece is attacked
    std::vector<int> bishop_movement_squares = Piece::piece_movement(this, square_index, side, false, true, false);
    for (int square : bishop_movement_squares) {
        if (Board::piece_at(square)) {
            std::string piece_type = Board::piece_at(square)->get_appearance(true);
            if (Board::piece_at(square)->side != side && piece_type == "Q" || piece_type == "B")
                return true;
        }
    }

    // if piece was a knight would it see an enemy knight ? if yes piece is attacked
    std::vector<int> knight_movement_squares = Piece::knight_movement(this, square_index, side);
    for (int square : knight_movement_squares) {
        if (Board::piece_at(square)) {
            std::string piece_type = Board::piece_at(square)->get_appearance(true);
            if (Board::piece_at(square)->side != side && piece_type == "N")
                return true;
        }
    }

    // if piece was a pawn would it attack an enemy pawn ? if yes piece is attacked
    int side_multiplier = side ? -1 : 1;
    int take_square_one = square_index + side_multiplier * 7;
    int take_square_two = square_index + side_multiplier * 9;
    std::vector<int> pawn_attacked_squares = { take_square_one, take_square_two };
    for (int square : pawn_attacked_squares) {
        if (Board::piece_at(square) && square > 0 && square < 64) {
            std::string piece_type = Board::piece_at(square)->get_appearance(true);
            if (Board::piece_at(square)->side != side && piece_type == "P")
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

// side just gave a check, is it mate?
bool Board::is_mate(bool side) {

    Piece* king;
    std::vector<Piece*> pieces;
    if (side) {
        king = Board::black_king;
        pieces = Board::black_pieces;
    }
    else {
        king = Board::white_king;
        pieces = Board::white_pieces;
    }

    if (king->is_attacked(this)) {
        for (Piece* piece : pieces) {
            std::vector<int> moves = piece->pseudo_legal_moves(this);
            for (int move : moves) {
                if (is_move_legal(piece, move)) {
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
    Piece* king;
    std::vector<Piece*>* pieces;
    if (side) {
        king = Board::black_king;
        pieces = &(Board::black_pieces);
    }
    else {
        king = Board::white_king;
        pieces = &(Board::white_pieces);
    }

    // if enemy king not in check
    if (!king->is_attacked(this)) {
        // if legal moves, not stalemate, else stalemate
        for (Piece* piece : *pieces) {
            std::vector<int> moves = piece->pseudo_legal_moves(this);
            if (moves.size()>0) {
                for (int move : moves) {
                    if (is_move_legal(piece, move)) {
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

bool Board::can_castle(char castle_type, bool side_turn) {

    if (castle_type == 'K' && side_turn) {
        if (Board::white_king_side_castle) {
            // if no pieces in the way
            if (Board::board[61] == NULL && Board::board[62] == NULL) {
                // if squares aren't attacked (including king isn't in check)
                if (!Board::is_square_attacked(60, !side_turn) && !Board::is_square_attacked(61, !side_turn) && !Board::is_square_attacked(62, !side_turn)) {
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
                if (!Board::is_square_attacked(60, !side_turn) && !Board::is_square_attacked(59, !side_turn) && !Board::is_square_attacked(58, !side_turn)) {
                    return true;
                }
            }
        }
    }

    if (castle_type == 'K' && !side_turn) {
        if (Board::black_king_side_castle) {
            if (Board::board[5] == NULL && Board::board[6] == NULL) {
                if (!Board::is_square_attacked(4, !side_turn) && !Board::is_square_attacked(5, !side_turn) && !Board::is_square_attacked(6, !side_turn)) {
                    return true;
                }
            }
        }
    }

    if (castle_type == 'Q' && !side_turn) {
        if (Board::black_queen_side_castle) {
            if (Board::board[3] == NULL && Board::board[2] == NULL && Board::board[1] == NULL) {
                if (!Board::is_square_attacked(4, !side_turn) && !Board::is_square_attacked(3, !side_turn) && !Board::is_square_attacked(2, !side_turn)) {
                    return true;
                }
            }
        }
    } 
            
    return false;
}

// equivalent of taking the piece and moving it into position (removing other piece if there are any)
void Board::move_piece_to(int position, Piece* piece) {

    // if there is a piece where the piece moves to
    // remove piece (from board and piece list)
    // put new piece

    int original_piece_position = piece->position;
    std::string piece_type = piece->get_appearance(true);

    bool takes_a_piece = false;
    int taken_piece_position = -1;

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

    if (takes_a_piece) {

        // remove taken piece from list of pieces
        bool done = false;
        int i = 0;
        Piece* taken_piece = Board::board[taken_piece_position];
        for (Piece* p : Board::white_pieces) {
            if (p->position == taken_piece_position) {
                Board::white_pieces.erase(Board::white_pieces.begin() + i);
            }
            i++;
        }
        if (!done) {
            i = 0;
            for (Piece* p : Board::black_pieces) {
                if (p->position == taken_piece_position) {
                    Board::black_pieces.erase(Board::black_pieces.begin() + i);
                }
                i++;
            }
        }

        // remove taken piece from square
        Board::board[taken_piece_position] = NULL;
    }

    // remove piece from original position
    Board::board[original_piece_position] = NULL;

    // put piece in position
    Board::board[position] = piece;

    // piece has moved, updating pieces internal position
    piece->position = position;

    // after any move, en passant is no longer possible
    Board::en_passant_file = 8;

    // if move is pawn 2 forward then en passant is possible
    if (piece_type == "P" && abs(position - original_piece_position) > 15) {
        int file = original_piece_position % 8;
        Board::en_passant_file = file;
    }
}

void Board::castle(std::string move, bool side_turn) {

    if ((side_turn && (move == "O-O" || move == "O-O-O")) || (!side_turn && (move == "o-o" || move == "o-o-o"))) {

        // WHITE ==============
        if (side_turn) {
            if (move == "O-O" && can_castle('K', true)) {
                Board::move_piece_to(62, Board::piece_at(60));
                Board::move_piece_to(61, Board::piece_at(63));
                Board::white_king_side_castle = false;
                Board::white_queen_side_castle = false;
                return;
            }
            else if (side_turn && move == "O-O") {
                throw std::invalid_argument("Illegal move, the player does not have the right to castle");
            }

            if (move == "O-O-O" && can_castle('Q', true)) {
                Board::move_piece_to(58, Board::piece_at(60));
                Board::move_piece_to(59, Board::piece_at(56));
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
                Board::move_piece_to(6, Board::piece_at(4));
                Board::move_piece_to(5, Board::piece_at(7));
                Board::black_king_side_castle = false;
                Board::black_queen_side_castle = false;
                return;
            }
            else if (!side_turn && move == "o-o") {
                throw std::invalid_argument("Illegal move, the player does not have the right to castle");
            }

            // black queen side castles and has the right
            if (move == "o-o-o" && can_castle('Q', false)) {
                Board::move_piece_to(2, Board::piece_at(4));
                Board::move_piece_to(3, Board::piece_at(0));
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

void Board::promotion(std::string move, bool side) {
    
    int destination_square_index = coordinates_to_board_index(move.substr(4, 2));

    Piece* promote_to;
    if (move.size() == 8) {
        if (move.substr(7, 1) == "R") {
            promote_to = new Rook(side, destination_square_index);
        }
        else if (move.substr(7, 1) == "B") {
            promote_to = new Bishop(side, destination_square_index);
        }
        else if (move.substr(7, 1) == "N") {
            promote_to = new Knight(side, destination_square_index);
        }
        else {
            promote_to = new Queen(side, destination_square_index);
        }
    }
    else {
        promote_to = new Queen(side, destination_square_index);
    }

    // promotion is called after the pawn is put in its destination square
    // update piece list by removing pawn and inserting new piece, updating board subsequently
    std::vector<Piece*>* pieces = side ? &(Board::white_pieces) : &(Board::black_pieces);
    int i = 0;
    for (Piece* p : *pieces) {
        if (p->position == destination_square_index) {
            break;
        }
        i++;
    }
    // updating list: removing pawn, adding piece
    pieces->erase(pieces->begin() + i);
    pieces->push_back(promote_to);

    // deleting pawn
    delete Board::board[destination_square_index];
    // putting pience on the board
    Board::board[destination_square_index] = promote_to;
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
            int destination_square_index = coordinates_to_board_index(move.substr(4, 2));
            std::string taken_piece_type = Board::board[destination_square_index] ? Board::board[destination_square_index]->get_appearance(true) : "";
            Piece* piece = Board::board[origin_square_index];
            if (piece == NULL) {
                throw std::invalid_argument("Nothing at origin coordinates");
            }

            std::string piece_type = piece->get_appearance(true);

            if (piece_type != std::string(1, move[0])) {
                throw std::invalid_argument("Origin coordinates do not correspond with given piece");
            }
            else {
                if (piece_at(origin_square_index)->side != side_turn) {
                    throw std::invalid_argument("Piece isn't on the player's side");
                }
                else {
                    if (is_move_legal(piece_at(origin_square_index), destination_square_index)) {
                        // en passant is dealt with in here
                        Board::move_piece_to(destination_square_index, piece);

                        // if move is promotion, promote, update of pieces list is made in promotion
                        if (piece_type == "P" && ((side_turn && destination_square_index <= 7) || (!side_turn && destination_square_index >= 56))) {
                            Board::promotion(move, side_turn);
                        }

                        // updating castling rights --------
                        // if king moves, remove all castling rights
                        if (piece_type == "K" && side_turn && (Board::white_king_side_castle || Board::white_queen_side_castle)) {
                            Board::white_king_side_castle = false;
                            Board::white_queen_side_castle = false;
                        }
                        else if (piece_type == "K" && !side_turn && (Board::black_king_side_castle || Board::black_queen_side_castle)) {
                            Board::black_king_side_castle = false;
                            Board::black_queen_side_castle = false;
                        }
                        // if rook moves, remove corresponding castling right
                        if (piece_type == "R" && side_turn && Board::white_king_side_castle && origin_square_index == 63) {
                            Board::white_king_side_castle = false;
                        }
                        else if (piece_type == "R" && side_turn && Board::white_queen_side_castle && origin_square_index == 56) {
                            Board::white_queen_side_castle = false;
                        }
                        else if (piece_type == "R" && !side_turn && Board::black_king_side_castle && origin_square_index == 7) {
                            Board::black_king_side_castle = false;
                        }
                        else if (piece_type == "R" && !side_turn && Board::black_queen_side_castle && origin_square_index == 0) {
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
                    else {
                        throw std::invalid_argument("Illegal move");
                    }
                }
            }
        }
        // after a move has been made
        Board::side_turn = !side_turn;
        // this has to be done only when the move is actually made, not when the move is "tried" by the AI
        //positions_fens.push_back(Board::get_fen(true));
    }
    catch (const std::invalid_argument& e) {
        throw e;
    }
}
