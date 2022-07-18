#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <windows.h>
#include <stdexcept>

#include "src/board.h"
#include "src/piece.h"
#include "src/utils.h"

#include "src/king.h"
#include "src/queen.h"
#include "src/rook.h"
#include "src/bishop.h"
#include "src/knight.h"
#include "src/pawn.h"

void game_loop(Board* board) {
	
	int i = 0;
	bool mate = false;
	bool side_turn = (i % 2 == 0);

	while (!mate) {

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

				Piece* enemy_king = board->get_king(!side_turn);
				if (enemy_king->is_attacked(board) && board->is_mate(side_turn)) {
					mate = true;
				}
			}
			catch (const std::invalid_argument& e) {
				std::cout << e.what() << std::endl;
				tried_illegal_move = true;
			}
		}
	}

	std::string winner = side_turn ? "White" : "Black";

	std::cout << std::endl << winner << " won!";
}

/*
int main() {

	Board* board = new Board("");
	game_loop(board);

	return 0;
}
*/