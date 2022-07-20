#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <windows.h>
#include <stdexcept>

#include "src/board.h"
#include "src/piece.h"
#include "src/utils.h"
#include "src/generic_ai.h"
#include "src/random_ai.h"

#include "src/king.h"
#include "src/queen.h"
#include "src/rook.h"
#include "src/bishop.h"
#include "src/knight.h"
#include "src/pawn.h"

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
				board->move(move, side_turn);
				is_input_move_legal = true;
				i += 1;

				if (board->is_threefold_repetition()) {
					draw = true;
					system("cls");
					std::cout << board->print_board(false);
				}
				else {
					Piece* enemy_king = board->get_king(!side_turn);
					if (enemy_king->is_attacked(board)) {
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

void ai_game_loop(Board* board, bool player_side, int turn) {

	int i = turn;
	bool mate = false;
	bool draw = false;
	bool side_turn = (i % 2 == 0);

	Generic_ai* ai = new Random_ai(!player_side);

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
				board->move(move, side_turn);
				is_input_move_legal = true;
				i += 1;

				if (board->is_threefold_repetition()) {
					draw = true;
					system("cls");
					std::cout << board->print_board(false);
				}
				else {
					Piece* enemy_king = board->get_king(!side_turn);
					if (enemy_king->is_attacked(board)) {
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

void against_random() {
	Board* board = new Board("");
	board->set_castle_rights(true, true, true, true);

	ai_game_loop(board, true, 0);

	while (true) {}
}

int main() {

	against_random();

	return 0;
}

