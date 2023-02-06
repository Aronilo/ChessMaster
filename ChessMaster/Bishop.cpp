#include "stdafx.h"
#include <iostream>

#include "Figures.h"
#include "ChessBoard.h"

using namespace std;

CBishop::CBishop() {}


CBishop::~CBishop() {}

void CBishop::Print() {
	if (m_iFigureColor == FIGURE_COLOR_WHITE)
		cout << "WB  ";
	else
		cout << "BB  ";
}

bool CBishop::PerformMove(CChessBoard* b, ChessMove* M) {
	//что-то не так с ходом: данные не совпадают
	if (m_Position->m_iCellLetter != M->pos_old->m_iCellLetter || m_Position->m_iCellNumber != M->pos_old->m_iCellNumber)
		return false;


	int linedelta = M->pos_new->m_iCellNumber - M->pos_old->m_iCellNumber;
	int letterdelta = M->pos_new->m_iCellLetter - M->pos_old->m_iCellLetter;

	//ход не ао диагонали
	if (fabs(linedelta) != fabs(letterdelta))
		return false;

	linedelta /= abs(linedelta);
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

bool CBishop::Beats(CChessBoard* board) {

	bool res = false;

	if (!m_bInGame)
		return false;

	int Letter = m_Position->m_iCellLetter;
	int number = m_Position->m_iCellNumber;

	//++
	for (int N = number + 1, L = Letter + 1; N < 8 && L < 8; N++, L++) {
		res |= BeatsNL(board, N, L);
		if (!board->m_Cells[N][L].IsVoid())
			break;
	}

	//+-
	for (int N = number + 1, L = Letter - 1; N < 8 && L>-1; N++, L--) {
		res |= BeatsNL(board, N, L);
		if (!board->m_Cells[N][L].IsVoid())
			break;
	}

	//--
	for (int N = number - 1, L = Letter - 1; N > -1 && L > -1; N--, L--) {
		res |= BeatsNL(board, N, L);
		if (!board->m_Cells[N][L].IsVoid())
			break;
	}

	//-+
	for (int N = number - 1, L = Letter + 1; N > -1 && L < 8; N--, L++) {
		res |= BeatsNL(board, N, L);
		if (!board->m_Cells[N][L].IsVoid())
			break;
	}

	return res;
}