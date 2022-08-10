#include "random_ai.h"
#include "../utils.h"
#include <random>

int random_int_in_range(int lower, int upper) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distr(lower, upper-1);

	return distr(gen);
}

int Random_ai::evaluate(Board* board) {
	return 0;
}

Random_ai::Random_ai(bool side) {
	Random_ai::side = side;
}

std::vector<Ply*> Random_ai::generate_moves(Board* board) {
	bool side_turn = board->get_player();
	int promotion_multiplier = side_turn ? 1 : 0;
	std::vector<Piece*> pieces = board->get_pieces(side_turn);
	std::vector<Ply*> legal_moves = std::vector<Ply*>();

	for (Piece* p : pieces) {
		std::vector<int> moves = p->pseudo_legal_moves(board);
		bool is_p_pawn = p->get_type() == 5;
		for (int move : moves) {
			if (board->is_move_legal(p, move, false)) {
				// generate promotions
				if (is_p_pawn && abs((64 * promotion_multiplier) - move) >= 56 + promotion_multiplier) {
					for (int i = 0; i < 4; i++) {
						Ply* current_ply = new Ply(p->position, move, side_turn, -1, i);
						current_ply->set_legally_generated(true);
						legal_moves.push_back(current_ply);
					}
				}
				else {
					Ply* current_ply = new Ply(p->position, move, side_turn, -1, -1);
					current_ply->set_legally_generated(true);
					legal_moves.push_back(current_ply);
				}
			}
		}
	}

	// generate castling
	if (side_turn) {
		if (board->can_castle(true, true)) {
			Ply* current_ply = new Ply(-1, -1, side_turn, 0, -1);
			current_ply->set_legally_generated(true);
			legal_moves.push_back(current_ply);
		}

		if (board->can_castle(false, true)) {
			Ply* current_ply = new Ply(-1, -1, side_turn, 1, -1);
			current_ply->set_legally_generated(true);
			legal_moves.push_back(current_ply);
		}
	}
	else {
		if (board->can_castle(true, false)) {
			Ply* current_ply = new Ply(-1, -1, side_turn, 2, -1);
			current_ply->set_legally_generated(true);
			legal_moves.push_back(current_ply);
		}
		if (board->can_castle(false, false)) {
			Ply* current_ply = new Ply(-1, -1, side_turn, 3, -1);
			current_ply->set_legally_generated(true);
			legal_moves.push_back(current_ply);
		}
	}

	// add castles to moves (what about promotion??)
	return legal_moves;
}

Ply* Random_ai::move(Board* board) {
	
	std::vector<Ply*> legal_moves = generate_moves(board);

	int random_move_index = random_int_in_range(0, legal_moves.size());

	return legal_moves.at(random_move_index);
}