#include "stdafx.h"
#include "Figures.h"
#include "ChessGame.h"
#include <iostream>


using namespace std;

int main() {
	CChessGame game;
	switch (game.DoPlay()) {
	case CHESS_GAME_WHITE_WON:
		cout << "White won!\n";
		break;
	case CHESS_GAME_BLACK_WON:
		cout << "Black won!\n";
		break;
	case CHESS_GAME_DRAW:
		cout << "Draw!\n";
		break;
	};

	system("pause");
	return 0;
}