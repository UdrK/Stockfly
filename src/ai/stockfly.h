#pragma once

#include "ai.h"
#include "../pieces/piece.h"

class Stockfly : public Ai {
private:
	bool side;
	int search_depth;
	int material_imbalance(Board*);
	int negamax(int, int, int, Board*);
public:
	Stockfly(bool, int);
	int evaluate(Board*);
	std::vector<Ply*> generate_moves(Board*);
	Ply* move(Board*);
};