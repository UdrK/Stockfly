#pragma once

#include "generic_ai.h"
#include "piece.h"

class Stockfly : public Generic_ai {
private:
	bool side;
	int material_imbalance(Board*);
	int evaluate(Board*);
	std::vector<std::string> generate_moves(Board*);
	int negamax(int, int, int, Board*);
public:
	Stockfly(bool);
	std::string move(Board*);
};