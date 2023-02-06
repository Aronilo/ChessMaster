#include "stdafx.h"
#include "Figures.h"
#include "ChessBoard.h"
#include <iostream>


using namespace std;

CKing::CKing() {}

CKing::~CKing() {}

void CKing::Print() {
	if (m_iFigureColor == FIGURE_COLOR_WHITE)
		cout << "WK  ";
	else
		cout << "BK  ";
}

bool CKing::Castling(CChessBoard* b, ChessMove* M) {
	int linedelta = M->pos_new->m_iCellNumber - M->pos_old->m_iCellNumber;
	int letterdelta = M->pos_new->m_iCellLetter - M->pos_old->m_iCellLetter;

	//смещение короля не на 2 влево или вправо
	if (fabs(linedelta) != 0 || fabs(letterdelta) != 2)
		return false;

	if (!m_bInInitialPos)
		return false;

	int i_old = M->pos_old->m_iCellNumber;
	int j_old = M->pos_old->m_iCellLetter;

	int j_new = j_old + letterdelta;

	//где находится ладья номер столбика
	int j_rook_old = j_old + 3;
	int shift = 1;

	if (letterdelta == -2) {
		j_rook_old = j_old - 4;
		shift = -1;
	}

	if (b->m_Cells[i_old][j_rook_old].IsVoid())
		return false;

	if (!b->m_Cells[i_old][j_rook_old].m_pFigure->m_bInInitialPos)
		return false;

	int p = 0;

	for (int i = 0; i < 3; i++) {

		CChessCell* p_Cell = &b->m_Cells[i_old][j_old + p];
		cout << p_Cell->m_bUnderBlackAttack << endl;
		if (m_iFigureColor == FIGURE_COLOR_WHITE && p_Cell->m_bUnderBlackAttack ||
			m_iFigureColor != FIGURE_COLOR_WHITE && p_Cell->m_bUnderWhiteAttack)
			return false;
		p += shift;
	}

	p = 1;

	for (int i = 0; i < 2; i++) {

		CChessCell* p_Cell = &b->m_Cells[i_old][j_old + p];
		if (!p_Cell->IsVoid())
			return false;
		p += shift;
	}

	if (letterdelta == -2) {
		CChessCell* p_Cell = &b->m_Cells[i_old][j_old - 3];
		if (!p_Cell->IsVoid())
			return false;
		if (m_iFigureColor == FIGURE_COLOR_WHITE && p_Cell->m_bUnderBlackAttack ||
			m_iFigureColor != FIGURE_COLOR_WHITE && p_Cell->m_bUnderWhiteAttack)
			return false;
	}

	//ходим
	M->pos_old->SetFigure(NULL);
	M->pos_new->SetFigure(this);

	b->m_Cells[i_old][M->pos_new->m_iCellLetter - letterdelta / abs(letterdelta)].SetFigure(b->m_Cells[i_old][j_rook_old].m_pFigure);
	b->m_Cells[i_old][j_rook_old].m_pFigure->m_bInInitialPos = false;
	b->m_Cells[i_old][j_rook_old].SetFigure(NULL);

	m_bInInitialPos = false;

	return true;
}


bool CKing::PerformMove(CChessBoard* b, ChessMove* M) {
	//что-то не так с ходом: данные не совпадают
	if (m_Position->m_iCellLetter != M->pos_old->m_iCellLetter || m_Position->m_iCellNumber != M->pos_old->m_iCellNumber)
		return false;

	if (Castling(b, M))
		return true;

	int linedelta = M->pos_new->m_iCellNumber - M->pos_old->m_iCellNumber;
	int letterdelta = M->pos_new->m_iCellLetter - M->pos_old->m_iCellLetter;

	//ход больше чем может король
	if (fabs(linedelta) > 1 || fabs(letterdelta) > 1)
		return false;

	int N = M->pos_new->m_iCellNumber;
	int L = M->pos_new->m_iCellLetter;

	if (m_iFigureColor == FIGURE_COLOR_WHITE && b->m_Cells[N][L].m_bUnderBlackAttack)
		return false;

	if (m_iFigureColor == FIGURE_COLOR_BLACK && b->m_Cells[N][L].m_bUnderWhiteAttack)
		return false;

	//клетка назначения
	if (!b->m_Cells[N][L].IsVoid()) {
		if (m_iFigureColor == FIGURE_COLOR_WHITE && b->m_Cells[N][L].m_pFigure->m_iFigureColor == FIGURE_COLOR_WHITE)
			return false;

		if (m_iFigureColor == FIGURE_COLOR_BLACK && b->m_Cells[N][L].m_pFigure->m_iFigureColor == FIGURE_COLOR_BLACK)
			return false;

		M->pos_new->m_pFigure->m_bInGame = false;
		M->pos_new->m_pFigure->m_Position = NULL;
	}
	//ходим
	M->pos_old->SetFigure(NULL);
	M->pos_new->SetFigure(this);

	m_bInInitialPos = false;
	return true;


}

bool CKing::Beats(CChessBoard* board) {

	bool res = false;

	if (!m_bInGame)
		return false;

	int Letter = m_Position->m_iCellLetter;
	int number = m_Position->m_iCellNumber;

	res |= BeatsNL(board, number + 1, Letter + 1);
	res |= BeatsNL(board, number + 1, Letter);
	res |= BeatsNL(board, number + 1, Letter - 1);

	res |= BeatsNL(board, number, Letter + 1);
	res |= BeatsNL(board, number, Letter - 1);

	res |= BeatsNL(board, number - 1, Letter + 1);
	res |= BeatsNL(board, number - 1, Letter);
	res |= BeatsNL(board, number - 1, Letter - 1);

	return res;
}

bool CKing::IsChecked() {
	if (m_iFigureColor == FIGURE_COLOR_WHITE)
		return m_Position->m_bUnderBlackAttack;
	else
		return m_Position->m_bUnderWhiteAttack;
}


bool CKing::IsCheckMated(CChessBoard* b) {
	int N = m_Position->m_iCellNumber;
	int L = m_Position->m_iCellLetter;


	if (m_iFigureColor == FIGURE_COLOR_WHITE) {

		if (TestCellForWhiteKing(b, N + 1, L)
			|| TestCellForWhiteKing(b, N + 1, L + 1)
			|| TestCellForWhiteKing(b, N, L + 1)
			|| TestCellForWhiteKing(b, N - 1, L + 1)
			|| TestCellForWhiteKing(b, N - 1, L)
			|| TestCellForWhiteKing(b, N - 1, L - 1)
			|| TestCellForWhiteKing(b, N, L - 1)
			|| TestCellForWhiteKing(b, N + 1, L - 1)) {

			return false;

		}
	}
	if (m_iFigureColor == FIGURE_COLOR_BLACK) {

		if (TestCellForBlackKing(b, N + 1, L)
			|| TestCellForBlackKing(b, N + 1, L + 1)
			|| TestCellForBlackKing(b, N, L + 1)
			|| TestCellForBlackKing(b, N - 1, L + 1)
			|| TestCellForBlackKing(b, N - 1, L)
			|| TestCellForBlackKing(b, N - 1, L - 1)
			|| TestCellForBlackKing(b, N, L - 1)
			|| TestCellForBlackKing(b, N + 1, L - 1)) {

			return false;

		}
	}

	return true;
}

bool CKing::TestCellForWhiteKing(CChessBoard* b, int N, int L) {
	if (N > 7 || N < 0 || L > 7 || L < 0)
		return false;

	if (!b->m_Cells[N][L].m_bUnderBlackAttack && b->m_Cells[N][L].IsVoid())
		return true;

	if (!b->m_Cells[N][L].m_bUnderBlackAttack && b->m_Cells[N][L].m_pFigure->m_iFigureColor == FIGURE_COLOR_BLACK)
		return true;

	return false;
}

bool CKing::TestCellForBlackKing(CChessBoard* b, int N, int L) {
	if (N > 7 || N < 0 || L > 7 || L < 0)
		return false;

	if (!b->m_Cells[N][L].m_bUnderWhiteAttack && b->m_Cells[N][L].IsVoid())
		return true;

	if (!b->m_Cells[N][L].m_bUnderWhiteAttack && b->m_Cells[N][L].m_pFigure->m_iFigureColor == FIGURE_COLOR_WHITE)
		return true;

	return false;
}