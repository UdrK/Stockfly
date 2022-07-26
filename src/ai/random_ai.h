#pragma once

#include "ai.h"
#include "../pieces/piece.h"

class Random_ai : public Ai {
private:
	bool side;
public:
	Random_ai(bool);
	std::vector<std::string> generate_moves(Board*);
	std::string move(Board*);
};