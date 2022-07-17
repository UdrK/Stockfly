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
	bool king_captured = false;
	while (!king_captured) {

		bool is_input_move_legal = false;
		bool tried_illegal_move = false;

		while (!is_input_move_legal) {
			system("cls");
			std::cout << board->print_board(false);
			std::string move = "";

			if (tried_illegal_move) {
				std::cout << "Illegal move, try again." << std::endl;
			}
			if ((i % 2 == 0)) {
				std::cout << "White's turn, make a move > " << std::endl;
			}
			else {
				std::cout << "Black's turn, make a move > " << std::endl;
			}
			std::cin >> move;

			try {
				board->move(move, (i % 2 == 0));
				is_input_move_legal = true;
				i += 1;

				int destination_square_index = coordinates_to_board_index(move.substr(3, 4));
				Piece* piece = board->piece_at(destination_square_index);

				std::string str = piece->get_appearance();
				char piece_type = std::toupper(char(str[0]));

				if (piece_type == 'K')
					king_captured = true;
			}
			catch (const std::invalid_argument& e) {
				std::cout << e.what() << std::endl;
				tried_illegal_move = true;
			}
		}
	}

	std::cout << std::endl << (i % 2 == 0) ? "White" : "Black" " won!";
}

/*
int main() {

	Board* board = new Board("");
	game_loop(board);

	return 0;
}
*/