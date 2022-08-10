#pragma once

#include <string>
#include "../board.h"

class Ai {
public:
	virtual int evaluate(Board*) = 0;
	virtual std::vector<Ply*> generate_moves(Board*) = 0;
	virtual Ply* move(Board*)=0;
};