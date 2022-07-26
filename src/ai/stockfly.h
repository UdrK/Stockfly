#pragma once

#include "ai.h"
#include "../pieces/piece.h"

class Stockfly : public Ai {
private:
	bool side;
	int material_imbalance(Board*);
	int evaluate(Board*);
	int negamax(int, int, int, Board*);
public:
	Stockfly(bool);
	std::vector<std::string> generate_moves(Board*);
	std::string move(Board*);
};