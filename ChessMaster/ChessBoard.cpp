#include "stdafx.h"
#include <iostream>

#include "ChessBoard.h"

using namespace std;

CChessBoard::CChessBoard() {
	m_Queens.resize(2);
	m_pFigures.resize(32);
	m_pWhiteFigures.resize(16);
	m_pBlackFigures.resize(16);

	for (int i = 0; i < 16; i++) {
		if (i < 8) {
			m_Pawns[i] = CPawn(FIGURE_COLOR_WHITE);
			m_pWhiteFigures[i] = &m_Pawns[i];
		} else {
			m_Pawns[i] = CPawn(FIGURE_COLOR_BLACK);
			m_pBlackFigures[i - 8] = &m_Pawns[i];
		}
	}

	for (int i = 0; i < 4; i++) {
		if (i < 2)
			m_Rooks[i] = CRook(FIGURE_COLOR_WHITE);
		else
			m_Rooks[i] = CRook(FIGURE_COLOR_BLACK);
	}

	m_pWhiteFigures[8] = &m_Rooks[0];
	m_pWhiteFigures[9] = &m_Rooks[1];
	m_pBlackFigures[8] = &m_Rooks[2];
	m_pBlackFigures[9] = &m_Rooks[3];

	for (int i = 0; i < 4; i++) {
		if (i < 2)
			m_Knights[i] = CKnight(FIGURE_COLOR_WHITE);
		else
			m_Knights[i] = CKnight(FIGURE_COLOR_BLACK);
	}

	m_pWhiteFigures[10] = &m_Knights[0];
	m_pWhiteFigures[11] = &m_Knights[1];
	m_pBlackFigures[10] = &m_Knights[2];
	m_pBlackFigures[11] = &m_Knights[3];

	for (int i = 0; i < 4; i++) {
		if (i < 2)
			m_Bishops[i] = CBishop(FIGURE_COLOR_WHITE);
		else
			m_Bishops[i] = CBishop(FIGURE_COLOR_BLACK);
	}

	m_pWhiteFigures[12] = &m_Bishops[0];
	m_pWhiteFigures[13] = &m_Bishops[1];
	m_pBlackFigures[12] = &m_Bishops[2];
	m_pBlackFigures[13] = &m_Bishops[3];

	m_Queens[0] = CQueen(FIGURE_COLOR_WHITE);
	m_Queens[1] = CQueen(FIGURE_COLOR_BLACK);

	m_pWhiteFigures[14] = &m_Queens[0];
	m_pBlackFigures[14] = &m_Queens[1];

	m_Kings[0] = CKing(FIGURE_COLOR_WHITE);
	m_Kings[1] = CKing(FIGURE_COLOR_BLACK);

	m_pWhiteFigures[15] = &m_Kings[0];
	m_pBlackFigures[15] = &m_Kings[1];
}


CChessBoard::~CChessBoard() {}

void CChessBoard::InitBoard() {
	m_iResult = CHESS_GAME_RESULT_UNKNOWN;
	int color = CELL_COLOR_BLACK;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			m_Cells[i][j].m_iCellNumber = i;
			m_Cells[i][j].m_iCellLetter = j;
			m_Cells[i][j].m_iColor = color;
			color *= -1;
		}
		color *= -1;
	}

	for (int i = 0; i < 8; i++) {
		m_Cells[1][i].SetFigure(&m_Pawns[i]);
		m_Cells[6][i].SetFigure(&m_Pawns[i + 8]);
	}

	m_Cells[0][0].SetFigure(&m_Rooks[0]);
	m_Cells[0][7].SetFigure(&m_Rooks[1]);

	m_Cells[0][1].SetFigure(&m_Knights[0]);
	m_Cells[0][6].SetFigure(&m_Knights[1]);

	m_Cells[0][2].SetFigure(&m_Bishops[0]);
	m_Cells[0][5].SetFigure(&m_Bishops[1]);

	m_Cells[0][3].SetFigure(&m_Queens[0]);
	m_Cells[0][4].SetFigure(&m_Kings[0]);



	m_Cells[7][0].SetFigure(&m_Rooks[2]);
	m_Cells[7][7].SetFigure(&m_Rooks[3]);

	m_Cells[7][1].SetFigure(&m_Knights[2]);
	m_Cells[7][6].SetFigure(&m_Knights[3]);

	m_Cells[7][2].SetFigure(&m_Bishops[2]);
	m_Cells[7][5].SetFigure(&m_Bishops[3]);

	m_Cells[7][3].SetFigure(&m_Queens[1]);
	m_Cells[7][4].SetFigure(&m_Kings[1]);

	MakeCellsUnderAttack();
}

void CChessBoard::PrintBoard() {
	cout << endl;
	for (int i = 7; i > -1; i--) {
		cout << i + 1 << "   ";
		for (int j = 0; j < 8; j++) {
			m_Cells[i][j].Print();

			if (j == 7)
				cout << endl;
		}
	}
	cout << endl << "    " << "A   B   C   D   E   F   G   H  " << endl;
}

//патовая ситуация
bool CChessBoard::IsStalemate(int color) {

	bool res = false;

	if (FIGURE_COLOR_WHITE == color) {

		for (size_t i = 0; i < m_pWhiteFigures.size(); i++) {
			CFigure* pWhiteFigure = m_pWhiteFigures[i];
			res |= pWhiteFigure->Beats(this);
		}
	}
	else

		for (size_t i = 0; i < m_pBlackFigures.size(); i++) {
			CFigure* pBlackFigure = m_pBlackFigures[i];
			res |= pBlackFigure->Beats(this);
		}

	return !res;
}

bool CChessBoard::PerformMove(ChessMove M, bool& bEndGame) {
	if (M.pos_old->IsVoid())
		return false;

	if (M.pos_old->m_pFigure->m_iFigureColor != M.m_iColor)
		return false;


	if (!M.pos_new->IsVoid() && M.pos_new->m_pFigure->m_iFigureColor == M.m_iColor)
			return false;

	if (!M.pos_old->m_pFigure->PerformMove(this, &M))
		return false;

	//проверка шаха и мата

	MakeCellsUnderAttack();

	if (M.m_iColor == FIGURE_COLOR_BLACK) {

		if (m_Kings[0].IsChecked()) {
			if (m_Kings[0].IsCheckMated(this)) {
				cout << endl << "Мат белому королю!" << endl;
				bEndGame = false;
				m_iResult = CHESS_GAME_BLACK_WON;
				PrintBoard();
				return true;
			}
			cout << endl << "Шах белому королю!" << endl;
		}
	}
	else
		if (m_Kings[1].IsChecked()) {
			if (m_Kings[1].IsCheckMated(this)) {
				cout << endl << "Мат черному королю!" << endl;
				bEndGame = false;
				m_iResult = CHESS_GAME_WHITE_WON;
				PrintBoard();
				return true;
			}
			cout << endl << "Шах черному королю!" << endl;
		}

	if (M.m_iColor == FIGURE_COLOR_BLACK && !m_Kings[0].IsChecked()) {
		if (IsStalemate(FIGURE_COLOR_WHITE)) {
			cout << endl << "Пат при ходе белых!" << endl;
			bEndGame = false;
			m_iResult = CHESS_GAME_DRAW;
			return true;
		}
	}

	if (M.m_iColor == FIGURE_COLOR_WHITE && !m_Kings[1].IsChecked()) {
		if (IsStalemate(FIGURE_COLOR_BLACK)) {
			cout << endl << "Пат при ходе черных!" << endl;
			bEndGame = false;
			m_iResult = CHESS_GAME_DRAW;
			return true;
		}
	}

	return true;
}

void CChessBoard::MakeCellsUnderAttack() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			m_Cells[i][j].m_bUnderWhiteAttack = false;
			m_Cells[i][j].m_bUnderBlackAttack = false;
		}
	}

	for (size_t i = 0; i < m_pWhiteFigures.size(); i++) {
		CFigure* pWhiteFigure = m_pWhiteFigures[i];
		pWhiteFigure->Beats(this);
	}

	for (size_t i = 0; i < m_pWhiteFigures.size(); i++) {
		CFigure* pBlackFigure = m_pBlackFigures[i];
		pBlackFigure->Beats(this);
	}
}
