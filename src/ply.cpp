#include "ply.h"
#include "utils.h"

Ply::Ply(std::string move_notation, bool player) {
	Ply::move_notation = move_notation;
	Ply::player = player;
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

bool* Ply::get_castle_rights() {
	bool rights[4] = { Ply::white_king_side_castle, Ply::white_queen_side_castle, Ply::black_king_side_castle, Ply::black_queen_side_castle };
	return rights;
}

int Ply::get_en_passant_file() {
	return en_passant_file;
}


void Ply::set_origin_square(int origin_square) {
	Ply::origin_square = origin_square;
}

void Ply::set_destination_square(int destination_square) {
	Ply::destination_square = destination_square;
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

bool Ply::is_en_passant() {
	return 'P' == Ply::move_notation[0] && taken_square != -1 && destination_square != taken_square;
}

bool Ply::is_castle() {
	return Ply::castle;
}

bool Ply::is_promotion() {
	return Ply::promotion;
}
