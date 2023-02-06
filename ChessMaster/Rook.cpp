#include "stdafx.h"
#include "Figures.h"
#include "ChessBoard.h"
#include <iostream>

using namespace std;

CRook::CRook() {}


CRook::~CRook() {}

void CRook::Print() {
	if (m_iFigureColor == FIGURE_COLOR_WHITE)
		cout << "WR  ";
	else
		cout << "BR  ";
}

bool CRook::PerformMove(CChessBoard* b, ChessMove* M) {
	//что-то не так с ходом: данные не совпадают
	if (m_Position->m_iCellLetter != M->pos_old->m_iCellLetter || m_Position->m_iCellNumber != M->pos_old->m_iCellNumber)
		return false;


	int linedelta = M->pos_new->m_iCellNumber - M->pos_old->m_iCellNumber;
	int letterdelta = M->pos_new->m_iCellLetter - M->pos_old->m_iCellLetter;

	//ход больше чем может ладья
	if (fabs(linedelta) != 0 && fabs(letterdelta) != 0)
		return false;

	if (linedelta != 0)
		linedelta /= abs(linedelta);

	if (letterdelta != 0)
		letterdelta /= abs(letterdelta);

	for (int N = M->pos_old->m_iCellNumber + linedelta, L = M->pos_old->m_iCellLetter + letterdelta; N > -1, N<8, L>-1, L < 8; N += linedelta, L += letterdelta) {
		//клетка назначения
		if (N == M->pos_new->m_iCellNumber && L == M->pos_new->m_iCellLetter) {
			if (!b->m_Cells[N][L].IsVoid()) {
				M->pos_new->m_pFigure->m_bInGame = false;
				M->pos_new->m_pFigure->m_Position = NULL;

			}
			//ходим
			M->pos_old->SetFigure(NULL);
			M->pos_new->SetFigure(this);
			break;
		}

		if (!b->m_Cells[N][L].IsVoid())
			return false;
	}

	m_bInInitialPos = false;
	return true;


}

bool CRook::Beats(CChessBoard* board) {

	bool res = false;

	if (!m_bInGame)
		return false;

	int Letter = m_Position->m_iCellLetter;
	int number = m_Position->m_iCellNumber;

	//+N
	int L = Letter;
	for (int N = number + 1; N < 8; N++) {
		res |= BeatsNL(board, N, L);
		if (!board->m_Cells[N][L].IsVoid())
			break;
	}

	//-N
	for (int N = number - 1; N > -1; N--) {
		res |= BeatsNL(board, N, L);
		if (!board->m_Cells[N][L].IsVoid())
			break;
	}

	//+L
	int N = number;
	for (int L = Letter + 1; L < 8; L++) {
		res |= BeatsNL(board, N, L);
		if (!board->m_Cells[N][L].IsVoid())
			break;
	}

	//-L
	for (int L = Letter - 1; L > -1; L--) {
		res |= BeatsNL(board, N, L);
		if (!board->m_Cells[N][L].IsVoid())
			break;
	}

	return res;
}
