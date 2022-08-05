#include "random_ai.h"
#include "../utils.h"
#include <random>

int random_int_in_range(int lower, int upper) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distr(lower, upper-1);

	return distr(gen);
}

Random_ai::Random_ai(bool side) {
	Random_ai::side = side;
}

std::vector<std::string> Random_ai::generate_moves(Board*) {
	return std::vector<std::string>();
}

std::string Random_ai::move(Board* board) {
	
	std::vector<Piece*> pieces = board->get_pieces(Random_ai::side);

	int piece_to_move = 0;
	int actual_move = -1;
	std::string move_notation;
	std::vector<int> moves = {};

	// while i've not come up with a move
	while (actual_move == -1) {

		// while the pieces i randomly pick don't have pseudolegal moves, pick a random piece
		while (moves.size() == 0) {
			piece_to_move = random_int_in_range(0, pieces.size());
			moves = pieces.at(piece_to_move)->pseudo_legal_moves(board);
			// if piece has no moves, remove it from list
			if (moves.size() == 0) {
				pieces.erase(pieces.begin() + piece_to_move);
			}
		}

		// once i have pseudolegal moves, let's look for legal moves
		std::vector<int> legal_moves = {};
		for (int move : moves) {
			if (board->is_move_legal(pieces.at(piece_to_move), move, false)) {
				legal_moves.push_back(move);
			}
		}
		// pick a random legal move if there are any
		if (legal_moves.size() > 0) {
			int move_index = random_int_in_range(0, legal_moves.size());
			actual_move = legal_moves.at(move_index);
		} // delete piece from list otherwise
		else {
			pieces.erase(pieces.begin() + piece_to_move);
			moves = {};
			legal_moves = {};
		}
	}

	// move notation
	move_notation = pieces.at(piece_to_move)->get_appearance(true);
	move_notation += board_index_to_coordinates(pieces.at(piece_to_move)->position);
	move_notation += "-";
	move_notation += board_index_to_coordinates(actual_move);
	
	return move_notation;
}