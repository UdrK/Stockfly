# CMakeList.txt : CMake project for stockfly, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("stockfly")

# Add source to this project's executable.
add_executable (stockfly "src/ai/stockfly.cpp" "src/ai/stockfly.h" "test.cpp" "src/utils.cpp" "src/utils.h" "src/board.h" "src/board.cpp" "src/pieces/piece.h" "src/pieces/piece.cpp" "src/pieces/king.h" "src/pieces/king.cpp" "src/pieces/queen.h" "src/pieces/queen.cpp" "src/pieces/rook.h" "src/pieces/rook.cpp" "src/pieces/bishop.h" "src/pieces/bishop.cpp" "src/pieces/knight.h" "src/pieces/knight.cpp" "src/pieces/pawn.h" "src/pieces/pawn.cpp" "game_loop.cpp" "src/ai/ai.h" "src/ai/random_ai.h" "src/ai/random_ai.cpp"  "src/ply.h" "src/ply.cpp" )

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET stockfly PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
