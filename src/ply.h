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
	Ply(int, int, bool, int, int);
	Ply(std::string, bool);

	Ply* clone();

	// getters
	std::string get_move_notation();
	bool get_player();
	Piece* get_moving_piece();
	Piece* get_taken_piece();
	Piece* get_promotion_piece();
	int get_origin_square();
	int get_destination_square();
	int get_taken_square();
	int get_castle_move();
	int get_promotion_move();
	
	bool* get_castle_rights();
	int get_en_passant_file();

	// setters
	void set_move_notation(std::string);
	void set_moving_piece(Piece*);
	void set_taken_piece(Piece*);
	void set_promotion_piece(Piece*);
	void set_taken_square(int);
	void set_origin_square(int);
	void set_destination_square(int);
	void set_castle_move(int);
	void set_promotion_move(int);
	void set_legally_generated(bool);
	void set_castle(bool);
	void set_promotion(bool);
	void set_castle_rights(bool, bool, bool, bool);
	void set_en_passant_file(int);

	bool is_legally_generated();
	bool is_en_passant();
	bool is_castle();
	bool is_promotion();

private:
	std::string move_notation;
	bool player;	        // true: white, false: black
	bool legally_generated = false;
	Piece* moving_piece;
	Piece* taken_piece;
	Piece* promotion_piece;

	// information that actually codifies the move ---- (remaining variables are needed to undo the move)
	int origin_square;
	int destination_square;

	int castle_move = -1;	    // 0 White King side, 1 White Queen side, 2 Black King side, 3 Black Queen side
	int promotion_move = -1;	    // 0 Queen promotion, 1 Rook promotion, 2 Bishop promotion, 3 Knight promotion
	// ----

	int taken_square;
	bool castle = false;
	bool promotion = false;
	bool white_king_side_castle = false, white_queen_side_castle = false;
	bool black_king_side_castle = false, black_queen_side_castle = false;
	int en_passant_file = 8;
};
