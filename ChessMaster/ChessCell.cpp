#pragma once

#include "stdafx.h"
#include <iostream>

#include "ChessBoard.h"

using namespace std;


void CChessCell::Print() {

	if (IsVoid()) {
		if (m_iColor == CELL_COLOR_WHITE)
			cout << "W   ";
		else
			cout << "B   ";
	} else
		m_pFigure->Print();
}

void CChessCell::SetFigure(CFigure* pFigure) {
	m_pFigure = pFigure;

	if (pFigure)
		pFigure->m_Position = this;
}
