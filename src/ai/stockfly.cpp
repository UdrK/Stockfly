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
	int side_multiplier = board->get_player() ? 1 : -1;
	return material_imbalance(board) * side_multiplier;
}

std::vector<std::string> Stockfly::generate_moves(Board* board) {

	bool side_turn = board->get_player();
	int promotion_multiplier = side_turn ? 1 : 0;
	std::vector<Piece*> pieces = board->get_pieces(side_turn);
	std::vector<std::string> legal_moves = std::vector<std::string>();

	for (Piece* p : pieces) {
		std::vector<int> moves = p->pseudo_legal_moves(board);
		for (int move : moves) {
			// does not check if move is legal, we let that to board->move 
			std::string str_move = piece_and_position_to_move(p, move);
			
			// generate promotions
			if (p->get_appearance(true) == "P" && abs((64 * promotion_multiplier) - move) >= 56 + promotion_multiplier) {
				legal_moves.push_back(str_move + "-Q");
				legal_moves.push_back(str_move + "-R");
				legal_moves.push_back(str_move + "-B");
				legal_moves.push_back(str_move + "-N");
			}
			else {
				legal_moves.push_back(str_move);
			}
		}
	}
	
	// generate castling
	bool* castle_rights = board->get_castle_rights();
	if (side_turn) {
		if (castle_rights[0])
			legal_moves.push_back("O-O");
		if (castle_rights[1])
			legal_moves.push_back("O-O-O");
	}
	else {
		if (castle_rights[2])
			legal_moves.push_back("o-o");
		if (castle_rights[3])
			legal_moves.push_back("o-o-o");
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

		int illegal_moves = 0;
		for (std::string move : moves) {
			Ply* p = new Ply(move, board->get_player());
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
			// >= ????
			if (eval > alpha) {
				alpha = eval;
			}
		}

		// no legal moves
		if (moves.size() == illegal_moves) {
			// if king in check: checkmate
			if (board->get_king(board->get_player())->is_attacked(board)) {
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

std::string Stockfly::move(Board* board) {
	std::vector<std::string> moves = generate_moves(board);

	int side_multiplier = Stockfly::side ? 1 : -1;
	int best_eval = bad_position;
	std::string best_move = moves[0];

	for (std::string move : moves) {
		Ply* p = new Ply(move, board->get_player());
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
			best_move = move;
		}
		board->undo_move(p);
		delete p;
	}

	return best_move;
}
