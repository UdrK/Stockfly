#pragma once

#include "ai.h"
#include "../pieces/piece.h"

class Random_ai : public Ai {
private:
	bool side;
public:
	Random_ai(bool);
	int evaluate(Board*);
	std::vector<Ply*> generate_moves(Board*);
	Ply* move(Board*);
};