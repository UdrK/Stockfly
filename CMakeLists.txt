# CMakeList.txt : CMake project for stockfly, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("stockfly")

# Add source to this project's executable.
add_executable (stockfly "src/stockfly.cpp" "src/stockfly.h" "test.cpp" "src/utils.cpp" "src/utils.h" "src/board.h" "src/board.cpp" "src/piece.h" "src/piece.cpp" "src/king.h" "src/king.cpp" "src/queen.h" "src/queen.cpp" "src/rook.h" "src/rook.cpp" "src/bishop.h" "src/bishop.cpp" "src/knight.h" "src/knight.cpp" "src/pawn.h" "src/pawn.cpp" "game.cpp" "src/generic_ai.h" "src/random_ai.h" "src/random_ai.cpp" )

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET stockfly PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
