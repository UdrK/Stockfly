#pragma once

#include "generic_ai.h"
#include "piece.h"

class Random_ai : public Generic_ai {
private:
	bool side;
public:
	Random_ai(bool);
	std::string move(Board*);
};