#pragma once

#include <string>
#include "board.h"

class Generic_ai {
public:
	virtual std::string move(Board*)=0;
};