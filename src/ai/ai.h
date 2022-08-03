#pragma once

#include <string>
#include "../board.h"

class Ai {
public:
	virtual int evaluate(Board*) = 0;
	virtual std::vector<std::string> generate_moves(Board*)=0;
	virtual std::string move(Board*)=0;
};