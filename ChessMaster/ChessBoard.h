#pragma once

#include <vector>
#include <string>

#include "Figures.h"

#define CELL_COLOR_WHITE 1
#define CELL_COLOR_BLACK -1

#define CELL_LETTER_A 0
#define CELL_LETTER_B 1
#define CELL_LETTER_C 2
#define CELL_LETTER_D 3
#define CELL_LETTER_E 4
#define CELL_LETTER_F 5
#define CELL_LETTER_G 6
#define CELL_LETTER_H 7

#define CHESS_GAME_MOVE_WHITE 1
#define CHESS_GAME_MOVE_BLACK -1

#define CHESS_GAME_WHITE_WON     -1
#define CHESS_GAME_RESULT_UNKNOWN 0
#define CHESS_GAME_BLACK_WON      1
#define CHESS_GAME_DRAW           2

using namespace std;

class CChessCell {
public:
	CChessCell() { m_pFigure = NULL; }
	~CChessCell() {};

	int m_iColor;

	int m_iCellLetter;
	int m_iCellNumber;

	bool m_bUnderWhiteAttack = false;
	bool m_bUnderBlackAttack = false;

	void Print();
	void SetFigure(CFigure* pFigure);
	bool IsValid() { return (m_iCellNumber < 8 && m_iCellNumber > -1 && m_iCellLetter < 8 && m_iCellLetter > -1); }

	CFigure* m_pFigure;

	bool IsVoid() { return m_pFigure == NULL; }
};

struct ChessMove {
	CChessCell* pos_old;
	CChessCell* pos_new;
	int m_iColor;
};

class CChessBoard {
public:
	CChessBoard();
	~CChessBoard();

	void InitBoard();
	void PrintBoard();

	void MakeCellsUnderAttack();

	bool PerformMove(ChessMove M, bool& bEndGame);

	bool IsStalemate(int color);

	int GetGameResult() { return m_iResult; }

	CChessCell m_Cells[8][8];


	CPawn m_Pawns[16];

	CRook m_Rooks[4];


	CKnight m_Knights[4];

	CBishop m_Bishops[4];

	vector<CQueen> m_Queens;

	CKing m_Kings[2];

	vector <CFigure*> m_pFigures;
	vector <CFigure*> m_pWhiteFigures;
	vector <CFigure*> m_pBlackFigures;

	string last_move;

	int m_iResult = CHESS_GAME_RESULT_UNKNOWN;
};

