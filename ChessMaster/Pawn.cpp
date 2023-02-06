#include "stdafx.h"
#include "Figures.h"
#include "ChessBoard.h"
#include <iostream>
#include <memory>

using namespace std;

CPawn::CPawn() {}

CPawn::~CPawn() {}

void CPawn::Print() {
	if (m_iFigureColor == FIGURE_COLOR_WHITE)
		cout << "WP  ";
	else
		cout << "BP  ";
}

bool CPawn::PerformMove(CChessBoard* b, ChessMove* M) {

	//что-то не так с ходом: данные не совпадают
	if (m_Position->m_iCellLetter != M->pos_old->m_iCellLetter || m_Position->m_iCellNumber != M->pos_old->m_iCellNumber)
		return false;


	int linedelta = M->pos_new->m_iCellNumber - M->pos_old->m_iCellNumber;
	int letterdelta = M->pos_new->m_iCellLetter - M->pos_old->m_iCellLetter;

	//ход больше чем на 2 клетки
	if (fabs(linedelta) > 2)
		return false;

	//	//ход на 2 клетки если уже ходили этой пешкой
	if ((fabs(linedelta) == 2 && !m_bInInitialPos))
		return false;

	//по диагонали только едим на 1 клетку
	if (fabs(letterdelta) > 1)
		return false;

	//нельзя ходить назад
	if (linedelta * M->pos_old->m_pFigure->m_iFigureColor < 0)
		return false;

	if (fabs(letterdelta) == 1) {
		//нельзя ходить вбок
		if (linedelta == 0)
			return false;

		//взятие пешки на проходе
		int N = M->pos_new->m_iCellNumber - m_iFigureColor;
		int L = M->pos_new->m_iCellLetter;

		if (!b->m_Cells[N][L].IsVoid()) {

			bool is_Pawn = false;
			int is_white = 0;

			if (m_iFigureColor == FIGURE_COLOR_WHITE) {
				is_white = 8;
			}

			for (size_t i = 0 + is_white; i < 8 + is_white; i++) {
				if (b->m_Pawns[i].m_bInGame && (&b->m_Pawns[i] == b->m_Cells[N][L].m_pFigure)) {
					is_Pawn = true;
					break;
				}
			}

			if (is_Pawn) {
				int L_start;
				if ('A' <= b->last_move[0] && b->last_move[0] <= 'H') {
					L_start = b->last_move[0] - 'A';
				} else {
					L_start = b->last_move[0] - 'a';
				}

				int L_end;
				if ('A' <= b->last_move[2] && b->last_move[0] <= 'H') {
					L_end = b->last_move[2] - 'A';
				} else {
					L_end = b->last_move[2] - 'a';
				}

				int N_start = b->last_move[1] - '1';
				int N_end = b->last_move[3] - '1';

				if (L_start == L_end && L_end == L) {
					if (N_start == N + 2 * m_iFigureColor && N_end == N) {

						CChessCell* cell_Pawn = &b->m_Cells[N][L];
						
						cell_Pawn->m_pFigure->m_bInGame = false;
						cell_Pawn->m_pFigure->m_Position = NULL;
						cell_Pawn->SetFigure(NULL);

						M->pos_old->SetFigure(NULL);;
						M->pos_new->SetFigure(this);

						return true;
					}
				}

			}
		}

		//едим
		if (M->pos_new->IsVoid())
			return false;

		M->pos_new->m_pFigure->m_bInGame = false;
		M->pos_new->m_pFigure->m_Position = NULL;

		M->pos_old->SetFigure(NULL);
		M->pos_new->SetFigure(this);

	} else if (!M->pos_new->IsVoid()) {
		return false;
	} else {
		M->pos_old->SetFigure(NULL);
		M->pos_new->SetFigure(this);
	}

	if (M->pos_new->m_iCellNumber == 0 || M->pos_new->m_iCellNumber == 7) {
		m_Position = NULL;
		m_bInGame = false;


		b->m_Queens.push_back(CQueen(m_iFigureColor));

		CQueen* p_Queen = &b->m_Queens[b->m_Queens.size() - 1];

		if (m_iFigureColor == FIGURE_COLOR_WHITE) {
			b->m_pWhiteFigures.push_back(p_Queen);
		} else {
			b->m_pBlackFigures.push_back(p_Queen);
		}

		b->m_pFigures.push_back(p_Queen);

		M->pos_new->SetFigure(p_Queen);
		M->pos_new->m_pFigure->m_bInInitialPos = false;
		M->pos_new->m_pFigure->m_Position = M->pos_new;
		M->pos_new->m_pFigure->m_iFigureColor = m_iFigureColor;
	}

	m_bInInitialPos = false;

	return true;
}

bool CPawn::Beats(CChessBoard* board) {

	bool res = false;

	if (!m_bInGame)
		return false;

	int Letter = m_Position->m_iCellLetter;
	int number = m_Position->m_iCellNumber;

	int N = number + m_iFigureColor, L = Letter;


	res |= BeatsNL(board, N, L + 1);
	res |= BeatsNL(board, N, L - 1);

	return res;
}
