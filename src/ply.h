#pragma once

#include <string>

class Piece;

/*
After the constructor is called, only 
move_notation, origin_square and destination_square are initialized
Board::move(Ply*) sets the other parameters, which can then be used wherever needed (e.g. to unmake moves)
*/
class Ply {
public:
	Ply(std::string, bool);

	// getters
	std::string get_move_notation();
	bool get_player();
	Piece* get_moving_piece();
	Piece* get_taken_piece();
	Piece* get_promotion_piece();
	int get_origin_square();
	int get_destination_square();
	int get_taken_square();
	bool* get_castle_rights();
	int get_en_passant_file();

	// setters
	void set_moving_piece(Piece*);
	void set_taken_piece(Piece*);
	void set_promotion_piece(Piece*);
	void set_taken_square(int);
	void set_origin_square(int);
	void set_destination_square(int);
	void set_castle(bool);
	void set_promotion(bool);
	void set_castle_rights(bool, bool, bool, bool);
	void set_en_passant_file(int);

	bool is_en_passant();
	bool is_castle();
	bool is_promotion();

private:
	std::string move_notation;
	bool player;	// true: white, false: black
	Piece* moving_piece;
	Piece* taken_piece;
	Piece* promotion_piece;
	int origin_square;
	int destination_square;
	int taken_square;
	bool castle;
	bool promotion;
	bool white_king_side_castle, white_queen_side_castle;
	bool black_king_side_castle, black_queen_side_castle;
	int en_passant_file = 8;
};
