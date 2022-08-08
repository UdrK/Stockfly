#include "ply.h"
#include "utils.h"

Ply::Ply(int origin_square, int destination_square, bool player, int castle_move, int promotion_move) {
	
	Ply::origin_square = origin_square;
	Ply::destination_square = destination_square;
	Ply::player = player;
	Ply::castle_move = castle_move;
	Ply::promotion_move = promotion_move;

	if (castle_move != -1)
		Ply::castle = true;
	else
		Ply::castle = false;

	if (promotion_move != -1)
		Ply::promotion = true;
	else
		Ply::promotion = false;
}

Ply::Ply(std::string move_notation, bool player) {
	
	int castle_move = -1;
	int promotion_move = -1;

	int origin_square = -1;
	int destination_square = -1;

	if (move_notation[0] == 'O' || move_notation[0] == 'o') {
		if (move_notation == "O-O") {
			castle_move = 0;
		}
		else if (move_notation == "O-O-O") {
			castle_move = 1;
		}
		else if (move_notation == "o-o") {
			castle_move = 2;
		}
		else if (move_notation == "o-o-o") {
			castle_move = 3;
		}
	}
	else {
		origin_square = coordinates_to_board_index(move_notation.substr(1, 2));
		destination_square = coordinates_to_board_index(move_notation.substr(4, 2));
	}

	if (move_notation.size() == 8) {
		if (move_notation.substr(7, 1) == "R") {
			promotion_move = 1;
		}
		else if (move_notation.substr(7, 1) == "B") {
			promotion_move = 2;
		}
		else if (move_notation.substr(7, 1) == "N") {
			promotion_move = 3;
		}
		else {
			promotion_move = 0;
		}
	}
	else if (move_notation[0] == 'P' && ((player && destination_square <= 7) || (!player && destination_square >= 56))) {
		promotion_move = 0;
	}

	Ply::origin_square = origin_square;
	Ply::destination_square = destination_square;
	Ply::player = player;
	Ply::castle_move = castle_move;
	Ply::promotion_move = promotion_move;

	if (castle_move != -1)
		Ply::castle = true;
	else
		Ply::castle = false;

	if (promotion_move != -1)
		Ply::promotion = true;
	else
		Ply::promotion = false;
}

Ply* Ply::clone() {
	Ply* p = new Ply(Ply::origin_square, Ply::destination_square, Ply::player, Ply::castle_move, Ply::promotion_move);
	p->set_castle(this->is_castle());
	bool* rights = this->get_castle_rights();
	p->set_castle_rights(rights[0], rights[1], rights[2], rights[3]);
	p->set_en_passant_file(this->get_en_passant_file());
	p->set_legally_generated(this->is_legally_generated());
	p->set_move_notation(this->get_move_notation());
	p->set_moving_piece(this->get_moving_piece());
	p->set_promotion(this->is_promotion());
	p->set_promotion_piece(this->get_promotion_piece());
	p->set_taken_piece(this->get_taken_piece());
	p->set_taken_square(this->get_taken_square());
	return p;
}

std::string Ply::get_move_notation() {
	return Ply::move_notation;
}

bool Ply::get_player() {
	return Ply::player;
}

Piece* Ply::get_moving_piece() {
	return Ply::moving_piece;
}

Piece* Ply::get_taken_piece() {
	return Ply::taken_piece;
}

Piece* Ply::get_promotion_piece() {
	return Ply::promotion_piece;
}

int Ply::get_origin_square() {
	return Ply::origin_square;
}

int Ply::get_destination_square() {
	return Ply::destination_square;
}

int Ply::get_taken_square() {
	return Ply::taken_square;
}

int Ply::get_castle_move() {
	return Ply::castle_move;
}

int Ply::get_promotion_move() {
	return Ply::promotion_move;
}

bool* Ply::get_castle_rights() {
	bool rights[4] = { Ply::white_king_side_castle, Ply::white_queen_side_castle, Ply::black_king_side_castle, Ply::black_queen_side_castle };
	return rights;
}

int Ply::get_en_passant_file() {
	return en_passant_file;
}

void Ply::set_move_notation(std::string move_notation) {
	Ply::move_notation = move_notation;
}

void Ply::set_origin_square(int origin_square) {
	Ply::origin_square = origin_square;
}

void Ply::set_destination_square(int destination_square) {
	Ply::destination_square = destination_square;
}

void Ply::set_castle_move(int castle_move) {
	Ply::castle_move = castle_move;
}

void Ply::set_promotion_move(int promotion_moev) {
	Ply::promotion_move = promotion_move;
}

void Ply::set_legally_generated(bool legally_generated) {
	Ply::legally_generated = legally_generated;
}

void Ply::set_castle(bool castle) {
	Ply::castle = castle;
}

void Ply::set_promotion(bool promotion) {
	Ply::promotion = promotion;
}

void Ply::set_moving_piece(Piece* piece) {
	Ply::moving_piece = piece;
}

void Ply::set_taken_piece(Piece* piece) {
	Ply::taken_piece = piece;
}

void Ply::set_promotion_piece(Piece* piece) {
	Ply::promotion_piece = piece;
}

void Ply::set_taken_square(int square) {
	Ply::taken_square = square;
}

void Ply::set_castle_rights(bool white_king_side_castle, bool white_queen_side_castle, bool black_king_side_castle, bool black_queen_side_castle) {
	Ply::white_king_side_castle = white_king_side_castle;
	Ply::white_queen_side_castle = white_queen_side_castle;
	Ply::black_king_side_castle = black_king_side_castle;
	Ply::black_queen_side_castle = black_queen_side_castle;
}

void Ply::set_en_passant_file(int en_passant_file) {
	Ply::en_passant_file = en_passant_file;
}

bool Ply::is_legally_generated() {
	return Ply::legally_generated;
}

bool Ply::is_en_passant() {
	return taken_square != -1 && destination_square != taken_square;
}

bool Ply::is_castle() {
	return Ply::castle;
}

bool Ply::is_promotion() {
	return Ply::promotion;
}
