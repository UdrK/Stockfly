#include "ply.h"
#include "utils.h"

Ply::Ply(std::string move_notation, bool player) {
	Ply::move_notation = move_notation;
	Ply::player = player;
	if (move_notation != "O-O" && move_notation != "O-O-O" && move_notation != "o-o" && move_notation != "o-o-o") {
		Ply::origin_square = coordinates_to_board_index(move_notation.substr(1, 2));
		Ply::destination_square = coordinates_to_board_index(move_notation.substr(4, 2));
		if ((move_notation[0] == 'P' && destination_square <= 7 && player) ||
			(move_notation[0] == 'P' && destination_square >= 56 && !player)) {
			Ply::promotion = true;
			Ply::castle = false;
		}
	}
	else {
		Ply::castle = true;
		Ply::promotion = false;
		Ply::origin_square = -1;
		Ply::destination_square = -1;
		Ply::taken_square = -1;
	}
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

int Ply::get_origin_square() {
	return Ply::origin_square;
}

int Ply::get_destination_square() {
	return Ply::destination_square;
}

int Ply::get_taken_square() {
	return Ply::taken_square;
}

void Ply::set_moving_piece(Piece* piece) {
	Ply::moving_piece = piece;
}

void Ply::set_taken_piece(Piece* piece) {
	Ply::taken_piece = piece;
}

void Ply::set_taken_square(int square) {
	Ply::taken_square = square;
}

bool Ply::is_en_passant() {
	return 'P' == Ply::move_notation[0] && taken_square != -1 && destination_square != taken_square;
}

bool Ply::is_castle() {
	return Ply::castle;
}

bool Ply::is_promotion() {
	return Ply::promotion;
}
