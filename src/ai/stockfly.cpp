// stockfly.cpp : Defines the entry point for the application.
//

#include "stockfly.h"
#include "../utils.h"
#include "../pieces/piece.h"
#include "../pieces/king.h"

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
		std::string piece_type = piece->get_appearance(true);
		if (piece_type == "P")
			material_imbalance += pawn;
		if (piece_type == "K")
			material_imbalance += knight;
		if (piece_type == "B")
			material_imbalance += bishop;
		if (piece_type == "R")
			material_imbalance += rook;
		if (piece_type == "Q")
			material_imbalance += queen;
	}

	for (Piece* piece : black_pieces) {
		std::string piece_type = piece->get_appearance(true);
		if (piece_type == "P")
			material_imbalance -= pawn;
		if (piece_type == "K")
			material_imbalance -= knight;
		if (piece_type == "B")
			material_imbalance -= bishop;
		if (piece_type == "R")
			material_imbalance -= rook;
		if (piece_type == "Q")
			material_imbalance -= queen;
	}

	return material_imbalance;
}

int Stockfly::evaluate(Board* board) {
	int side_multiplier = board->get_side_turn() ? 1 : -1;
	return material_imbalance(board) * side_multiplier;
}

std::vector<std::string> Stockfly::generate_moves(Board* board) {

	bool side_turn = board->get_side_turn();
	std::vector<Piece*> pieces = board->get_pieces(side_turn);
	std::vector<std::string> legal_moves = std::vector<std::string>();

	for (Piece* p : pieces) {
		std::vector<int> moves = p->pseudo_legal_moves(board);
		for (int move : moves) {
			if (board->is_move_legal(p, move)) {
				std::string str_move = piece_position_to_move(p, move);
				legal_moves.push_back(str_move);
			}
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
		std::vector<std::string> moves = generate_moves(board);

		// no legal moves
		if (moves.size() == 0) {
			// if king in check: checkmate
			if (board->get_king(board->get_side_turn())->is_attacked(board)) {
				return bad_position;
			}
			// else stalemate
			else {
				return 0;
			}
		}

		std::string unmake_move_fen = board->get_fen(true);
		for (std::string move : moves) {
			board->move(move, board->get_side_turn());
			int eval = - negamax(depth - 1, -beta, -alpha, board);
			board->set_from_fen(unmake_move_fen, true);
			if (eval >= beta) {
				// move is better than other previously considered move, therefore opponent will avoid this branch
				return beta;
			}
			if (eval >= alpha) {
				alpha = eval;
			}
		}
		return alpha;
	}
}

Stockfly::Stockfly(bool side) {
	Stockfly::side = side;
}

std::string Stockfly::move(Board* board) {
	std::vector<std::string> moves = generate_moves(board);
	std::string unmake_move_fen = board->get_fen(true);

	int side_multiplier = Stockfly::side ? 1 : -1;
	int best_eval = bad_position;
	std::string best_move = moves[0];

	for (std::string move : moves) {
		board->move(move, board->get_side_turn());
		int move_eval = -Stockfly::negamax(3, bad_position, good_position, board);
		if (move_eval > best_eval) {
			best_eval = move_eval;
			best_move = move;
		}
		board->set_from_fen(unmake_move_fen, true);
	}

	return best_move;
}