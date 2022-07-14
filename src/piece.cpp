#include <string>
#include <algorithm>
#include "piece.h"
#include "board.h"
#include "utils.h"

std::vector<int> Piece::piece_movement(Board* board, bool king, bool diagonal, bool orthogonal) {

    std::vector<int> list;

    // position = (8 * rank) + file
    int file = Piece::position % 8;
    int rank = ((Piece::position - file) / 8);

    // how many ranks up/down files left/right ?
    int ranks_up = rank;
    int ranks_down = (7 - rank);
    int files_left = file;
    int files_right = (7 - file);

    // king can only move 1 square away
    if (king) {
        ranks_up = ranks_up ? 1 : 0;
        ranks_down = ranks_down ? 1 : 0;
        files_left = files_left ? 1 : 0;
        files_right = files_right ? 1 : 0;
    }

    bool nwe_n_diag = true;     // north-west to east - nort diagonal
    bool n_orth = true;         // north orthogonal
    bool new_n_diag = true;     // north-east to west - north diagonal
    bool west_orth = true;      // west orthogonal
    bool east_orth = true;      // east orthogonal
    bool nwe_s_diag = true;     // nort-west to east - south diagonal
    bool s_orth = true;         // south orthogonal
    bool new_s_diag = true;     // nort-east to west - south diagonal

    // first we calculate sections of "north-bound" pieces trajectories
    for (int i = 0; i < ranks_up; i++) {

        // first the diagonal from the top left towards the piece
        if (i < files_left && diagonal && nwe_n_diag) {
            int nort_west_east_diag = Piece::position - ((i + 1) * 9);
            // other sections of pieces trajectories work the same:
            // if empty square or enemy piece, add as possible move
            if ((board->piece_at(nort_west_east_diag) == NULL) || 
                (board->piece_at(nort_west_east_diag)->side != Piece::side)) {
                list.push_back(nort_west_east_diag);
            }
            // if i find a piece, no more adding possible moves from this section of diagonal
            if (board->piece_at(nort_west_east_diag) != NULL) {
                nwe_n_diag = false;
            }
        }

        // then the orthogonal "file" from up to the piece
        if (orthogonal && n_orth) {
            int north_file = Piece::position - ((i + 1) * 8);
            if ((board->piece_at(north_file) == NULL) ||
                (board->piece_at(north_file)->side != Piece::side)) {
                list.push_back(north_file);
            }
            if (board->piece_at(north_file) != NULL) {
                n_orth = false;
            }
        }

        // then the diagonal from the top right to the piece
        if (i < files_right && diagonal && new_n_diag) {
            int north_east_west_diag = Piece::position - ((i + 1) * 7);
            if ((board->piece_at(north_east_west_diag) == NULL) ||
                (board->piece_at(north_east_west_diag)->side != Piece::side)) {
                list.push_back(north_east_west_diag);
            }
            if (board->piece_at(north_east_west_diag) != NULL) {
                new_n_diag = false;
            }
            
        }
    }

    // orthogonal rank-like possible moves (left of piece)
    if (orthogonal) {
        for (int i = 0; i < files_left && west_orth; i++) {
            int west_file = Piece::position - ((i + 1) * 1);
            if ((board->piece_at(west_file) == NULL) ||
                (board->piece_at(west_file)->side != Piece::side)) {
                list.push_back(west_file);
            }
            if (board->piece_at(west_file) != NULL) {
                west_orth = false;
            }
        }
    }

    // (right of piece)
    if (orthogonal) {
        for (int i = 0; i < files_right && east_orth; i++) {
            int east_file = Piece::position + ((i + 1) * 1);
            if ((board->piece_at(east_file) == NULL) ||
                (board->piece_at(east_file)->side != Piece::side)) {
                list.push_back(east_file);
            }
            if (board->piece_at(east_file) != NULL) {
                east_orth = false;
            }
        }
    }

    // same as up, but down
    for (int i = 0; i < ranks_down; i++) {

        if (i < files_left && diagonal && nwe_s_diag) {
            int nort_west_east_diag = Piece::position + ((i + 1) * 7);
            if ((board->piece_at(nort_west_east_diag) == NULL) ||
                (board->piece_at(nort_west_east_diag)->side != Piece::side)) {
                list.push_back(nort_west_east_diag);
            }
            if (board->piece_at(nort_west_east_diag) != NULL) {
                nwe_s_diag = false;
            }
        }

        if (orthogonal && s_orth) {
            int north_file = Piece::position + ((i + 1) * 8);
            if ((board->piece_at(north_file) == NULL) ||
                (board->piece_at(north_file)->side != Piece::side)) {
                list.push_back(north_file);
            }
            if (board->piece_at(north_file) != NULL) {
                s_orth = false;
            }
        }

        if (i < files_right && diagonal && new_s_diag) {
            int north_east_west_diag = Piece::position + ((i + 1) * 9);
            if ((board->piece_at(north_east_west_diag) == NULL) ||
                (board->piece_at(north_east_west_diag)->side != Piece::side)) {
                list.push_back(north_east_west_diag);
            }
            if (board->piece_at(north_east_west_diag) != NULL) {
                new_s_diag = false;
            }
        }

    }

    return list;
}

std::vector<int> Piece::knight_movement(Board* board) {

    std::vector<int> list;

    // position = (8 * rank) + file
    int file = Piece::position % 8;
    int rank = ((Piece::position - file) / 8);

    int ranks_up = rank;
    int ranks_down = (7 - rank);
    int files_left = file;
    int files_right = (7 - file);

    std::vector<int> candidate_moves;

    // if at least 2 ranks north of the piece
    if (rank > 1) {

        // if enough room (at least 1 file) to the left
        if (file > 0) {
            candidate_moves.push_back(Piece::position - 17);
        }

        // same but right
        if (file < 7) {
            candidate_moves.push_back(Piece::position - 15);
        }
    }

    // if at least 1 rank north of the piece
    if (rank > 0) {
        if (file > 1) {
            candidate_moves.push_back(Piece::position - 10);
        }
        if (file < 6) {
            candidate_moves.push_back(Piece::position - 6);
        }
    }

    //  same but south
    if (rank < 7) {
        if (file > 1) {
            candidate_moves.push_back(Piece::position + 6);
        }
        if (file < 6) {
            candidate_moves.push_back(Piece::position + 10);
        }
    }

    if (rank < 6) {
        if (file > 0) {
            candidate_moves.push_back(Piece::position + 15);
        }
        if (file < 7) {
            candidate_moves.push_back(Piece::position + 17);
        }
    }

    // return only moves s.t. your knight doesn't take your pieces
    for (int move : candidate_moves) {
        if (board->piece_at(move) == NULL || board->piece_at(move)->side != Piece::side) {
            list.push_back(move);
        }
    }

    return list;
}

std::vector<int> Piece::pawn_movement(Board* board) {

    std::vector<int> list;

    // pawns move in different directions based on player color
    // this allows generalization
    int side_multiplier = Piece::side ? -1 : 1;

    // position = (8 * rank) + file
    int file = Piece::position % 8;
    int rank = ((Piece::position - file) / 8);

    int one_square_move = Piece::position + side_multiplier * 8;
    // check for pieces in the way
    if (board->piece_at(one_square_move) == NULL) {
        list.push_back(one_square_move);

        // if starting position, pawn can move two squares
        if ((Piece::side && rank == 6) || (!Piece::side && rank == 1)) {
            int two_squares_move = Piece::position + side_multiplier * 16;
            // given there aren't pieces in the way
            if (board->piece_at(two_squares_move) == NULL) {
                list.push_back(two_squares_move);
            }
        }
    }    

    // Normal takes

    int take_square_one = Piece::position + side_multiplier * 7;
    int take_square_two = Piece::position + side_multiplier * 9;

    if (board->piece_at(take_square_one) != NULL && board->piece_at(take_square_one)->side != Piece::side) {
        // position +- 7 leads to "teleport" when white borders the board to the right and black to the left
        // so i only check this position when i'm white and not to the right, and when i'm black and not the left
        if((Piece::side && file != 7) || (!Piece::side && file != 0))
            list.push_back(take_square_one);
    }

    if (board->piece_at(take_square_two) != NULL && board->piece_at(take_square_two)->side != Piece::side) {
        // position +- 9 leads to "teleport" when white borders the board to the left, and black to the right
        // so i only check this position when i'm white and not to the left and when i'm black and not to the right
        if ((Piece::side && file != 0) || (!Piece::side && file != 7))
            list.push_back(take_square_two);
    }

    // En Passant

    int en_passant_file = board->get_en_passant_file(); // this will be equal to the pawn's file only for the move after it played 2 squares forward
    int file_distance = file - en_passant_file;

    // if opponent played pawn forward 2 squares
    if (en_passant_file != -1) {
        // if my pawn is next to the file of the opponents pawn AND
        // EITHER i'm white and i'm on the 5th rank OR i'm black and i'm on the 4th rank
        if (abs(file_distance) == 1 && ((rank == 3 && side_multiplier == -1) || (rank == 4 && side_multiplier == 1))) {
            // then take in the right direction and calculate resulting pawn position according to which side i am
            list.push_back(Piece::position + side_multiplier * (8 - (side_multiplier * file_distance)));
        }
    }

    return list;
}

bool Piece::is_attacked(Board* board) {

    // if piece was a queen/rook would it see an enemy queen/rook ? if piece is attacked
    std::vector<int> rook_movement_squares = piece_movement(board, false, false, true);
    for (int square : rook_movement_squares) {
        if (board->piece_at(square)) {
            std::string str = board->piece_at(square)->get_appearance();
            char piece_type = std::toupper(char(str[0]));
            if (board->piece_at(square)->side != Piece::side && piece_type == 'Q' || piece_type == 'R')
                return true;
        }
    }

    // if piece was a queen/bishop would it see an enemy queen/bishop ? if yes piece is attacked
    std::vector<int> bishop_movement_squares = piece_movement(board, false, true, false);
    for (int square : bishop_movement_squares) {
        if (board->piece_at(square)) {
            std::string str = board->piece_at(square)->get_appearance();
            char piece_type = std::toupper(char(str[0]));
            if (board->piece_at(square)->side != Piece::side && piece_type == 'Q' || piece_type == 'B')
                return true;
        }
    }

    // if piece was a knight would it see an enemy knight ? if yes piece is attacked
    std::vector<int> knight_movement_squares = knight_movement(board);
    for (int square : knight_movement_squares) {
        if (board->piece_at(square)) {
            std::string str = board->piece_at(square)->get_appearance();
            char piece_type = std::toupper(char(str[0]));
            if (board->piece_at(square)->side != Piece::side && piece_type == 'N')
                return true;
        }
    }

    // if piece was a pawn would it attack an enemy pawn ? if yes piece is attacked
    int side_multiplier = Piece::side ? -1 : 1;
    int take_square_one = Piece::position + side_multiplier * 7;
    int take_square_two = Piece::position + side_multiplier * 9;
    std::vector<int> pawn_attacked_squares = { take_square_one, take_square_two };
    for (int square : pawn_attacked_squares) {
        if (board->piece_at(square)) {
            std::string str = board->piece_at(square)->get_appearance();
            char piece_type = std::toupper(char(str[0]));
            if (board->piece_at(square)->side != Piece::side && piece_type == 'P')
                return true;
        }
    }

    return false;
}

std::string Piece::get_appearance() {
    return Piece::appearance;
}
