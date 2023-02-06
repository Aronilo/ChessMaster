#pragma once
//#include "ChessBoard.h"

#define FIGURE_COLOR_WHITE 1
#define FIGURE_COLOR_BLACK -1

class CChessBoard;
class CChessCell;
struct ChessMove;

class CFigure {
public:
	CFigure();
	CFigure(int iColor) { m_iFigureColor = iColor; }
	~CFigure();

	virtual void Print() {}
	virtual bool PerformMove(CChessBoard* b, ChessMove* M) { return false; }
	virtual bool Beats(CChessBoard* board) { if (!m_bInGame) return false; }
	bool BeatsNL(CChessBoard* board, int N, int L);


	int m_iFigureColor; //цвет фигуры
	bool m_bInGame = true; //признак живая или нет
	CChessCell* m_Position;
	bool m_bInInitialPos = true;
};

