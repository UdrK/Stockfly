#pragma once

#include <stdlib.h>
#include <string>
#include <map>
#include <vector>
#include "piece.h"

extern std::map<std::string, std::string> unicode_pieces;

int coordinates_to_board_index(std::string);
std::string board_index_to_coordinates(int);
std::string piece_position_to_move(Piece*, int);