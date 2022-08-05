#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <windows.h>
#include <stdexcept>

#include "src/board.h"
#include "src/pieces/piece.h"
#include "src/utils.h"
#include "src/ply.h"
#include "src/ai/ai.h"
#include "src/ai/random_ai.h"
#include "src/ai/stockfly.h"

#include "src/pieces/king.h"
#include "src/pieces/queen.h"
#include "src/pieces/rook.h"
#include "src/pieces/bishop.h"
#include "src/pieces/knight.h"
#include "src/pieces/pawn.h"

// 0 white's turn, 1 black's turn
void game_loop(Board* board, int turn) {
	
	int i = turn;
	bool mate = false;
	bool draw = false;
	bool side_turn = (i % 2 == 0);

	while (!mate && !draw) {

		bool is_input_move_legal = false;
		bool tried_illegal_move = false;
		side_turn = (i % 2 == 0);

		while (!is_input_move_legal) {
			system("cls");
			std::cout << board->print_board(false);
			std::string move = "";

			if (tried_illegal_move) {
				std::cout << "Illegal move, try again." << std::endl;
			}
			if (side_turn) {
				std::cout << "White's turn, make a move > " << std::endl;
			}
			else {
				std::cout << "Black's turn, make a move > " << std::endl;
			}
			std::cin >> move;

			try {
				Ply* p = new Ply(move, side_turn);
				board->move(p);
				delete p;
				is_input_move_legal = true;
				i += 1;

				if (board->is_threefold_repetition()) {
					draw = true;
					system("cls");
					std::cout << board->print_board(false);
				}
				else {
					Piece* enemy_king = board->get_king(!side_turn);
					if (enemy_king->is_attacked(board).size()!=0) {
						if (board->is_mate(side_turn)) {
							mate = true;
							system("cls");
							std::cout << board->print_board(false);
						}
					}
					else if (board->is_stalemate(side_turn)) {
						draw = true;
						system("cls");
						std::cout << board->print_board(false);
					}
					else if (board->is_draw_by_insufficient_material()) {
						draw = true;
						system("cls");
						std::cout << board->print_board(false);
					}
				}
			}
			catch (const std::invalid_argument& e) {
				std::cout << e.what() << std::endl;
				tried_illegal_move = true;
			}
		}
	}

	std::string winner = side_turn ? "White" : "Black";

	if (mate)
		std::cout << "Checkmate!" << std::endl << winner << " won!";
	else
		std::cout << "Draw!";
}

void ai_game_loop(Board* board, Ai* ai, bool player_side, int turn) {

	int i = turn;
	bool mate = false;
	bool draw = false;
	bool side_turn = (i % 2 == 0);

	while (!mate && !draw) {

		bool is_input_move_legal = false;
		bool tried_illegal_move = false;
		side_turn = (i % 2 == 0);

		while (!is_input_move_legal) {
			system("cls");
			std::cout << board->print_board(false);
			std::string move = "";

			if (tried_illegal_move) {
				std::cout << "Illegal move, try again." << std::endl;
			}
			if (side_turn) {
				std::cout << "White's turn, make a move > " << std::endl;
			}
			else {
				std::cout << "Black's turn, make a move > " << std::endl;
			}
			if (side_turn == player_side)
				std::cin >> move;
			else
				move = ai->move(board);

			try {
				Ply* p = new Ply(move, side_turn);
				board->move(p);
				delete p;
				is_input_move_legal = true;
				i += 1;

				if (board->is_threefold_repetition()) {
					draw = true;
					system("cls");
					std::cout << board->print_board(false);
				}
				else {
					Piece* enemy_king = board->get_king(!side_turn);
					if (enemy_king->is_attacked(board).size()!=0) {
						if (board->is_mate(side_turn)) {
							mate = true;
							system("cls");
							std::cout << board->print_board(false);
						}
					}
					else if (board->is_stalemate(side_turn)) {
						draw = true;
						system("cls");
						std::cout << board->print_board(false);
					}
					else if (board->is_draw_by_insufficient_material()) {
						draw = true;
						system("cls");
						std::cout << board->print_board(false);
					}
				}
			}
			catch (const std::invalid_argument& e) {
				std::cout << e.what() << std::endl;
				tried_illegal_move = true;
			}
		}
	}

	std::string winner = side_turn ? "White" : "Black";

	if (mate)
		std::cout << "Checkmate!" << std::endl << winner << " won!";
	else
		std::cout << "Draw!";
}

void solitaire() {
	Board* board = new Board("r1k4r/1p4p1/pBbR1n2/4p1p1/2Q1P3/8/PP3PPP/6K1");
	board->set_castle_rights(false, false, false, false);
	game_loop(board, 0);

	while (true) {}
}

int loop() {

	std::string debug_fen = "2r1kr1b/1b2p2p/4Npp1/p3N3/P1RP4/1P6/1B3PPP/2R3K1 w";

	Board* board = new Board("");

	ai_game_loop(board, new Stockfly(false, 3), true, 0);

	while (true) {}

	return 0;
}
