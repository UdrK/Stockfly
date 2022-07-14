#include "utils.h"
#include <stdlib.h>
#include <stdexcept>
#include <string>
#include <map>
#include <vector>
#include <iostream>

std::map<std::string, std::string> unicode_pieces = {
    {"K", "\xE2\x99\x94"},
    {"Q", "\xE2\x99\x95"},
    {"R", "\xE2\x99\x96"},
    {"B", "\xE2\x99\x97"},
    {"N", "\xE2\x99\x98"},
    {"P", "\xE2\x99\x99"},
    {"k", "\xE2\x99\x9A"},
    {"q", "\xE2\x99\x9B"},
    {"r", "\xE2\x99\x9C"},
    {"b", "\xE2\x99\x9D"},
    {"n", "\xE2\x99\x9E"},
    {"p", "\xE2\x99\x9F"},
};

int coordinates_to_board_index(std::string coordinates) {
    int file_number = int(coordinates[0]) - 97;  // column: a->0, b->1, ..., h->7 
    int rank_number = 8 - (int(coordinates[1]) - 48);  // row: these are reversed in my representation, also should start from 0 instead of 1
    int index = 8 * rank_number + file_number;

    if (file_number < 0 || file_number > 7 || rank_number < 0 || rank_number > 7) {
        throw std::invalid_argument("Coordinates do not represent a square on the chessboard");
    }

    return index;
}
