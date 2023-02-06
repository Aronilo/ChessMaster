#include "stdafx.h"
#include "Figures.h"
#include "ChessBoard.h"
#include <iostream>

using namespace std;

CKnight::CKnight() {}


CKnight::~CKnight() {}

void CKnight::Print() {
	if (m_iFigureColor == FIGURE_COLOR_WHITE)
		cout << "WKn ";
	else
		cout << "BKn ";
}

bool CKnight::PerformMove(CChessBoard* b, ChessMove* M) {

	if (m_Position->m_iCellLetter != M->pos_old->m_iCellLetter || m_Position->m_iCellNumber != M->pos_old->m_iCellNumber)
		return false;


	int linedelta = M->pos_new->m_iCellNumber - M->pos_old->m_iCellNumber;
	int letterdelta = M->pos_new->m_iCellLetter - M->pos_old->m_iCellLetter;

	if (!(fabs(linedelta) == 2 && fabs(letterdelta) == 1 || fabs(letterdelta) == 2 && fabs(linedelta) == 1))
		return false;

	//клетка назначения
	int N = M->pos_new->m_iCellNumber;
	int L = M->pos_new->m_iCellLetter;

	if (!b->m_Cells[N][L].IsVoid()) {
		M->pos_new->m_pFigure->m_bInGame = false;
		M->pos_new->m_pFigure->m_Position = NULL;
	}

	//ходим
	M->pos_old->SetFigure(NULL);
	M->pos_new->SetFigure(this);

	m_bInInitialPos = false;
	return true;
}

bool CKnight::Beats(CChessBoard* board) {

	bool res = false;

	if (!m_bInGame)
		return false;

	int L = m_Position->m_iCellLetter;
	int N = m_Position->m_iCellNumber;

	res |= BeatsNL(board, N + 1, L + 2);
	res |= BeatsNL(board, N + 1, L - 2);
	res |= BeatsNL(board, N - 1, L + 2);
	res |= BeatsNL(board, N - 1, L - 2);
	res |= BeatsNL(board, N + 2, L + 1);
	res |= BeatsNL(board, N + 2, L - 1);
	res |= BeatsNL(board, N - 2, L + 1);
	res |= BeatsNL(board, N - 2, L - 1);

	return res;
}