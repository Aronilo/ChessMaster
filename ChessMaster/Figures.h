#pragma once


#include "Figure.h"


class CChessBoard;
struct ChessMove;

//пешка
class CPawn :
	public CFigure {
public:
	CPawn();
	CPawn(int color) { m_iFigureColor = color; }
	~CPawn();
	void Print();

	bool PerformMove(CChessBoard* b, ChessMove* M);
	bool Beats(CChessBoard* board);
};

//ладья
class CRook :
	public CFigure {
public:
	CRook();
	CRook(int color) { m_iFigureColor = color; }
	~CRook();
	void Print();

	bool PerformMove(CChessBoard* b, ChessMove* M);
	bool Beats(CChessBoard* board);
};

//конь
class CKnight :
	public CFigure {
public:
	CKnight();
	CKnight(int color) { m_iFigureColor = color; }
	~CKnight();
	void Print();

	bool PerformMove(CChessBoard* b, ChessMove* M);
	bool Beats(CChessBoard* board);
};

//слон
class CBishop :
	public CFigure {
public:
	CBishop();
	CBishop(int color) { m_iFigureColor = color; }
	~CBishop();
	void Print();

	bool PerformMove(CChessBoard* b, ChessMove* M);
	bool Beats(CChessBoard* board);
};

//ферзь
class CQueen :
	public CFigure {
public:
	CQueen();
	CQueen(int color) { m_iFigureColor = color; }
	~CQueen();
	void Print();

	bool PerformMove(CChessBoard* b, ChessMove* M);
	bool Beats(CChessBoard* board);
};

//король
class CKing : public CFigure {
public:
	CKing();
	CKing(int color) { m_iFigureColor = color; }
	~CKing();
	void Print();

	bool PerformMove(CChessBoard* b, ChessMove* M);
	bool Castling(CChessBoard* b, ChessMove* M);
	bool Beats(CChessBoard* board);

	bool IsChecked();
	bool IsCheckMated(CChessBoard* b);
	bool TestCellForWhiteKing(CChessBoard* b, int N, int L);
	bool TestCellForBlackKing(CChessBoard* b, int N, int L);
};
