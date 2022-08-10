#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include <stdexcept>

#include <chrono>
#include <thread>

#include "src/board.h"
#include "src/pieces/piece.h"
#include "src/utils.h"
#include "src/ply.h"
#include "src/ai/ai.h"
#include "src/ai/random_ai.h"
#include "src/ai/stockfly.h"

#include "src/pieces/king.h"
#include "src/pieces/queen.h"
#include "src/pieces/rook.h"
#include "src/pieces/bishop.h"
#include "src/pieces/knight.h"
#include "src/pieces/pawn.h"



#pragma comment(lib,"ws2_32.lib") //Winsock Library

void send_gui(std::string fen, SOCKET s) {
	char* message = const_cast<char*>(fen.c_str());

	if (send(s, message, strlen(message), 0) < 0) {
		std::cout << "Communication with Python GUI failed" << std::endl;
		return;
	}
	std::cout << "Successfully exchanged FEN with Python GUI" << std::endl;
}

// 0 white's turn, 1 black's turn
void game_loop(Board* board, int turn) {
	
	int i = turn;
	bool mate = false;
	bool draw = false;
	bool side_turn = (i % 2 == 0);

	while (!mate && !draw) {

		bool is_input_move_legal = false;
		bool tried_illegal_move = false;
		side_turn = (i % 2 == 0);

		while (!is_input_move_legal) {
			system("cls");
			std::cout << board->print_board(false);
			std::string move = "";

			if (tried_illegal_move) {
				std::cout << "Illegal move, try again." << std::endl;
			}
			if (side_turn) {
				std::cout << "White's turn, make a move > " << std::endl;
			}
			else {
				std::cout << "Black's turn, make a move > " << std::endl;
			}
			std::cin >> move;

			try {
				Ply* p = new Ply(move, side_turn);
				board->move(p);
				delete p;
				is_input_move_legal = true;
				i += 1;

				if (board->is_threefold_repetition()) {
					draw = true;
					system("cls");
					std::cout << board->print_board(false);
				}
				else {
					Piece* enemy_king = board->get_king(!side_turn);
					if (enemy_king->is_attacked(board).size()!=0) {
						if (board->is_mate(side_turn)) {
							mate = true;
							system("cls");
							std::cout << board->print_board(false);
						}
					}
					else if (board->is_stalemate(side_turn)) {
						draw = true;
						system("cls");
						std::cout << board->print_board(false);
					}
					else if (board->is_draw_by_insufficient_material()) {
						draw = true;
						system("cls");
						std::cout << board->print_board(false);
					}
				}
			}
			catch (const std::invalid_argument& e) {
				std::cout << e.what() << std::endl;
				tried_illegal_move = true;
			}
		}
	}

	std::string winner = side_turn ? "White" : "Black";

	if (mate)
		std::cout << "Checkmate!" << std::endl << winner << " won!";
	else
		std::cout << "Draw!";
}

void ai_game_loop(Board* board, Ai* ai, bool player_side, int turn) {

	int i = turn;
	bool mate = false;
	bool draw = false;
	bool side_turn = (i % 2 == 0);

	while (!mate && !draw) {

		bool is_input_move_legal = false;
		bool tried_illegal_move = false;
		side_turn = (i % 2 == 0);

		while (!is_input_move_legal) {
			system("cls");
			std::cout << board->print_board(false);
			std::string move_notation = "";
			Ply* move;

			if (tried_illegal_move) {
				std::cout << "Illegal move, try again." << std::endl;
			}
			if (side_turn) {
				std::cout << "White's turn, make a move > " << std::endl;
			}
			else {
				std::cout << "Black's turn, make a move > " << std::endl;
			}
			if (side_turn == player_side) {
				std::cin >> move_notation;
				move = new Ply(move_notation, side_turn);
			}
			else
				move = ai->move(board);

			try {
				//Ply* p = new Ply(move, side_turn);
				board->move(move);
				delete move;
				is_input_move_legal = true;
				i += 1;

				if (board->is_threefold_repetition()) {
					draw = true;
					system("cls");
					std::cout << board->print_board(false);
				}
				else {
					Piece* enemy_king = board->get_king(!side_turn);
					if (enemy_king->is_attacked(board).size()!=0) {
						if (board->is_mate(side_turn)) {
							mate = true;
							system("cls");
							std::cout << board->print_board(false);
						}
					}
					else if (board->is_stalemate(side_turn)) {
						draw = true;
						system("cls");
						std::cout << board->print_board(false);
					}
					else if (board->is_draw_by_insufficient_material()) {
						draw = true;
						system("cls");
						std::cout << board->print_board(false);
					}
				}
			}
			catch (const std::invalid_argument& e) {
				std::cout << e.what() << std::endl;
				tried_illegal_move = true;
			}
		}
	}

	std::string winner = side_turn ? "White" : "Black";

	if (mate)
		std::cout << "Checkmate!" << std::endl << winner << " won!";
	else
		std::cout << "Draw!";
}

void solitaire() {
	Board* board = new Board("r1k4r/1p4p1/pBbR1n2/4p1p1/2Q1P3/8/PP3PPP/6K1");
	board->set_castle_rights(false, false, false, false);
	game_loop(board, 0);

	while (true) {}
}

void ai_vs_ai_loop(Board* board, Ai* ai1, Ai* ai2, int forced_time_between_moves, SOCKET s) {
	int i = 0;
	bool mate = false;
	bool draw = false;
	bool side_turn = (i % 2 == 0);

	while (!mate && !draw) {

		bool is_input_move_legal = false;
		bool tried_illegal_move = false;
		side_turn = (i % 2 == 0);

		send_gui(board->get_fen(false), s);

		if (forced_time_between_moves != -1) {
			std::this_thread::sleep_for(std::chrono::milliseconds(forced_time_between_moves));
		}

		Ply* move;
		if (side_turn) {
			move = ai1->move(board);
		}
		else {
			move = ai2->move(board);
		}

		try {
			board->move(move);
			i += 1;
			// this if would be for board->is_threefold_repetition()
			// but not yet implemented
			if (false) {
				draw = true;
				send_gui(board->get_fen(false), s);
			}
			else {
				Piece* enemy_king = board->get_king(!side_turn);
				if (enemy_king->is_attacked(board).size() != 0) {
					if (board->is_mate(side_turn)) {
						mate = true;
						send_gui(board->get_fen(false), s);
						send_gui("Game Ended", s);
					}
				}
				else if (board->is_stalemate(side_turn)) {
					draw = true;
					send_gui(board->get_fen(false), s);
					send_gui("Game Ended", s);
				}
				else if (board->is_draw_by_insufficient_material()) {
					draw = true;
					send_gui(board->get_fen(false), s);
					send_gui("Game Ended", s);
				}
			}
		}
		catch (const std::invalid_argument& e) {
			std::cout << "Ai" << (i % 2) + 1 << " tried illegal move" << std::endl;
			std::cout << e.what() << std::endl;
		}
	}

	std::string winner = side_turn ? "White" : "Black";

	if (mate)
		std::cout << "Checkmate!" << std::endl << winner << " won!";
	else
		std::cout << "Draw!";
}

int main() {

	std::string fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
	// starting Python GUI

	// stuff to communicate with Python GUI
	WSADATA wsa;
	SOCKET s;
	struct sockaddr_in server;

	// initialising winsock
	std::cout << "Initialising Winsock [...]" << std::endl;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		std::cout << "Failed. Error Code: " << WSAGetLastError() << std::endl;
		return 1;
	}

	//Create a socket
	std::cout << "Initialising Socket [..]" << std::endl;
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		std::cout << "Could not create socket: " << WSAGetLastError() << std::endl;
	}

	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons(24718);

	//Connect to remote server
	if (connect(s, (struct sockaddr*)&server, sizeof(server)) < 0)
	{
		std::cout<<"connect error" << std::endl;
		return 1;
	}

	std::cout << "Successful connection to Python GUI [.]" << std::endl;

	Board* board = new Board("");
	//ai_game_loop(board, new Stockfly(false, 3), true, 0);

	std::cout << "Game starting now..." << std::endl;

	send_gui("Game begins", s);

	ai_vs_ai_loop(board, new Random_ai(true), new Stockfly(false, 4), 1000, s);

	closesocket(s);

	while (true) {}

	return 0;
}
