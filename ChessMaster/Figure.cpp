#include "stdafx.h"
#include "Figure.h"
#include "ChessBoard.h"


CFigure::CFigure() {}

CFigure::~CFigure() {}

bool CFigure::BeatsNL(CChessBoard* board, int N, int L) {
	if (0 <= N && N <= 7 && 0 <= L && L <= 7) {
		if (m_iFigureColor == FIGURE_COLOR_WHITE) {
			board->m_Cells[N][L].m_bUnderWhiteAttack = true;
			return board->m_Cells[N][L].IsVoid() || board->m_Cells[N][L].m_pFigure->m_iFigureColor == FIGURE_COLOR_BLACK;
		} else {
			board->m_Cells[N][L].m_bUnderBlackAttack = true;
			return board->m_Cells[N][L].IsVoid() || board->m_Cells[N][L].m_pFigure->m_iFigureColor == FIGURE_COLOR_WHITE;
		}
	}

	return false;
}
