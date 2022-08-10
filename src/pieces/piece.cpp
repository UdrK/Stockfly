#include <string>
#include <algorithm>
#include "piece.h"
#include "../board.h"
#include "../utils.h"

std::vector<int> Piece::piece_movement(Board* board, int position, bool side, bool king, bool diagonal, bool orthogonal) {

    std::vector<int> list;
    list.reserve(30);

    // position = (8 * rank) + file
    int file = position % 8;
    int rank = ((position - file) / 8);

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

    for (int i = 0; i < 7; i++) {   // 7 max possible "distance"
        // north west to east diagonal
        if (diagonal && nwe_n_diag && i < files_left && i < ranks_up) {
            int nort_west_east_diag = position - ((i + 1) * 9);
            // other sections of pieces trajectories work the same:
            // if empty square or enemy piece, add as possible move
            if ((board->piece_at(nort_west_east_diag) == NULL) ||
                (board->piece_at(nort_west_east_diag)->side != side)) {
                list.push_back(nort_west_east_diag);
            }
            // if i find a piece, no more adding possible moves from this section of diagonal
            if (board->piece_at(nort_west_east_diag) != NULL) {
                nwe_n_diag = false;
            }
        }

        // north file
        if (orthogonal && n_orth && i < ranks_up) {
            int north_file = position - ((i + 1) * 8);
            if ((board->piece_at(north_file) == NULL) ||
                (board->piece_at(north_file)->side != side)) {
                list.push_back(north_file);
            }
            if (board->piece_at(north_file) != NULL) {
                n_orth = false;
            }
        }

        // north east to west diagonal
        if (diagonal && new_n_diag && i < files_right && i < ranks_up) {
            int north_east_west_diag = position - ((i + 1) * 7);
            if ((board->piece_at(north_east_west_diag) == NULL) ||
                (board->piece_at(north_east_west_diag)->side != side)) {
                list.push_back(north_east_west_diag);
            }
            if (board->piece_at(north_east_west_diag) != NULL) {
                new_n_diag = false;
            }
        }

        // west rank
        if (orthogonal && west_orth && i < files_left) {
            int west_file = position - ((i + 1) * 1);
            if ((board->piece_at(west_file) == NULL) ||
                (board->piece_at(west_file)->side != side)) {
                list.push_back(west_file);
            }
            if (board->piece_at(west_file) != NULL) {
                west_orth = false;
            }
        }

        // east rank
        if (orthogonal && east_orth && i < files_right) {
            int east_file = position + ((i + 1) * 1);
            if ((board->piece_at(east_file) == NULL) ||
                (board->piece_at(east_file)->side != side)) {
                list.push_back(east_file);
            }
            if (board->piece_at(east_file) != NULL) {
                east_orth = false;
            }
        }

        // north west to east south section of the diagonal
        if (diagonal && nwe_s_diag && i < files_left && i < ranks_down) {
            int nort_west_east_diag = position + ((i + 1) * 7);
            if ((board->piece_at(nort_west_east_diag) == NULL) ||
                (board->piece_at(nort_west_east_diag)->side != side)) {
                list.push_back(nort_west_east_diag);
            }
            if (board->piece_at(nort_west_east_diag) != NULL) {
                nwe_s_diag = false;
            }
        }

        // south file
        if (orthogonal && s_orth && i < ranks_down) {
            int north_file = position + ((i + 1) * 8);
            if ((board->piece_at(north_file) == NULL) ||
                (board->piece_at(north_file)->side != side)) {
                list.push_back(north_file);
            }
            if (board->piece_at(north_file) != NULL) {
                s_orth = false;
            }
        }

        // north east to west south section of diagonal
        if (diagonal && new_s_diag && i < files_right && i < ranks_down) {
            int north_east_west_diag = position + ((i + 1) * 9);
            if ((board->piece_at(north_east_west_diag) == NULL) ||
                (board->piece_at(north_east_west_diag)->side != side)) {
                list.push_back(north_east_west_diag);
            }
            if (board->piece_at(north_east_west_diag) != NULL) {
                new_s_diag = false;
            }
        }
    }
     
    return list;
}

std::vector<int> Piece::knight_movement(Board* board, int position, bool side) {

    std::vector<int> list;
    list.reserve(10);

    // position = (8 * rank) + file
    int file = position % 8;
    int rank = ((position - file) / 8);

    int move = position - 17;

    // if at least 2 ranks north of the piece
    if (rank > 1) {
        // if enough room (at least 1 file) to the left
        if (file > 0) {
            if (board->piece_at(move) == NULL || board->piece_at(move)->side != side)
                list.push_back(move);
        }
        // same but right
        if (file < 7) {
            move = position - 15;   
            if (board->piece_at(move) == NULL || board->piece_at(move)->side != side)
                list.push_back(move);
        }
    }

    // if at least 1 rank north of the piece
    if (rank > 0) {
        if (file > 1) {
            move = position - 10;
            if (board->piece_at(move) == NULL || board->piece_at(move)->side != side)
                list.push_back(move);
        }
        if (file < 6) {
            move = position - 6;
            if (board->piece_at(move) == NULL || board->piece_at(move)->side != side)
                list.push_back(move);
        }
    }

    //  same but south
    if (rank < 7) {
        if (file > 1) {
            move = position + 6;
            if (board->piece_at(move) == NULL || board->piece_at(move)->side != side)
                list.push_back(move);
        }
        if (file < 6) {
            move = position + 10;
            if (board->piece_at(move) == NULL || board->piece_at(move)->side != side)
                list.push_back(move);
        }
    }

    if (rank < 6) {
        if (file > 0) {
            move = position + 15;
            if (board->piece_at(move) == NULL || board->piece_at(move)->side != side)
                list.push_back(move);
        }
        if (file < 7) {
            move = position + 17;
            if (board->piece_at(move) == NULL || board->piece_at(move)->side != side)
                list.push_back(move);
        }
    }

    return list;
}

std::vector<int> Piece::pawn_movement(Board* board, int position, bool side) {

    std::vector<int> list;
    list.reserve(10);

    // pawns move in different directions based on player color
    // this allows generalization
    int side_multiplier = side ? -1 : 1;

    // position = (8 * rank) + file
    int file = position % 8;
    int rank = ((position - file) / 8);

    int one_square_move = position + side_multiplier * 8;
    // check for pieces in the way
    if (board->piece_at(one_square_move) == NULL) {
        list.push_back(one_square_move);

        // if starting position, pawn can move two squares
        if ((side && rank == 6) || (!side && rank == 1)) {
            int two_squares_move = position + side_multiplier * 16;
            // given there aren't pieces in the way
            if (board->piece_at(two_squares_move) == NULL) {
                list.push_back(two_squares_move);
            }
        }
    }    

    // Normal takes

    int take_square_one = position + side_multiplier * 7;
    int take_square_two = position + side_multiplier * 9;

    if (board->piece_at(take_square_one) != NULL && board->piece_at(take_square_one)->side != side) {
        // position +- 7 leads to "teleport" when white borders the board to the right and black to the left
        // so i only check this position when i'm white and not to the right, and when i'm black and not the left
        if((side && file != 7) || (!side && file != 0))
            list.push_back(take_square_one);
    }

    if (board->piece_at(take_square_two) != NULL && board->piece_at(take_square_two)->side != side) {
        // position +- 9 leads to "teleport" when white borders the board to the left, and black to the right
        // so i only check this position when i'm white and not to the left and when i'm black and not to the right
        if ((side && file != 0) || (!side && file != 7))
            list.push_back(take_square_two);
    }

    // En Passant

    int en_passant_file = board->get_en_passant_file(); // this will be equal to the pawn's file only for the move after it played 2 squares forward
    int file_distance = file - en_passant_file;

    // if opponent played pawn forward 2 squares
    if (en_passant_file != 8) {
        // if my pawn is next to the file of the opponents pawn AND
        // EITHER i'm white and i'm on the 5th rank OR i'm black and i'm on the 4th rank
        if (abs(file_distance) == 1 && ((rank == 3 && side_multiplier == -1) || (rank == 4 && side_multiplier == 1))) {
            // then take in the right direction and calculate resulting pawn position according to which side i am
            list.push_back(position + side_multiplier * (8 - (side_multiplier * file_distance)));
        }
    }

    return list;
}

std::vector<int> Piece::is_attacked(Board* board) {
    return board->is_square_attacked_by(Piece::position, !Piece::side);
}

int Piece::get_type() {
    return Piece::piece_type;
}

std::string Piece::get_appearance() {
    return Piece::appearance;
}
