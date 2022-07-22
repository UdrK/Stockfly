#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <windows.h>

#include "src/board.h"
#include "src/piece.h"
#include "src/utils.h"

#include "src/king.h"
#include "src/queen.h"
#include "src/rook.h"
#include "src/bishop.h"
#include "src/knight.h"
#include "src/pawn.h"


using namespace std;

string separator = "--------------------------------------------------";

bool compare_moves(vector<int> computer_moves, vector<int> legal_moves) {
    
    if (computer_moves.empty() && legal_moves.empty()) {
        return true;
    }

    if (computer_moves.size() != legal_moves.size()) {
        return false;
    }

    for (int i = 0; i < legal_moves.size(); i++) {
        if (count(computer_moves.begin(), computer_moves.end(), legal_moves[i]) != 1)
            return false;
    }

    return true;
}

void fen_test(bool print) {
    string fens[] = {
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR",
        "rn1qk1nr/1ppb1pbp/p3p3/3p2p1/P2P4/3BPN2/1PP2PPP/RNBQ1RK1",
        "3r4/pb4kp/5p2/1pp5/4P3/1BP2P2/PP1r1P1P/2R1R1K1",
        "r1bqk1nr/pp3pb1/2n4p/6p1/2BP1p2/2P2N2/PP4PP/RNBQK2R",
        "r3kbnr/p1pp1ppp/1pb5/6B1/8/2N2N2/PPP1BPPP/R2R2K1",
        "5bnr/3Qkp1p/p2pp3/1B4p1/7q/2B1P3/P1P2PPP/RN2K1NR",
        "r4r2/4qpkR/b3pNp1/pp1pP1P1/n1pP4/2P2N2/PP1QKPP1/R7",
        "8/1k5q/1p1Q3p/6p1/5B2/2P2PP1/PP3P1P/4R1K1",
        "rn2kbnr/ppp3p1/3p3p/1q6/6bN/2N3P1/PPP1PPBP/R1BQ1RK1",
        "r2q1rk1/p2nppbp/3P2p1/1bpQ4/1pN5/1B1P4/PPPBNPPP/R4RK1",
        "r1bqk2r/p4ppp/np1bpn2/2ppN3/3P4/2PBP1B1/PP3PPP/RN1QK2R",
        "1r6/pP6/5p2/3p4/kQpP4/2P1P3/P2N1PPP/1R4K1",
        "r1bqkb1r/p3pppp/1pn2n2/3p4/2pP1B2/2P1P3/PPBN1PPP/R2QK1NR",
        "r3rbk1/p1p1q3/Pp3p1p/1Pn1pPpP/2Np1n2/B4P2/R1PP4/3K1QNR",
        "rnbq1rk1/ppp2pbp/5np1/4p3/3pP1P1/PP3P2/RBPPQ2P/1N2KBNR",
        "r1bqk1nr/2p2ppp/p1n1p3/1p6/1b1P1B2/2NBPN2/PP3PPP/R2QK2R",
        "r3kb1r/pppqpppp/2n5/3p1b2/3PnB1N/3BP3/PPPN1PPP/R2QK2R",
        "6k1/1p4pp/p3p1r1/3p4/P1pP4/4q3/5RPP/6QK",
        "2q2K2/1p2r3/4kp2/p7/P2b4/8/5r2/8",
        "3r3r/P1p1n1pp/4bp2/2k5/1n1pP3/2P2N2/5PPP/RQ4K1",
        "r2qkbnr/ppp3pp/2n1bp2/3p4/5B2/3BP3/PPP2PPP/RN1QK1NR",
        "8/8/6k1/8/8/5K2/8/8",
        "8/6Q1/8/6k1/6r1/5K2/8/8",
        "8/8/8/8/8/2Q5/k1K5/8",
    };

    int correct_counter = 0;
    int fens_length = 0;

    Board* b = new Board(string(""));
    for (auto f : fens) {
        fens_length++;
        b->set_from_fen(f, true);
        string calculated_fen = b->get_fen();
        string res = "X";

        if (f == calculated_fen) {
            res = "✓";
            correct_counter++;
        }
        else {
            res = "X";
        }
        if (print) {
            cout << f << " " << calculated_fen << endl;
            cout << res << endl;
            printf(b->print_board(false));
        }
    }

    cout << "Fen <-> Board conversions test" << endl;
    cout << correct_counter << "/" << fens_length << endl;
    cout << separator << endl;
}

void verbose_fen_test() {
    string fens[] = {
        "r3kb1r/pp3p1p/3pbn2/6p1/4P3/1PNK4/PBP2PPP/R5NR w kq 6",
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq 8",
        "rnb1k2r/pppqppbp/3p1np1/8/4PP2/3P1N2/PPP1B1PP/RNBQ1RK1 b kq 8",
        "1rbqk1nr/pp3pb1/2n4p/6p1/2BP1pP1/2P2N2/PP5P/RNBQK2R w KQk 6",
        "r3kbnr/p1pp1ppp/1pb5/6B1/8/2N2N2/PPP1BPPP/R2R2K1 w kq 8",
    };

    int piece_counts[] = {
        25,
        32,
        32,
        28,
        26
    };

    int correct_counter = 0;
    int fens_length = 0;

    Board* b = new Board(string(""));
    for (auto f : fens) {
        b->set_from_fen(f, true);
        string calculated_fen = b->get_fen(true);

        if (f == calculated_fen && b->get_pieces(true).size() + b->get_pieces(false).size() == piece_counts[fens_length]) {
            correct_counter++;
        }
        fens_length++;
    }

    cout << "Fen <-> Board conversions test (verbose)" << endl;
    cout << correct_counter << "/" << fens_length << endl;
    cout << separator << endl;
}

void board_index_to_coordinates_test() {
    std::vector<int> indexes = { 0, 63, 35, 28, 16, 4, 7, 43 };
    std::vector<string> expected_coordinates = { "a8", "h1", "d4", "e5", "a6", "e8", "h8", "d3"};

    int correct_counter = 0;
    for (int i = 0; i < indexes.size(); i++) {
        if (board_index_to_coordinates(indexes[i]) == expected_coordinates[i]) {
            correct_counter++;
        }
        else {
            cout << "Board Index To Coordinates Test: " << i + 1 << " failed. C vs E.c.:" << board_index_to_coordinates(indexes[i]) << " vs " << expected_coordinates[i] << endl;
        }
    }

    cout << "Board Index To Coordinates test" << endl;
    cout << correct_counter << "/" << indexes.size() << endl;
    cout << separator << endl;

}

void empty_board_king_moves_test(Board* board) {

    vector<int> starting_positions = { 35, 0, 7, 63, 56, 32, 4, 39, 59, 14, 49 };
    const int l = starting_positions.size();

    vector<vector<int>> computer_moves;

    for (int p : starting_positions) {
        computer_moves.push_back((new King(true, p))->pseudo_legal_moves(board));
    }

    vector<int> ground_truth_1 = { 26, 27, 28, 34, 36, 42, 43, 44 };
    vector<int> ground_truth_2 = { 1, 8, 9 };
    vector<int> ground_truth_3 = { 6, 14, 15 };
    vector<int> ground_truth_4 = { 54, 55, 62 };
    vector<int> ground_truth_5 = { 48, 49, 57 };
    vector<int> ground_truth_6 = { 24, 25, 33, 40, 41 };
    vector<int> ground_truth_7 = { 3, 5, 11, 12, 13 };
    vector<int> ground_truth_8 = { 30, 31, 38, 46, 47 };
    vector<int> ground_truth_9 = { 50, 51, 52, 58, 60 };
    vector<int> ground_truth_10 = { 5, 6, 7, 13, 15, 21, 22, 23 };
    vector<int> ground_truth_11 = { 40, 41, 42, 48, 50, 56, 57, 58 };

    vector<int> ground_truths[] = { ground_truth_1, ground_truth_2, ground_truth_3, ground_truth_4,
        ground_truth_5, ground_truth_6, ground_truth_7, ground_truth_8, ground_truth_9, ground_truth_10, ground_truth_11 };

    int correct_counter = 0;
    for (int i = 0; i < l; i++) {
        if (compare_moves(computer_moves[i], ground_truths[i])) {
            correct_counter++;
        }
        else {
            cout << "King Test: " << i + 1 << " failed. Starting Position: " << starting_positions[i] << endl;
        }
    }

    cout << "Empty board, King moves test" << endl;
    cout << correct_counter << "/" << l << endl;
    cout << separator << endl;
}

void empty_board_queen_moves_test(Board* board) {

    vector<int> starting_positions = { 35, 0, 7, 63, 56, 32, 4, 39, 59, 14, 49 };
    const int l = starting_positions.size();

    vector<vector<int>> computer_moves;

    for (int p : starting_positions) {
        computer_moves.push_back((new Queen(true, p))->pseudo_legal_moves(board));
    }

    vector<int> ground_truth_1 = { 3, 7, 8, 11, 14, 17, 19, 21, 26, 27, 28, 32, 33, 34, 36, 37, 38, 39, 42, 43, 44, 49, 51, 53, 56, 59, 62 };
    vector<int> ground_truth_2 = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 16, 18, 24, 27, 32, 36, 40, 45, 48, 54, 56, 63 };
    vector<int> ground_truth_3= { 0, 1, 2, 3, 4, 5, 6, 14, 15, 21, 23, 28, 31, 35, 39, 42, 47, 49, 55, 56, 63 };
    vector<int> ground_truth_4 = { 0, 7, 9, 15, 18, 23, 27, 31, 36, 39, 45, 47, 54, 55, 56, 57, 58, 59, 60, 61, 62 };
    vector<int> ground_truth_5 = { 0, 7, 8, 14, 16, 21, 24, 28, 32, 35, 40, 42, 48, 49, 57, 58, 59, 60, 61, 62, 63 };
    vector<int> ground_truth_6 = { 0, 4, 8, 11, 16, 18, 24, 25, 33, 34, 35, 36, 37, 38, 39, 40, 41, 48, 50, 56, 59 };
    vector<int> ground_truth_7 = { 0, 1, 2, 3, 5, 6, 7, 11, 12, 13, 18, 20, 22, 25, 28, 31, 32, 36, 44, 52, 60 };
    vector<int> ground_truth_8 = { 3, 7, 12, 15, 21, 23, 30, 31, 32, 33, 34, 35, 36, 37, 38, 46, 47, 53, 55, 60, 63 };
    vector<int> ground_truth_9 = { 3, 11, 19, 27, 31, 32, 35, 38, 41, 43, 45, 50, 51, 52, 56, 57, 58, 60, 61, 62, 63 };
    vector<int> ground_truth_10 = { 5, 6, 7, 8, 9, 10, 11, 12, 13, 15, 21, 22, 23, 28, 30, 35, 38, 42, 46, 49, 54, 56, 62 };
    vector<int> ground_truth_11 = { 1, 7, 9, 14, 17, 21, 25, 28, 33, 35, 40, 41, 42, 48, 50, 51, 52, 53, 54, 55, 56, 57, 58 };

    vector<int> ground_truths[] = { ground_truth_1, ground_truth_2, ground_truth_3, ground_truth_4, 
        ground_truth_5, ground_truth_6, ground_truth_7, ground_truth_8, ground_truth_9, ground_truth_10, ground_truth_11 };

    int correct_counter = 0;
    for (int i = 0; i < l; i++) {
        if (compare_moves(computer_moves[i], ground_truths[i])) {
            correct_counter++;
        }
        else {
            cout << "Queen Test: " << i + 1 << " failed. Starting Position: " << starting_positions[i] << endl;
        }
    }

    cout << "Empty board, Queen moves test" << endl;
    cout << correct_counter << "/" << l << endl;
    cout << separator << endl;
}

void empty_board_rook_moves_test(Board* board) {

    vector<int> starting_positions = { 35, 0, 7, 63, 56, 32, 4, 39, 59, 14, 49 };
    const int l = starting_positions.size();

    vector<vector<int>> computer_moves;

    for (int p : starting_positions) {
        computer_moves.push_back((new Rook(true, p))->pseudo_legal_moves(board));
    }

    vector<int> ground_truth_1 = { 3, 11, 19, 27, 32, 33, 34, 36, 37, 38, 39, 43, 51, 59 };
    vector<int> ground_truth_2 = { 1, 2, 3, 4, 5, 6, 7, 8, 16, 24, 32, 40, 48, 56 };
    vector<int> ground_truth_3 = { 0, 1, 2, 3, 4, 5, 6, 15, 23, 31, 39, 47, 55, 63 };
    vector<int> ground_truth_4 = { 7, 15, 23, 31, 39, 47, 55, 56, 57, 58, 59, 60, 61, 62 };
    vector<int> ground_truth_5 = { 0, 8, 16, 24, 32, 40, 48, 57, 58, 59, 60, 61, 62, 63 };
    vector<int> ground_truth_6 = { 0, 8, 16, 24, 33, 34, 35, 36, 37, 38, 39, 40, 48, 56 };
    vector<int> ground_truth_7 = { 0, 1, 2, 3, 5, 6, 7, 12, 20, 28, 36, 44, 52, 60 };
    vector<int> ground_truth_8 = { 7, 15, 23, 31, 32, 33, 34, 35, 36, 37, 38, 47, 55, 63 };
    vector<int> ground_truth_9 = { 3, 11, 19, 27, 35, 43, 51, 56, 57, 58, 60, 61, 62, 63 };
    vector<int> ground_truth_10 = { 6, 8, 9, 10, 11, 12, 13, 15, 22, 30, 38, 46, 54, 62 };
    vector<int> ground_truth_11 = { 1, 9, 17, 25, 33, 41, 48, 50, 51, 52, 53, 54, 55, 57 };

    vector<int> ground_truths[] = { ground_truth_1, ground_truth_2, ground_truth_3, ground_truth_4, 
        ground_truth_5, ground_truth_6, ground_truth_7, ground_truth_8, ground_truth_9, ground_truth_10, ground_truth_11 };

    int correct_counter = 0;
    for (int i = 0; i < l; i++) {
        if (compare_moves(computer_moves[i], ground_truths[i])) {
            correct_counter++;
        }
        else {
            cout << "Rook Test: " << i + 1 << " failed. Starting Position: " << starting_positions[i] << endl;
        }
    }

    cout << "Empty board, Rook moves test" << endl;
    cout << correct_counter << "/" << l << endl;
    cout << separator << endl;
}

void empty_board_bishop_moves_test(Board* board) {

    vector<int> starting_positions = { 35, 0, 7, 63, 56, 32, 4, 39, 59, 14, 49 };
    const int l = starting_positions.size();

    vector<vector<int>> computer_moves;

    for (int p : starting_positions) {
        computer_moves.push_back((new Bishop(true, p))->pseudo_legal_moves(board));
    }

    vector<int> ground_truth_1 = { 7, 8, 14, 17, 21, 26, 28, 42, 44, 49, 53, 56, 62 };
    vector<int> ground_truth_2 = { 9, 18, 27, 36, 45, 54, 63 };
    vector<int> ground_truth_3 = { 14, 21, 28, 35, 42, 49, 56 };
    vector<int> ground_truth_4 = { 0, 9, 18, 27, 36, 45, 54 };
    vector<int> ground_truth_5 = { 7, 14, 21, 28, 35, 42, 49 };
    vector<int> ground_truth_6 = { 4, 11, 18, 25, 41, 50, 59 };
    vector<int> ground_truth_7 = { 11, 13, 18, 22, 25, 31, 32 };
    vector<int> ground_truth_8 = { 3, 12, 21, 30, 46, 53, 60 };
    vector<int> ground_truth_9 = { 31, 32, 38, 41, 45, 50, 52 };
    vector<int> ground_truth_10 = { 5, 7, 21, 23, 28, 35, 42, 49, 56 };
    vector<int> ground_truth_11 = { 7, 14, 21, 28, 35, 40, 42, 56, 58 };

    vector<int> ground_truths[] = { ground_truth_1, ground_truth_2, ground_truth_3, ground_truth_4, 
        ground_truth_5, ground_truth_6, ground_truth_7, ground_truth_8, ground_truth_9, ground_truth_10, ground_truth_11 };

    int correct_counter = 0;
    for (int i = 0; i < l; i++) {
        if (compare_moves(computer_moves[i], ground_truths[i])) {
            correct_counter++;
        }
        else {
            cout << "Bishop Test: " << i + 1 << " failed. Starting Position: " << starting_positions[i] << endl;
        }

    }

    cout << "Empty board, Bishop moves test" << endl;
    cout << correct_counter << "/" << l << endl;
    cout << separator << endl;
}

void empty_board_knight_moves_test(Board* board) {

    vector<int> starting_positions = { 35, 0, 7, 63, 56, 32, 4, 39, 59, 49, 9, 14, 54, 10, 13, 17, 22, 41, 46, 50, 53, 1, 6, 8, 15, 48, 55, 57, 62 };
    const int l = starting_positions.size();

    vector<vector<int>> computer_moves;

    for (int p : starting_positions) {
        computer_moves.push_back((new Knight(true, p))->pseudo_legal_moves(board));
    }

    vector<int> ground_truth_1 = { 18, 20, 25, 29, 41, 45, 50, 52 };
    vector<int> ground_truth_2 = { 10, 17 };
    vector<int> ground_truth_3 = { 13, 22 };
    vector<int> ground_truth_4 = { 46, 53 };
    vector<int> ground_truth_5 = { 41, 50 };
    vector<int> ground_truth_6 = { 17, 26, 42, 49 };
    vector<int> ground_truth_7 = { 10, 14, 19, 21 };
    vector<int> ground_truth_8 = { 22, 29, 45, 54 };
    vector<int> ground_truth_9 = { 42, 44, 49, 53 };
    vector<int> ground_truth_10 = { 32, 34, 43, 59 };
    vector<int> ground_truth_11 = { 3, 19, 24, 26 };
    vector<int> ground_truth_12 = { 4, 20, 29, 31 };
    vector<int> ground_truth_13 = { 37, 39, 44, 60 };
    vector<int> ground_truth_14 = { 0, 4, 16, 20, 25, 27 };
    vector<int> ground_truth_15 = { 3, 7, 19, 23, 28, 30 };
    vector<int> ground_truth_16 = { 0, 2, 11, 27, 32, 34 };
    vector<int> ground_truth_17 = { 5, 7, 12, 28, 37, 39 };
    vector<int> ground_truth_18 = { 24, 26, 35, 51, 56, 58 };
    vector<int> ground_truth_19 = { 29, 31, 36, 52, 61, 63 };
    vector<int> ground_truth_20 = { 33, 35, 40, 44, 56, 60 };
    vector<int> ground_truth_21 = { 36, 38, 43, 47, 59, 63 };
    vector<int> ground_truth_22 = { 11, 16, 18 };
    vector<int> ground_truth_23 = { 12, 21, 23 };
    vector<int> ground_truth_24 = { 2, 18, 25 };
    vector<int> ground_truth_25 = { 5, 21, 30 };
    vector<int> ground_truth_26 = { 33, 42, 58 };
    vector<int> ground_truth_27 = { 38, 45, 61 };
    vector<int> ground_truth_28 = { 40, 42, 51 };
    vector<int> ground_truth_29 = { 45, 47, 52 };

    vector<int> ground_truths[] = { ground_truth_1, ground_truth_2, ground_truth_3, ground_truth_4, ground_truth_5, 
        ground_truth_6, ground_truth_7, ground_truth_8, ground_truth_9, ground_truth_10, ground_truth_11, ground_truth_12, 
        ground_truth_13, ground_truth_14, ground_truth_15, ground_truth_16, ground_truth_17, ground_truth_18, ground_truth_19,
        ground_truth_20, ground_truth_21, ground_truth_22, ground_truth_23, ground_truth_24, ground_truth_25, ground_truth_26,
        ground_truth_27, ground_truth_28, ground_truth_29
    };

    int correct_counter = 0;
    for (int i = 0; i < l; i++) {
        if (compare_moves(computer_moves[i], ground_truths[i])) {
            correct_counter++;
        }
        else {
            cout << "Knight Test: " << i + 1 << " failed. Starting Position: " << starting_positions[i] << endl;
        }
    }

    cout << "Empty board, Knight moves test" << endl;
    cout << correct_counter << "/" << l << endl;
    cout << separator << endl;
}

void empty_board_pawn_moves_test(Board* board) {

    vector<int> starting_positions = { 49, 11, 36, 53, 9, 10, 50, 37 };
    vector<bool> sides = {true, true, true, true, false, false, false, false};
    const int l = starting_positions.size();

    vector<vector<int>> computer_moves;

    for (int i = 0; i < l; i++) {
        computer_moves.push_back((new Pawn(sides[i], starting_positions[i]))->pseudo_legal_moves(board));
    }

    vector<int> ground_truth_1 = { 41, 33 };
    vector<int> ground_truth_2 = { 3 };
    vector<int> ground_truth_3 = { 28 };
    vector<int> ground_truth_4 = { 45, 37 };
    vector<int> ground_truth_5 = { 17, 25 };
    vector<int> ground_truth_6 = { 18, 26 };
    vector<int> ground_truth_7 = { 58 };
    vector<int> ground_truth_8 = { 45 };

    vector<int> ground_truths[] = { ground_truth_1, ground_truth_2, ground_truth_3, ground_truth_4, ground_truth_5,
        ground_truth_6, ground_truth_7, ground_truth_8
    };

    int correct_counter = 0;
    for (int i = 0; i < l; i++) {
        if (compare_moves(computer_moves[i], ground_truths[i])) {
            correct_counter++;
        }
        else {
            cout << "Pawn Test: " << i + 1 << " failed. Starting Position: " << starting_positions[i] << endl;
        }
    }

    cout << "Empty board, Pawn moves test" << endl;
    cout << correct_counter << "/" << l << endl;
    cout << separator << endl;
}

void pop_board_king_moves_test(Board* board) { 
    
    vector<int> starting_positions = { 3, 13, 25, 26, 29, 31, 39, 41, 51, 56, 61, 7, 8, 18, 23, 26, 28, 36, 41, 46, 61 };

    vector<int> sides = { true, true, true, true, true, true, true, true, true, true, true, false, false, false, false, false,
    false, false, false, false, false
    };

    const int l = starting_positions.size();

    vector<vector<int>> computer_moves;

    for (int i = 0; i < l; i++) {
        computer_moves.push_back((new King(sides[i], starting_positions[i]))->pseudo_legal_moves(board));
    }

    vector<int> ground_truth_1 = { 2, 4, 10, 11, 12 };
    vector<int> ground_truth_2 = { 4, 5, 6, 12, 14, 20, 21, 22 };
    vector<int> ground_truth_3 = { 16, 17, 18, 24, 26, 33, 34 };
    vector<int> ground_truth_4 = { 17, 18, 19, 25, 27, 33, 34 };
    vector<int> ground_truth_5 = { 20, 21, 22, 28, 30, 36, 37, 38 };
    vector<int> ground_truth_6 = { 22, 23, 30, 38, 39 };
    vector<int> ground_truth_7 = { 30, 31, 38, 46, 47 };
    vector<int> ground_truth_8 = { 33, 34, 40, 42, 48, 49, 50 };
    vector<int> ground_truth_9 = { 42, 43, 44, 50, 52, 58, 59, 60 };
    vector<int> ground_truth_10 = { 48, 49, 57 };
    vector<int> ground_truth_11 = { 52, 60 };

    vector<int> ground_truth_12 = {  };
    vector<int> ground_truth_13 = { 0, 1, 17 };
    vector<int> ground_truth_14 = { 10, 11, 17, 19, 25, 26 };
    vector<int> ground_truth_15 = { 30, 31 };
    vector<int> ground_truth_16 = { 17, 18, 19, 25, 33, 35 };
    vector<int> ground_truth_17 = { 19, 21, 29, 35, 36, 37 };
    vector<int> ground_truth_18 = { 28, 29, 35, 37, 43, 45 };
    vector<int> ground_truth_19 = { 32, 33, 40, 42, 48, 49, 50 };
    vector<int> ground_truth_20 = { 37, 38, 39, 45, 47, 53, 54, 55 };
    vector<int> ground_truth_21 = { 52, 53, 54, 60, 62 };

    vector<int> ground_truths[] = { ground_truth_1, ground_truth_2, ground_truth_3, ground_truth_4,
        ground_truth_5, ground_truth_6, ground_truth_7, ground_truth_8, ground_truth_9, ground_truth_10, ground_truth_11,
        ground_truth_12, ground_truth_13, ground_truth_14, ground_truth_15, ground_truth_16, ground_truth_17, ground_truth_18,
        ground_truth_19, ground_truth_20, ground_truth_21
    };

    int correct_counter = 0;
    for (int i = 0; i < l; i++) {
        if (compare_moves(computer_moves[i], ground_truths[i])) {
            correct_counter++;
        }
        else {
            cout << "King Test: " << i + 1 << " failed. Starting Position: " << starting_positions[i] << endl;
        }
    }

    cout << "Populated board, King moves test" << endl;
    cout << correct_counter << "/" << l << endl;
    cout << separator << endl;
}

void pop_board_queen_moves_test(Board* board) {

    vector<int> starting_positions = { 3, 13, 25, 26, 29, 31, 39, 41, 51, 56, 61, 7, 8, 18, 23, 26, 28, 36, 41, 46, 61 };

    vector<int> sides = { true, true, true, true, true, true, true, true, true, true, true, false, false, false, false, false,
    false, false, false, false, false
    };

    const int l = starting_positions.size();

    vector<vector<int>> computer_moves;

    for (int i = 0; i < l; i++) {
        computer_moves.push_back((new Queen(sides[i], starting_positions[i]))->pseudo_legal_moves(board));
    }

    vector<int> ground_truth_1 = { 0, 1, 2, 4, 5, 6, 10, 11, 12, 17, 19, 21, 24, 27, 30, 39 };
    vector<int> ground_truth_2 = { 4, 5, 6, 9, 10, 11, 12, 14, 20, 21, 22, 29, 37, 45 };
    vector<int> ground_truth_3 = { 4, 9, 11, 16, 17, 18, 24, 26, 27, 33, 34, 41, 49, 57 };
    vector<int> ground_truth_4 = { 2, 5, 8, 10, 12, 17, 18, 19, 24, 25, 27, 33, 34, 40 };
    vector<int> ground_truth_5 = { 5, 13, 20, 21, 22, 27, 28, 30, 31, 36, 37, 38, 43, 45, 47, 50, 57 };
    vector<int> ground_truth_6 = { 15, 22, 23, 27, 28, 29, 30, 38, 39, 45, 47, 52, 59 };
    vector<int> ground_truth_7 = { 3, 12, 15, 21, 23, 30, 31, 36, 37, 38, 46, 47 };
    vector<int> ground_truth_8 = { 9, 17, 25, 33, 34, 40, 42, 43, 44, 48, 49, 50, 57, 59 };
    vector<int> ground_truth_9 = { 24, 33, 42, 43, 44, 48, 49, 50, 52, 58, 59, 60 };
    vector<int> ground_truth_10 = { 40, 42, 48, 49, 57, 58, 59, 60, 61 };
    vector<int> ground_truth_11 = { 34, 43, 52, 56, 57, 58, 59, 60 };

    vector<int> ground_truth_12 = {  };
    vector<int> ground_truth_13 = { 0, 1, 17, 26, 35 };
    vector<int> ground_truth_14 = { 2, 4, 10, 11, 17, 19, 25, 26, 32 };
    vector<int> ground_truth_15 = { 30, 31, 37, 39, 47, 55 };
    vector<int> ground_truth_16 = { 2, 5, 8, 10, 12, 17, 18, 19, 24, 25, 33, 35, 40 };
    vector<int> ground_truth_17 = { 1, 10, 19, 21, 29, 30, 31, 35, 36, 37, 46, 55 };
    vector<int> ground_truth_18 = { 28, 29, 35, 37, 38, 39, 43, 45, 50, 54, 57 };
    vector<int> ground_truth_19 = { 17, 25, 32, 33, 40, 42, 43, 48, 49, 50, 57, 59 };
    vector<int> ground_truth_20 = { 1, 10, 19, 28, 30, 37, 38, 39, 45, 47, 53, 54, 55 };
    vector<int> ground_truth_21 = { 43, 52, 53, 54, 56, 57, 58, 59, 60, 62 };

    vector<int> ground_truths[] = { ground_truth_1, ground_truth_2, ground_truth_3, ground_truth_4,
        ground_truth_5, ground_truth_6, ground_truth_7, ground_truth_8, ground_truth_9, ground_truth_10, ground_truth_11,
        ground_truth_12, ground_truth_13, ground_truth_14, ground_truth_15, ground_truth_16, ground_truth_17, ground_truth_18,
        ground_truth_19, ground_truth_20, ground_truth_21
    };

    int correct_counter = 0;
    for (int i = 0; i < l; i++) {
        if (compare_moves(computer_moves[i], ground_truths[i])) {
            correct_counter++;
        }
        else {
            cout << "Queen Test: " << i + 1 << " failed. Starting Position: " << starting_positions[i] << endl;
        }
    }

    cout << "Populated board, Queen moves test" << endl;
    cout << correct_counter << "/" << l << endl;
    cout << separator << endl;
}

void pop_board_rook_moves_test(Board* board) {

    vector<int> starting_positions = { 3, 13, 25, 26, 29, 31, 39, 41, 51, 56, 61, 7, 8, 18, 23, 26, 28, 36, 41, 46, 61 };

    vector<int> sides = { true, true, true, true, true, true, true, true, true, true, true, false, false, false, false, false,
    false, false, false, false, false
    };

    const int l = starting_positions.size();

    vector<vector<int>> computer_moves;

    for (int i = 0; i < l; i++) {
        computer_moves.push_back((new Rook(sides[i], starting_positions[i]))->pseudo_legal_moves(board));
    }

    vector<int> ground_truth_1 = { 0, 1, 2, 4, 5, 6, 11, 19, 27 };
    vector<int> ground_truth_2 = { 5, 9, 10, 11, 12, 14, 21, 29, 37, 45 };
    vector<int> ground_truth_3 = { 9, 17, 24, 26, 27, 33, 41, 49, 57 };
    vector<int> ground_truth_4 = { 2, 10, 18, 24, 25, 27, 34 };
    vector<int> ground_truth_5 = { 5, 13, 21, 27, 28, 30, 31, 37, 45 };
    vector<int> ground_truth_6 = { 15, 23, 27, 28, 29, 30, 39, 47 };
    vector<int> ground_truth_7 = { 15, 23, 31, 36, 37, 38, 47 };
    vector<int> ground_truth_8 = { 9, 17, 25, 33, 40, 42, 43, 44, 49, 57 };
    vector<int> ground_truth_9 = { 43, 48, 49, 50, 52, 59 };
    vector<int> ground_truth_10 = { 40, 48, 57, 58, 59, 60, 61 };
    vector<int> ground_truth_11 = { 56, 57, 58, 59, 60 };

    vector<int> ground_truth_12 = {  };
    vector<int> ground_truth_13 = { 0 };
    vector<int> ground_truth_14 = { 2, 10, 17, 19, 26 };
    vector<int> ground_truth_15 = { 31, 39, 47, 55 };
    vector<int> ground_truth_16 = { 2, 10, 18, 24, 25 };
    vector<int> ground_truth_17 = { 29, 30, 31, 36 };
    vector<int> ground_truth_18 = { 28, 35, 37, 38, 39 };
    vector<int> ground_truth_19 = { 17, 25, 33, 40, 42, 43, 49, 57 };
    vector<int> ground_truth_20 = { 30, 38, 45, 47, 54 };
    vector<int> ground_truth_21 = { 53, 56, 57, 58, 59, 60, 62 };

    vector<int> ground_truths[] = { ground_truth_1, ground_truth_2, ground_truth_3, ground_truth_4,
        ground_truth_5, ground_truth_6, ground_truth_7, ground_truth_8, ground_truth_9, ground_truth_10, ground_truth_11,
        ground_truth_12, ground_truth_13, ground_truth_14, ground_truth_15, ground_truth_16, ground_truth_17, ground_truth_18,
        ground_truth_19, ground_truth_20, ground_truth_21
    };

    int correct_counter = 0;
    for (int i = 0; i < l; i++) {
        if (compare_moves(computer_moves[i], ground_truths[i])) {
            correct_counter++;
        }
        else {
            cout << "Rook Test: " << i + 1 << " failed. Starting Position: " << starting_positions[i] << endl;
        }
    }

    cout << "Populated board, Rook moves test" << endl;
    cout << correct_counter << "/" << l << endl;
    cout << separator << endl;
}

void pop_board_bishop_moves_test(Board* board) {

    vector<int> starting_positions = { 3, 13, 25, 26, 29, 31, 39, 41, 51, 56, 61, 7, 8, 18, 23, 26, 28, 36, 41, 46, 61 };

    vector<int> sides = { true, true, true, true, true, true, true, true, true, true, true, false, false, false, false, false,
    false, false, false, false, false
    };

    const int l = starting_positions.size();

    vector<vector<int>> computer_moves;

    for (int i = 0; i < l; i++) {
        computer_moves.push_back((new Bishop(sides[i], starting_positions[i]))->pseudo_legal_moves(board));
    }

    vector<int> ground_truth_1 = { 10, 12, 17, 21, 24, 30, 39 };
    vector<int> ground_truth_2 = { 4, 6, 20, 22 };
    vector<int> ground_truth_3 = { 4, 11, 16, 18, 34 };
    vector<int> ground_truth_4 = { 5, 8, 12, 17, 19, 33, 40 };
    vector<int> ground_truth_5 = { 20, 22, 36, 38, 43, 47, 50, 57 };
    vector<int> ground_truth_6 = { 22, 38, 45, 52, 59 };
    vector<int> ground_truth_7 = { 3, 12, 21, 30, 46 };
    vector<int> ground_truth_8 = { 34, 48, 50, 59 };
    vector<int> ground_truth_9 = { 24, 33, 42, 44, 58, 60 };
    vector<int> ground_truth_10 = { 42, 49 };
    vector<int> ground_truth_11 = { 34, 43, 52 };

    vector<int> ground_truth_12 = {  };
    vector<int> ground_truth_13 = { 1, 17, 26, 35 };
    vector<int> ground_truth_14 = { 4, 11, 25, 32 };
    vector<int> ground_truth_15 = { 30, 37 };
    vector<int> ground_truth_16 = { 5, 8, 12, 17, 19, 33, 35, 40 };
    vector<int> ground_truth_17 = { 1, 10, 19, 21, 35, 37, 46, 55 };
    vector<int> ground_truth_18 = { 29, 43, 45, 50, 54, 57 };
    vector<int> ground_truth_19 = { 32, 48, 50, 59 };
    vector<int> ground_truth_20 = { 1, 10, 19, 28, 37, 39, 53, 55 };
    vector<int> ground_truth_21 = { 43, 52, 54 };

    vector<int> ground_truths[] = { ground_truth_1, ground_truth_2, ground_truth_3, ground_truth_4,
        ground_truth_5, ground_truth_6, ground_truth_7, ground_truth_8, ground_truth_9, ground_truth_10, ground_truth_11,
        ground_truth_12, ground_truth_13, ground_truth_14, ground_truth_15, ground_truth_16, ground_truth_17, ground_truth_18,
        ground_truth_19, ground_truth_20, ground_truth_21
    };

    int correct_counter = 0;
    for (int i = 0; i < l; i++) {
        if (compare_moves(computer_moves[i], ground_truths[i])) {
            correct_counter++;
        }
        else {
            cout << "Bishop Test: " << i + 1 << " failed. Starting Position: " << starting_positions[i] << endl;
        }
    }

    cout << "Populated board, Bishop moves test" << endl;
    cout << correct_counter << "/" << l << endl;
    cout << separator << endl;
}

void pop_board_knight_moves_test(Board* board) {

    vector<int> starting_positions = { 17, 28, 29, 17, 28, 29 };

    vector<int> sides = { true, true, true, false, false, false };

    const int l = starting_positions.size();

    vector<vector<int>> computer_moves;

    for (int i = 0; i < l; i++) {
        computer_moves.push_back((new Knight(sides[i], starting_positions[i]))->pseudo_legal_moves(board));
    }

    vector<int> ground_truth_1 = { 0, 11, 27, 34 };
    vector<int> ground_truth_2 = { 11, 13, 18, 22, 34, 38, 43, 45 };
    vector<int> ground_truth_3 = { };
    vector<int> ground_truth_4 = { 2, 27, 32 };
    vector<int> ground_truth_5 = { };
    vector<int> ground_truth_6 = { 12, 14, 19, 23, 35, 39, 44, 46 };

    vector<int> ground_truths[] = { ground_truth_1, ground_truth_2, ground_truth_3, ground_truth_4,
        ground_truth_5, ground_truth_6 };

    int correct_counter = 0;
    for (int i = 0; i < l; i++) {
        if (compare_moves(computer_moves[i], ground_truths[i])) {
            correct_counter++;
        }
        else {
            cout << "Knight Test: " << i + 1 << " failed. Starting Position: " << starting_positions[i] << endl;
        }
    }

    cout << "Populated board, Knight moves test" << endl;
    cout << correct_counter << "/" << l << endl;
    cout << separator << endl;
}

void pop_board_pawn_moves_test(Board* board, bool side) { 

    string side_str = side ? "White" : "Black";
    vector<int> w_starting_positions = { 9, 28, 30, 48, 49, 50, 51, 55 };
    vector<int> b_starting_positions = { 8, 12, 13, 14, 15, 33, 35, 54 };
    vector<int> starting_positions = side ? w_starting_positions : b_starting_positions;

    const int l = starting_positions.size();

    vector<vector<int>> computer_moves;

    for (int i = 0; i < l; i++) {
        computer_moves.push_back((new Pawn(side, starting_positions[i]))->pseudo_legal_moves(board));
    }

    vector<int> w_ground_truth_1 = { 0, 1 };
    vector<int> w_ground_truth_2 = { 21 };
    vector<int> w_ground_truth_3 = { 21, 22 };
    vector<int> w_ground_truth_4 = { };
    vector<int> w_ground_truth_5 = { 33, 40, 41, 42 };
    vector<int> w_ground_truth_6 = {  };
    vector<int> w_ground_truth_7 = { 42 };
    vector<int> w_ground_truth_8 = { 46 };

    vector<int> b_ground_truth_1 = { 17 };
    vector<int> b_ground_truth_2 = { 21 };
    vector<int> b_ground_truth_3 = { };
    vector<int> b_ground_truth_4 = { 21, 22, 23, 30 };
    vector<int> b_ground_truth_5 = { };
    vector<int> b_ground_truth_6 = { 41, 42 };
    vector<int> b_ground_truth_7 = { 42 };
    vector<int> b_ground_truth_8= { 62, 63 };

    vector<int> ground_truth_1 = side ? w_ground_truth_1 : b_ground_truth_1;
    vector<int> ground_truth_2 = side ? w_ground_truth_2 : b_ground_truth_2;
    vector<int> ground_truth_3 = side ? w_ground_truth_3 : b_ground_truth_3;
    vector<int> ground_truth_4 = side ? w_ground_truth_4 : b_ground_truth_4;
    vector<int> ground_truth_5 = side ? w_ground_truth_5 : b_ground_truth_5;
    vector<int> ground_truth_6 = side ? w_ground_truth_6 : b_ground_truth_6;
    vector<int> ground_truth_7 = side ? w_ground_truth_7 : b_ground_truth_7;
    vector<int> ground_truth_8 = side ? w_ground_truth_8 : b_ground_truth_8;

    vector<int> ground_truths[] = { ground_truth_1, ground_truth_2, ground_truth_3, ground_truth_4,
        ground_truth_5, ground_truth_6, ground_truth_7, ground_truth_8 };

    int correct_counter = 0;
    for (int i = 0; i < l; i++) {
        if (compare_moves(computer_moves[i], ground_truths[i])) {
            correct_counter++;
        }
        else {
            cout << side_str << " Pawn Test: " << i + 1 << " failed. Starting Position: " << starting_positions[i] << endl;
        }
    }

    cout << "Populated board, " << side_str << " pawn moves test" << endl;
    cout << correct_counter << "/" << l << endl;
    cout << separator << endl;
}

void move_piece_test() {

    std::vector<std::string> fens_before = {
        "8/6k1/8/8/8/4Q3/2K5/8",
        "b1B5/3pPpP1/2pP2pP/8/P1pP2pP/3pPpP1/8/K6k",
        "k7/p3pppp/NP2bB1N/8/1pPp4/3P4/6p1/3K3R",
        "r1bqk2r/pppp3p/2nb1p1n/4p3/3P3B/2P2N2/PP2PPP1/RN1QKB1R",
        "r1bqk2r/p1pp3p/2nb1p1n/1p2p3/3P3B/2P2N2/PP2PPP1/RN1QKB1R",
        "r1bqk2r/p1pp3p/2nb1p1n/1p2p3/3P3B/2P2N2/PP1NPPP1/R2QKB1R",
        "1rbqk2r/p1pp3p/2nb1p1n/1p2p3/3P3B/2P2N2/PP1NPPP1/R2QKB1R",
        "1rbqk2r/2pp3p/p1nb1p1n/4p3/PpPP3B/5N1R/1P1NPPP1/R2QKB2",
        "rnbqkbnr/p1pp1ppp/1p6/8/3PpP2/N7/PPP1P1PP/1RBQKBNR",
        "rnbqkbnr/1p1ppppp/p7/2pP4/8/8/PPP1PPPP/RNBQKBNR",
        "rnbqkbnr/pp1ppppp/8/2p5/3P4/8/PPP1PPPP/RNBQKBNR",
        "rnbqkbnr/pp1pppp1/2p4p/3P4/8/8/PPP1PPPP/RNBQKBNR",
    };

    std::vector<std::pair<int, int>> moves = {
        std::make_pair(coordinates_to_board_index("e3"), coordinates_to_board_index("c5")),
        std::make_pair(coordinates_to_board_index("c8"), coordinates_to_board_index("d7")),
        std::make_pair(coordinates_to_board_index("b4"), coordinates_to_board_index("c3")),
        std::make_pair(coordinates_to_board_index("b7"), coordinates_to_board_index("b5")),
        std::make_pair(coordinates_to_board_index("b1"), coordinates_to_board_index("d2")),
        std::make_pair(coordinates_to_board_index("a8"), coordinates_to_board_index("b8")),
        std::make_pair(coordinates_to_board_index("h1"), coordinates_to_board_index("h3")),
        std::make_pair(coordinates_to_board_index("b4"), coordinates_to_board_index("a3")),
        std::make_pair(coordinates_to_board_index("e4"), coordinates_to_board_index("f3")),
        std::make_pair(coordinates_to_board_index("d5"), coordinates_to_board_index("c6")),
        std::make_pair(coordinates_to_board_index("d4"), coordinates_to_board_index("c5")),
        std::make_pair(coordinates_to_board_index("d5"), coordinates_to_board_index("c6")),
    };

    std::vector<std::string> fens_after = {
        "8/6k1/8/2Q5/8/8/2K5/8",
        "b7/3BPpP1/2pP2pP/8/P1pP2pP/3pPpP1/8/K6k",
        "k7/p3pppp/NP2bB1N/8/3p4/2pP4/6p1/3K3R",
        "r1bqk2r/p1pp3p/2nb1p1n/1p2p3/3P3B/2P2N2/PP2PPP1/RN1QKB1R",
        "r1bqk2r/p1pp3p/2nb1p1n/1p2p3/3P3B/2P2N2/PP1NPPP1/R2QKB1R",
        "1rbqk2r/p1pp3p/2nb1p1n/1p2p3/3P3B/2P2N2/PP1NPPP1/R2QKB1R",
        "1rbqk2r/p1pp3p/2nb1p1n/1p2p3/3P3B/2P2N1R/PP1NPPP1/R2QKB2",
        "1rbqk2r/2pp3p/p1nb1p1n/4p3/2PP3B/p4N1R/1P1NPPP1/R2QKB2",
        "rnbqkbnr/p1pp1ppp/1p6/8/3P4/N4p2/PPP1P1PP/1RBQKBNR",
        "rnbqkbnr/1p1ppppp/p1P5/8/8/8/PPP1PPPP/RNBQKBNR",
        "rnbqkbnr/pp1ppppp/8/2P5/8/8/PPP1PPPP/RNBQKBNR",
        "rnbqkbnr/pp1pppp1/2P4p/8/8/8/PPP1PPPP/RNBQKBNR",
    };
    
    std::vector<int> pieces_count_after = {
        3,
        20,
        17,
        30,
        30,
        30,
        30,
        29,
        31,
        31,
        31,
        31,
    };

    int correct_counter = 0;

    for (int i = 0; i < fens_before.size(); i++) {
        Board* b = new Board(fens_before[i]);

        if (fens_before[i] == "k7/p3pppp/NP2bB1N/8/1pPp4/3P4/6p1/3K3R") {
            b->set_en_passant_file(2);
        }
        if (fens_before[i] == "1rbqk2r/2pp3p/p1nb1p1n/4p3/PpPP3B/5N1R/1P1NPPP1/R2QKB2") {
            b->set_en_passant_file(0);
        }
        if (fens_before[i] == "rnbqkbnr/p1pp1ppp/1p6/8/3PpP2/N7/PPP1P1PP/1RBQKBNR") {
            b->set_en_passant_file(5);
        }
        if (fens_before[i] == "rnbqkbnr/1p1ppppp/p7/2pP4/8/8/PPP1PPPP/RNBQKBNR") {
            b->set_en_passant_file(2);
        }
        if (fens_before[i] == "rnbqkbnr/pp1ppppp/8/2p5/3P4/8/PPP1PPPP/RNBQKBNR") {
            b->set_en_passant_file(2);
        }

        b->move_piece_to(moves[i].second, b->piece_at(moves[i].first));
        std::string res = b->get_fen();
        int pieces_on_the_board = b->get_pieces(true).size() + b->get_pieces(false).size();
        if (res == fens_after[i] && pieces_on_the_board == pieces_count_after[i]) {
            correct_counter++;
        }
        else {
            cout << "Move Piece test " << i + 1 << " failed. " << endl;
        }

        delete b;
    }

    cout << "Move piece test" << endl;
    cout << correct_counter << "/" << fens_before.size() << endl;
    cout << separator << endl;
}

void is_attacked_test() {

    std::vector<std::string> fens = {
    "7k/8/8/8/3K4/4p3/8/8",      // this should be false
    "7k/8/8/8/3K1b2/8/8/8",      // this should be false
    "7k/8/5r2/8/3K4/8/8/8",      // this should be false
    "7k/8/1q6/8/3K4/8/8/8",
    "7k/3q4/8/8/3K4/8/8/8",
    "7k/8/2n5/8/3K4/8/8/8",
    "7k/8/5b2/8/3K4/8/8/8",
    "7k/8/8/8/3K4/8/8/3r4",
    "7k/8/8/8/3K4/8/5b2/8",
    "7k/8/8/4p3/3K4/8/8/8",
    "7k/8/8/2p5/3K4/8/8/8"
    };

    int correct_counter = 0;
    for (int i = 0; i < fens.size(); i++) {
        Board* b = new Board(fens[i]);
        if (i < 3) {
            if (!b->piece_at(coordinates_to_board_index("d4"))->is_attacked(b)) {
                correct_counter++;
            }
            else {
                cout << "Is Piece Attacked test " << i + 1 << " failed." << endl;
            }
        }
        else {
            if (b->piece_at(coordinates_to_board_index("d4"))->is_attacked(b)) {
                correct_counter++;
            }
            else {
                cout << "Is Piece Attacked test " << i + 1 << " failed." << endl;
            }
        }
    }

    cout << "Is Piece Attacked test" << endl;
    cout << correct_counter << "/" << fens.size() << endl;
    cout << separator << endl;
}

void is_move_legal_test() {
    std::vector<std::string> fens = {
        "7k/8/5r2/8/5N2/5K2/8/8",       // this should be false
        "7k/8/5r2/8/5N2/5K2/8/8",       // this should be false 
        "7k/6b1/4r3/4R3/3K4/8/8/8",     // this should be false 
        "7k/6b1/4r3/4R3/3K4/8/8/8",     // this should be false
        "5b1k/8/3r4/4R3/3K4/8/8/8",     // this should be false
        "5b1k/8/3r4/2p1R3/3K4/8/8/8",   // this should be false
        "5b1k/8/3r4/2p1R3/3K4/8/8/8",
        "5b1k/8/3r4/4R3/3K4/8/8/8",
        "8/3K4/1r6/4b2R/4k3/8/8/8",     
        "8/8/3r1k2/8/8/B2K4/8/8",
        "8/8/5kr1/8/8/B2K4/8/8",
        "8/8/5kr1/8/8/B2K4/8/8",
    };

    std::vector<std::pair<int, int>> moves = {
        std::make_pair(coordinates_to_board_index("f4"), coordinates_to_board_index("e3")),
        std::make_pair(coordinates_to_board_index("f4"), coordinates_to_board_index("e6")),
        std::make_pair(coordinates_to_board_index("e5"), coordinates_to_board_index("e6")),
        std::make_pair(coordinates_to_board_index("e5"), coordinates_to_board_index("f6")),
        std::make_pair(coordinates_to_board_index("e5"), coordinates_to_board_index("h5")),
        std::make_pair(coordinates_to_board_index("e5"), coordinates_to_board_index("d5")),
        std::make_pair(coordinates_to_board_index("d4"), coordinates_to_board_index("c5")),
        std::make_pair(coordinates_to_board_index("e5"), coordinates_to_board_index("d5")),
        std::make_pair(coordinates_to_board_index("h5"), coordinates_to_board_index("e5")),
        std::make_pair(coordinates_to_board_index("a3"), coordinates_to_board_index("d6")),
        std::make_pair(coordinates_to_board_index("a3"), coordinates_to_board_index("b4")),
        std::make_pair(coordinates_to_board_index("a3"), coordinates_to_board_index("e7")),
    };

    int correct_counter = 0;
    for (int i = 0; i < fens.size(); i++) {
        Board* b = new Board(fens[i]);
        if (i < 6) {
            if (!b->is_move_legal(b->piece_at(moves[i].first), moves[i].second)) {
                correct_counter++;
            }
            else {
                cout << "Is Move Legal test " << i + 1 << " failed." << endl;
            }
        }
        else {
            if (b->is_move_legal(b->piece_at(moves[i].first), moves[i].second)) {
                correct_counter++;
            }
            else {
                cout << "Is Move Legal test " << i + 1 << " failed." << endl;
            }
        }
    }

    cout << "Is Move Legal test" << endl;
    cout << correct_counter << "/" << fens.size() << endl;
    cout << separator << endl;

}

void castle_test() {
    std::vector<std::string> starting_fens = {
        "r2qkbnr/pppb2pp/2n1p3/1B1p1p2/3P1B2/4PN2/PPP2PPP/RN1QK2R",
        "r3kbnr/pppbqppp/2n1p3/3p4/3P1B2/2PBPN2/PP3PPP/RN1QK2R",
        "rn1qkbnr/pp2pp1p/2p3p1/3p1b2/3P1B2/2N5/PPPQPPPP/R3KBNR",
        "rn1qk2r/ppp2ppp/3bpn2/3p1b2/3P4/2PBP1B1/PP3PPP/RN1QK1NR",
        "r2qkbnr/pp1npppp/2p5/5b2/2pP1B2/2N5/PP1QPPPP/R3KBNR",
        "3rkbnr/pp2pppp/8/4Nb2/4nB2/8/PP2PPPP/R3KBNR",
        "2r1kbnr/pp2pppp/8/5b2/4nB2/3N4/PP2PPPP/R3KBNR",
        "r1b1kb1r/p3q1pp/1pn2n2/8/3P4/5N2/PPP2PPP/RN1QK2R",
        "r3kb1r/pb3qpp/1p6/3nP3/3P4/2N5/PPP3PP/R2QK2R",
        "r3kbr1/p6p/1p6/3bP3/3P4/2P2p2/PP1R3P/4K2R",
        "r3kbnr/pp1n1ppp/2p5/3q1b2/3P1B2/5N2/PPPNB1PP/R2QK2R",
        "r3kbnr/pp4pp/1np2p2/3q2N1/3P1BB1/8/PPPN2PP/R2Q1RK1",
        "r3kbnr/pp4pp/1np5/3q2B1/3P2B1/8/PPPN2PP/R2Q1RK1",
        "r3k2r/1p2b1pp/2p4n/p5B1/3PR1B1/5Q2/PnP3PP/4R1K1",
        "2r1k2r/1p2b2p/2p4p/p7/3PR3/6Q1/PnP3PP/4R1K1",
        "rn1qkbnr/pppb1ppp/3pp3/8/3P1B2/4PN2/PPP2PPP/RN1QKB1R",
        "rn1qkbnr/pppb1ppp/3pp3/8/3P1B2/4PN2/PPP2PPP/RN1QKB1R",
        "rn1qkbnr/pppb1ppp/3pp3/8/3P1B2/3BP3/PPP2PPP/RN1QK1NR",
        "rn1qkbnr/pppb1ppp/3pp3/8/3P1B2/2N1P3/PPP2PPP/R2QKBNR",
        "rn1qkbnr/pppb1ppp/3pp3/8/3P1B2/3QP3/PPP2PPP/RN2KBNR",
        "r2qkbnr/ppp1pppp/2n5/3p1b2/3P1B2/4P3/PPP2PPP/RN1QKBNR",
        "rn2kbnr/ppp1pppp/3q4/3p1b2/3P1B2/4P3/PPP2PPP/RN1QKBNR",
        "r1bqkbnr/ppp1pppp/2n5/3p4/3P1B2/8/PPP1PPPP/RN1QKBNR",
        "r1b1kbnr/pppqpppp/2n5/3p4/3P1B2/4P3/PPP2PPP/RN1QKBNR",
        "rnbqk1nr/ppp2ppp/3bp3/3p4/3P1B2/3BP3/PPP2PPP/RN1QK1NR",
        "rnbqkb1r/ppp2ppp/4pn2/3p4/3P1B2/2P1P3/PP3PPP/RN1QKBNR",
    };

    std::vector<std::string> moves = {
        "O-O",
        "o-o-o",
        "O-O-O",
        "o-o",
        "O-O-O",
        "O-O-O",
        "O-O-O",
        "O-O",
        "O-O",
        "O-O",
        "o-o-o",
        "o-o-o",
        "o-o-o",
        "o-o",
        "o-o",
        "O-O",
        "O-O-O",
        "O-O",
        "O-O-O",
        "O-O-O",
        "o-o-o",
        "o-o-o",
        "o-o-o",
        "o-o-o",
        "o-o",
        "o-o",
    };

    std::vector<bool> sides = {
        true,
        false,
        true,
        false,
        true,
        true,
        true,
        true,
        true,
        true,
        false,
        false,
        false,
        false,
        false,
        true,
        true,
        true,
        true,
        true,
        false,
        false,
        false,
        false,
        false,
        false,
    };

    std::vector<std::string> fens_after = {
        "r2qkbnr/pppb2pp/2n1p3/1B1p1p2/3P1B2/4PN2/PPP2PPP/RN1Q1RK1",
        "2kr1bnr/pppbqppp/2n1p3/3p4/3P1B2/2PBPN2/PP3PPP/RN1QK2R",
        "rn1qkbnr/pp2pp1p/2p3p1/3p1b2/3P1B2/2N5/PPPQPPPP/2KR1BNR",
        "rn1q1rk1/ppp2ppp/3bpn2/3p1b2/3P4/2PBP1B1/PP3PPP/RN1QK1NR",
        "r2qkbnr/pp1npppp/2p5/5b2/2pP1B2/2N5/PP1QPPPP/2KR1BNR",
        "Illegal move, the player does not have the right to castle",
        "Illegal move, the player does not have the right to castle",
        "Illegal move, the player does not have the right to castle",
        "Illegal move, the player does not have the right to castle",
        "Illegal move, the player does not have the right to castle",
        "2kr1bnr/pp1n1ppp/2p5/3q1b2/3P1B2/5N2/PPPNB1PP/R2QK2R",
        "Illegal move, the player does not have the right to castle",
        "Illegal move, the player does not have the right to castle",
        "Illegal move, the player does not have the right to castle",
        "Illegal move, the player does not have the right to castle",
        "Illegal move, the player does not have the right to castle",
        "Illegal move, the player does not have the right to castle",
        "Illegal move, the player does not have the right to castle",
        "Illegal move, the player does not have the right to castle",
        "Illegal move, the player does not have the right to castle",
        "Illegal move, the player does not have the right to castle",
        "Illegal move, the player does not have the right to castle",
        "Illegal move, the player does not have the right to castle",
        "Illegal move, the player does not have the right to castle",
        "Illegal move, the player does not have the right to castle",
        "Illegal move, the player does not have the right to castle",
    };

    int correct_counter = 0;
    for (int i = 0; i < starting_fens.size(); i++) {
        Board* b = new Board(starting_fens[i]);
        b->set_castle_rights(true, true, true, true);
        std::string final_fen;
        try {
            b->move(moves[i], sides[i]);
            final_fen = b->get_fen();
        }
        catch (const std::invalid_argument& e) {
            final_fen = e.what();
        }

        if (final_fen == fens_after[i]) {
            correct_counter++;
        }
        else {
            cout << "Castle test " << i + 1 << " failed." << endl;
        }

        delete b;
    }

    cout << "Castle test" << endl;
    cout << correct_counter << "/" << starting_fens.size() << endl;
    cout << separator << endl;
}

void castle_rights_test() {

    int correct_counter = 0;
    int n_tests = 4;
    
    std::string fen1 = "r3k2r/8/8/8/8/8/8/R3K2R";

    Board* b = new Board(fen1);
    b->set_castle_rights(true, true, true, true);
    b->move("Rh1-h8", true);
    try {
        b->move("O-O", true);
    }
    catch (const std::invalid_argument& e) {
        std::string res = e.what();
        if (res == "Illegal move, the player does not have the right to castle") {
            correct_counter++;
        }
    }

    try {
        b->move("o-o", false);
    }
    catch (const std::invalid_argument& e) {
        std::string res = e.what();
        if (res == "Illegal move, the player does not have the right to castle") {
            correct_counter++;
        }
    }

    try {
        b->move("O-O-O", true);
    }
    catch (const std::invalid_argument& e) {
        correct_counter--;
    }

    try {
        b->move("Rh8-h7", true);
        b->move("Rd1-f1", true);
        b->move("o-o-o", false);
    }
    catch (const std::invalid_argument& e) {
        correct_counter--;
    }
    delete b;

    b = new Board(fen1);
    b->set_castle_rights(true, true, true, true);
    b->move("Ra1-a8", true);

    try {
        b->move("O-O-O", true);
    }
    catch (const std::invalid_argument& e) {
        std::string res = e.what();
        if (res == "Illegal move, the player does not have the right to castle") {
            correct_counter++;
        }
    }

    try {
        b->move("o-o-o", false);
    }
    catch (const std::invalid_argument& e) {
        std::string res = e.what();
        if (res == "Illegal move, the player does not have the right to castle") {
            correct_counter++;
        }
    }

    try {
        b->move("O-O", true);
    }
    catch (const std::invalid_argument& e) {
        correct_counter--;
    }

    try {
        b->move("Ra8-a7", true);
        b->move("Rf1-d1", true);
        b->move("o-o", false);
    }
    catch (const std::invalid_argument& e) {
        correct_counter--;
    }

    delete b;
    
    cout << "Castle rights test" << endl;
    cout << correct_counter << "/" << n_tests << endl;
    cout << separator << endl;
}

void mate_test() {
    std::vector<std::string> fens = {
        "r1bqkbnr/p1pp1ppp/1pn5/4p3/2B1P3/5Q2/PPPP1PPP/RNB1K1NR",
        "7k/7r/2Np3Q/8/3PR3/2P5/1P2K1P1/1q6",
        "7k/7r/2Np3Q/8/3PR3/2P5/1P2K1P1/1q6",
        "4R3/5kr1/2Np3Q/8/3P4/2P5/1P2K1P1/1q6",
        "6r1/pp6/3p1k1K/1P1Pp3/P3P3/5pq1/N7/5B2",
        "6r1/pp6/3p1k1K/1P1Pp3/P3P3/5pq1/N7/5B2",
        "6r1/pp6/3p1k1K/1P1Pp3/P3P3/5pq1/N7/5B2",
        "rk5r/6p1/pBQ2n2/4p1p1/4P3/8/PP3PPP/6K1",
        "rn1qk2r/pbpp1pQp/1p3n2/4p3/4P3/bP6/P1PP1PPP/RN2KBNR",
    };

    std::vector<std::string> moves = {
        "Qf3-f7",
        "Re4-e8",
        "Qh6-f8",
        "Qh6-e6",
        "Rg8-h8",
        "Qg3-g6",
        "Qg3-h4",
        "Qc6-c7",
        "Qg7-h8",
    };

    std::vector<bool> sides = {
        true,
        true,
        true,
        true,
        false,
        false,
        false,
        true,
        true,
    };

    int correct_counter = 0;
    for (int i = 0; i < fens.size(); i++) {
        Board* board = new Board(fens[i]);
        board->set_castle_rights(true, true, true, true);
        try {
            board->move(moves[i], sides[i]);
            Piece* enemy_king = board->get_king(!sides[i]);
            if (enemy_king->is_attacked(board) && board->is_mate(sides[i])) {
                correct_counter++;
            }
            else if (fens[i] == "rn1qk2r/pbpp1pQp/1p3n2/4p3/4P3/bP6/P1PP1PPP/RN2KBNR") {
                correct_counter++;
            } else {
                cout << "Castle test " << i + 1 << " failed." << endl;
            }
        }
        catch (const std::invalid_argument& e) {
            cout << "Mate test " << i + 1 << " failed." << endl;
        }
        delete board;
    }

    cout << "Mate test" << endl;
    cout << correct_counter << "/" << fens.size() << endl;
    cout << separator << endl;
}

void insufficient_material_draw_test() {
    std::vector<std::string> fens = {
        "8/5k2/8/8/2P5/2K5/8/8",
        "8/5k2/8/8/1PN5/1K6/8/8",
        "8/5k2/8/8/2R5/2K5/8/8",
        "8/5k2/8/8/3Q4/2K5/8/8",
        "8/5k2/5r2/8/5R2/4PK2/8/8",
        "8/5k2/5R2/8/8/5K2/8/8",
        "8/5k2/4R3/8/6N1/5K2/8/8",
        "8/5k2/3n4/1R6/6N1/5K2/8/8",
    };

    std::vector<std::string> moves = {
        "Pc4-c5",   // not drawn
        "Pb4-b5",   // not drawn
        "Rc4-c5",   // not drawn
        "Qd4-c5",   // not drawn
        "Rf4-f6",   // not drawn
        "Kf7-f6",
        "Kf7-e6",
        "Nd6-b5"
    };

    std::vector<bool> sides = {
        true,
        true,
        true,
        true,
        true,
        false,
        false,
        false,
    };

    int correct_counter = 0;
    for (int i = 0; i < fens.size(); i++) {
        Board* board = new Board(fens[i]);
        board->set_castle_rights(false, false, false, false);
        try {
            board->move(moves[i], sides[i]);
            if (i < 5) {
                if (!board->is_draw_by_insufficient_material()) {
                    correct_counter++;
                }
                else {
                    cout << "Insufficient Material Draw test " << i + 1 << " failed." << endl;
                }
            }
            else {
                if (board->is_draw_by_insufficient_material()) {
                    correct_counter++;
                }
                else {
                    cout << "Insufficient Material Draw test " << i + 1 << " failed." << endl;
                }
            }
        }
        catch (const std::invalid_argument& e) {
            cout << "Insufficient Material Draw test " << i + 1 << " failed." << endl;
        }
        delete board;
    }

    cout << "Insufficient Material Draw test" << endl;
    cout << correct_counter << "/" << fens.size() << endl;
    cout << separator << endl;
}

void stalemate_test() {
    std::vector<std::string> fens = {
        "k7/8/8/1RK1N3/8/8/8/8",
        "k7/8/8/1RK1N3/8/p7/P7/8",
        "2k5/8/3K4/1Q6/8/8/8/8",
        "6bk/7p/7P/8/3R4/6K1/8/8",
        "6bk/7p/7P/8/3R4/6K1/8/8",
        "k7/2K5/8/1R2N3/8/8/8/8",
    };

    std::vector<std::string> moves = {
        "Ne5-c6",   // stalemate
        "Ne5-c6",   // stalemate
        "Qb5-b6",   // stalemate
        "Rd4-d8",   // stalemate
        "Rd4-g4",   
        "Rb5-a5",
    };

    std::vector<bool> sides = {
        true,
        true,
        true,
        true,
        true,
        true,
    };

    int correct_counter = 0;
    for (int i = 0; i < fens.size(); i++) {
        Board* board = new Board(fens[i]);
        board->set_castle_rights(false, false, false, false);
        try {
            board->move(moves[i], sides[i]);
            if (i < 4) {
                if (board->is_stalemate(sides[i])) {
                    correct_counter++;
                }
                else {
                    cout << "Stalemate test " << i + 1 << " failed." << endl;
                }
            }
            else {
                if (!board->is_stalemate(!sides[i])) {
                    correct_counter++;
                }
                else {
                    cout << "Stalemate test " << i + 1 << " failed." << endl;
                }
            }
        }
        catch (const std::invalid_argument& e) {
            cout << "Stalemate test " << i + 1 << " failed." << endl;
        }
        delete board;
    }

    cout << "Stalemate test" << endl;
    cout << correct_counter << "/" << fens.size() << endl;
    cout << separator << endl;
}

void repetition_draw_test() {
    std::string fen = "2r3k1/4ppp1/7p/8/8/3Q3P/5PP1/6K1 w  8";

    std::vector<std::string> moves = {
        "Qd3-e4",
        "Rc8-e8",
        "Qe4-d3",
        "Re8-c8",
        "Qd3-e4",
        "Rc8-e8",
        "Qe4-d3",
        "Re8-c8",
    };

    Board* board = new Board(fen);
    int i = 0;
    int correct_counter = 0;
    for (std::string move : moves) {
        board->move(move, (i % 2 == 0));
        i++;
        if (board->is_threefold_repetition()) {
            correct_counter++;
        }
    }

    cout << "Threefold Repetition Draw test" << endl;
    cout << correct_counter << "/" << 1 << endl;
    cout << separator << endl;
}

void promotion_test() {
    
    std::vector<std::string> fens = {
        "8/1P2k3/2K5/8/8/8/8/8",
        "8/1P2k3/2K5/8/8/8/8/8",
        "8/1P2k3/2K5/8/8/8/8/8",
        "8/1P2k3/2K5/8/8/8/8/8",
        "8/1P2k3/2K5/8/8/8/8/8",
        "8/8/8/4N3/8/2K5/5kp1/8",
        "8/8/8/4N3/8/2K5/5kp1/8",
        "8/8/8/4N3/8/2K5/5kp1/8",
        "8/8/8/4N3/8/2K5/5kp1/8",
        "8/8/8/4N3/8/2K5/5kp1/8",
        "8/1KP3rk/8/8/8/8/8/8",
    };

    std::vector<std::string> moves = {
        "Pb7-b8",
        "Pb7-b8-Q",
        "Pb7-b8-R",
        "Pb7-b8-B",
        "Pb7-b8-N",
        "Pg2-g1",
        "Pg2-g1-Q",
        "Pg2-g1-R",
        "Pg2-g1-B",
        "Pg2-g1-N",
        "Pc7-c8-Q"
    };

    std::vector<bool> sides = {
        true,
        true,
        true,
        true,
        true,
        false,
        false,
        false,
        false,
        false,
        true,
    };

    std::vector<std::string> fens_after = {
        "1Q6/4k3/2K5/8/8/8/8/8",
        "1Q6/4k3/2K5/8/8/8/8/8",
        "1R6/4k3/2K5/8/8/8/8/8",
        "1B6/4k3/2K5/8/8/8/8/8",
        "1N6/4k3/2K5/8/8/8/8/8",
        "8/8/8/4N3/8/2K5/5k2/6q1",
        "8/8/8/4N3/8/2K5/5k2/6q1",
        "8/8/8/4N3/8/2K5/5k2/6r1",
        "8/8/8/4N3/8/2K5/5k2/6b1",
        "8/8/8/4N3/8/2K5/5k2/6n1",
        "Illegal",
    };

    int correct_counter = 0;
    for (int i = 0; i < fens.size(); i++) {
        Board* board = new Board(fens[i]);
        board->set_castle_rights(false, false, false, false);
        try {
            board->move(moves[i], sides[i]);
            if (i < 10) {
                if (board->get_fen() == fens_after[i]) {
                    correct_counter++;
                }
                else {
                    cout << "Promotion test " << i + 1 << " failed." << endl;
                }
            }
        }
        catch (const std::invalid_argument& e) {
            if (i >= 10) {
                correct_counter++;
            }
            else {
                cout << "Promotion test " << i + 1 << " failed." << endl;
            }
            
        }
        delete board;
    }

    cout << "Promotion test" << endl;
    cout << correct_counter << "/" << fens.size() << endl;
    cout << separator << endl;
}

/*
int main() {
    SetConsoleOutputCP(65001);

    // board <-> fen tests
    fen_test(false);

    verbose_fen_test();

    // coordinates test
    board_index_to_coordinates_test();

    // empty board tests
    Board* empty_board = new Board("8/8/8/8/8/8/8/8 w KQkq - 0 1");
    empty_board_king_moves_test(empty_board);
    empty_board_queen_moves_test(empty_board);
    empty_board_rook_moves_test(empty_board);
    empty_board_bishop_moves_test(empty_board);
    empty_board_knight_moves_test(empty_board);
    empty_board_pawn_moves_test(empty_board);
    delete empty_board;

    // populated board tests
    Board* populated_board = new Board("6k1/1p4pp/p3p1r1/3p4/P1pP4/4q3/5RPP/6QK");
    pop_board_king_moves_test(populated_board);
    pop_board_queen_moves_test(populated_board);
    pop_board_rook_moves_test(populated_board);
    pop_board_bishop_moves_test(populated_board);

    delete populated_board;
    populated_board = new Board("b1B5/3pPpP1/2pP2pP/8/P1pP2pP/3pPpP1/8/K6k");
    pop_board_knight_moves_test(populated_board);

    delete populated_board;
    populated_board = new Board("r3k3/1P6/4p3/4PpP1/8/b1nB2pn/PPPP3P/7K");
    populated_board->set_en_passant_file(5);
    pop_board_pawn_moves_test(populated_board, true);

    delete populated_board;
    populated_board = new Board("k7/p3pppp/NP2bB1N/8/1pPp4/3P4/6p1/3K3R");
    populated_board->set_en_passant_file(2);
    pop_board_pawn_moves_test(populated_board, false);
    delete populated_board;

    move_piece_test();

    is_attacked_test();

    is_move_legal_test();

    castle_test();

    castle_rights_test();

    mate_test();

    insufficient_material_draw_test();

    stalemate_test();

    repetition_draw_test();

    promotion_test();

    return 0;
}
*/
