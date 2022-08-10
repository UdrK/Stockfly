// stockfly.cpp : Defines the entry point for the application.

#include "stockfly.h"
#include <iostream>
#include "../utils.h"
#include "../pieces/piece.h"
#include "../pieces/king.h"
#include "../ply.h"

int bad_position = -10000;
int good_position = 10000;

int pawn = 100;
int bishop = 3 * pawn;
int knight = 3 * pawn;
int rook = 5 * pawn;
int queen = 9 * pawn;

int Stockfly::material_imbalance(Board* board) {
	std::vector<Piece*> white_pieces = board->get_pieces(true);
	std::vector<Piece*> black_pieces = board->get_pieces(false);

	int material_imbalance = 0;

	for (Piece* piece : white_pieces) {
		int piece_type = piece->get_type();
		if (piece_type == 5)
			material_imbalance += pawn;
		if (piece_type == 4)
			material_imbalance += knight;
		if (piece_type == 3)
			material_imbalance += bishop;
		if (piece_type == 2)
			material_imbalance += rook;
		if (piece_type == 1)
			material_imbalance += queen;
	}

	for (Piece* piece : black_pieces) {
		int piece_type = piece->get_type();
		if (piece_type == 5)
			material_imbalance -= pawn;
		if (piece_type == 4)
			material_imbalance -= knight;
		if (piece_type == 3)
			material_imbalance -= bishop;
		if (piece_type == 2)
			material_imbalance -= rook;
		if (piece_type == 1)
			material_imbalance -= queen;
	}

	return material_imbalance;
}

int Stockfly::evaluate(Board* board) {
	int side_multiplier = board->get_player() ? 1 : -1;
	return material_imbalance(board) * side_multiplier;
}

std::vector<Ply*> Stockfly::generate_moves(Board* board) {

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

// alpha-beta pruning version of negamax (generalized minimax)
// first round of optimization: order std::vector<std::string> moves based on euristics (capturing high value piece is probably a good move)
// to help with alpha-beta pruning
// second: instead of returning evaluate(board) when depth == 0 return a secondary negamax, calculated only for capture moves
// this helps with the horizon problem: computer enters line in which at end a piece hangs, because it can't see to the depth+1 move to see that the piece hangs
int Stockfly::negamax(int depth, int alpha, int beta, Board* board) {
	if (depth == 0) {
		int static_evaluation = evaluate(board);
		return static_evaluation;
	}
	else {
		std::vector<Ply*> moves = generate_moves(board);

		int illegal_moves = 0;
		for (Ply* p : moves) {
			try {
				board->move(p);
			}
			catch (const std::invalid_argument& e) {
				illegal_moves++;
				delete p;
				continue;
			}
			
			int eval = - negamax(depth - 1, -beta, -alpha, board);

			board->undo_move(p);

			delete p;

			if (eval >= beta) {
				// move is better than other previously considered move, therefore opponent will avoid this branch
				return beta;
			}
			if (eval > alpha) {
				alpha = eval;
			}
		}

		// no legal moves
		if (moves.size() == illegal_moves) {
			// if king in check: checkmate
			if (board->get_king(board->get_player())->is_attacked(board).size()!=0) {
				return bad_position;
			}
			// else stalemate
			else {
				return 0;
			}
		}

		return alpha;
	}
}

Stockfly::Stockfly(bool side, int search_depth) {
	Stockfly::side = side;
	Stockfly::search_depth = search_depth;
}

Ply* Stockfly::move(Board* board) {
	std::vector<Ply*> moves = generate_moves(board);

	int side_multiplier = Stockfly::side ? 1 : -1;
	int best_eval = bad_position;
	Ply* best_move = moves[0]->clone();

	for (Ply* p : moves) {
		try {
			board->move(p);
		}
		catch (const std::invalid_argument& e) {
			delete p;
			continue;
		}
		int move_eval = -Stockfly::negamax(Stockfly::search_depth - 1, bad_position, good_position, board);
		if (move_eval >= best_eval) {
			best_eval = move_eval;
			best_move = p->clone();
		}
		board->undo_move(p);
		delete p;
	}

	return best_move;
}
