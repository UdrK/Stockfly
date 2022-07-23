#pragma once

#include <string>
#include "board.h"

class Generic_ai {
public:
	virtual std::vector<std::string> generate_moves(Board*)=0;
	virtual std::string move(Board*)=0;
};