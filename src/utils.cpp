#include "utils.h"
#include "piece.h"
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

// given chess style coordinates e.g. e4, returns board representation coordinate (aka index) e.g. e4 -> 35
int coordinates_to_board_index(std::string coordinates) {
    int file_number = int(coordinates[0]) - 97;  // file: a->0, b->1, ..., h->7 
    int rank_number = 8 - (int(coordinates[1]) - 48);  // rank
    int index = 8 * rank_number + file_number;

    if (file_number < 0 || file_number > 7 || rank_number < 0 || rank_number > 7) {
        throw std::invalid_argument("Coordinates do not represent a square on the chessboard");
    }

    return index;
}

std::string board_index_to_coordinates(int index) {
    int aux = (index / 8);
    int rank = 8 - aux;
    int file = (index % 8);

    std::string file_letter = std::string(1, char(97 + file));
    std::string coordinates = file_letter + std::to_string(rank);

    return coordinates;
}

std::string piece_position_to_move(Piece* piece, int position) {
    std::string str_move = piece->get_appearance(true);
    str_move += board_index_to_coordinates(piece->position);
    str_move += "-";
    str_move += board_index_to_coordinates(position);

    return str_move;
}