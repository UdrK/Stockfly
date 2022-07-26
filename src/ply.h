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
	bool get_player();
	Piece* get_moving_piece();
	Piece* get_taken_piece();
	int get_origin_square();
	int get_destination_square();
	int get_taken_square();

	// setters
	void set_moving_piece(Piece*);
	void set_taken_piece(Piece*);
	void set_taken_square(int);

	bool is_en_passant();
	bool is_castle();
	bool is_promotion();

private:
	std::string move_notation;
	bool player;	// true: white, false: black
	Piece* moving_piece;
	Piece* taken_piece;
	int origin_square;
	int destination_square;
	int taken_square;
	bool castle;
	bool promotion;
};
