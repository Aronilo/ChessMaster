#pragma once

#include "Figures.h"
#include "ChessBoard.h"
#include "ChessPlayer.h"
#include <string>
#include <vector>

using namespace std;

class CChessGame {
public:
	CChessGame();
	~CChessGame();

	bool m_bInGame;
	int m_iCurrentMove;
	CChessBoard m_ChessBoard;

	int DoPlay();
	void Intro();

	bool WhitesMove();
	bool BlackesMove();

	bool CaptureMove(ChessMove& Move, bool bComp = false);

	ChessPlayer m_WhitePlayer, m_BlackPlayer;

	bool StartUCI();

	string GetMovesString();
	void SendCommand(string str);
	string ParseBestMove(string read, bool& ok);

	bool bThreadAlive;

	vector <string> moves;
};

