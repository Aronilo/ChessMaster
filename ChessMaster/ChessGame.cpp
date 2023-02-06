#include "stdafx.h"
#include "windows.h"
#include <locale>
#include <iostream>

#include "ChessGame.h"

HANDLE hStdInPipeRead = NULL;
HANDLE hStdInPipeWrite = NULL;
HANDLE hStdOutPipeRead = NULL;
HANDLE hStdOutPipeWrite = NULL;

string bestmove = "";
HANDLE hBestMove;

using namespace std;

CChessGame::CChessGame() {
	m_iCurrentMove = CHESS_GAME_MOVE_WHITE;
}

CChessGame::~CChessGame() {}

int CChessGame::DoPlay() {
	setlocale(LC_ALL, "RUS");

	bool res = false;

	do {
		cout << "Шахматы" << endl;

		cout << "Выберите вариант игры" << endl;
		cout << "1 - компьютер играет за черных" << endl;
		cout << "2 - компьютер играет за белых" << endl;
		cout << "3 - компьютер не участвует в игре" << endl;
		cout << "4 - играет только компьютер" << endl;

		int choise = -1;

		cin >> choise;
		
		while (cin.fail() || choise < 1 || choise > 4) {
			cout << "Некорректный выбор. Введите число от 1 до 4." << endl;
			cin.clear();
			cin.ignore(256, '\n');
			cin >> choise;
		}

		if (1 <= choise && choise <= 3) {
			cout << "Игрок может запросить подсказку командой help" << endl;
			cout << "Формат хода: A1A2 или a1a2" << endl;
		}

		switch (choise) {
		case 1:
			m_BlackPlayer.m_bComputer = true;
			res = true;
			break;
		case 2:
			m_WhitePlayer.m_bComputer = true;
			res = true;
			break;
		case 3:
			res = true;
			break;
		case 4:
			m_BlackPlayer.m_bComputer = true;
			m_WhitePlayer.m_bComputer = true;
			res = true;
			break;
		default:
			cout << "Ошибочный выбор!" << endl;
		}

	} while (!res);

	//if (m_WhitePlayer.m_bComputer || m_BlackPlayer.m_bComputer) {
		StartUCI();
	//}

	m_bInGame = true;
	m_ChessBoard.InitBoard();

	Intro();
	
	while (m_bInGame) {
		m_ChessBoard.PrintBoard();

		if (m_iCurrentMove == CHESS_GAME_MOVE_WHITE) {
			if (!WhitesMove()) {
				cout << "\nНевозможный ход!\nВведите ход в формате A1A2 или a1a2!\n";

				continue;
			}

			m_iCurrentMove *= -1;
			continue;
		}

		if (m_iCurrentMove == CHESS_GAME_MOVE_BLACK) {
			if (!BlackesMove()) {
				cout << "\nНевозможный ход!\nВведите ход в формате A1A2 или a1a2!\n";

				continue;
			}

			m_iCurrentMove *= -1;
			continue;
		}
	}

	bThreadAlive = false;
	return m_ChessBoard.GetGameResult();
}

void CChessGame::Intro() {
	cout << "\nИгра начинается!" << endl;
}

bool CChessGame::WhitesMove() {
	if (!m_WhitePlayer.m_bComputer)
		cout << endl << "Ход белых!" << endl;
	ChessMove WMove;

	if (!CaptureMove(WMove, m_WhitePlayer.m_bComputer))
		return false;

	WMove.m_iColor = CHESS_GAME_MOVE_WHITE;

	CFigure* beatedFigure = WMove.pos_new->m_pFigure;

	if (moves.size() >= 2)
		m_ChessBoard.last_move = moves[moves.size() - 2];

	if (!m_ChessBoard.PerformMove(WMove, m_bInGame)) {
		moves.pop_back();
		return false;
	}

	//после хода белых их король не должен быть под шахом
	if (m_ChessBoard.m_Kings[0].IsChecked()) {
		CFigure* fg = WMove.pos_new->m_pFigure;
		WMove.pos_old->SetFigure(fg);
		WMove.pos_new->SetFigure(beatedFigure);

		if (beatedFigure) {
			beatedFigure->m_bInGame =true;
		}

		m_ChessBoard.MakeCellsUnderAttack();

		moves.pop_back();
		return false;
	}

	return true;
}

bool CChessGame::BlackesMove() {
	if (!m_BlackPlayer.m_bComputer)
		cout << endl << "Ход черных!" << endl;

	ChessMove BMove;
	if (!CaptureMove(BMove, m_BlackPlayer.m_bComputer))
		return false;

	BMove.m_iColor = CHESS_GAME_MOVE_BLACK;

	CFigure* beatedFigure = BMove.pos_new->m_pFigure;

	if (moves.size() >= 2)
		m_ChessBoard.last_move = moves[moves.size() - 2];

	if (!m_ChessBoard.PerformMove(BMove, m_bInGame)) {
		moves.pop_back();
		return false;
	}

	//после хода белых их король не должен быть под шахом
	if (m_ChessBoard.m_Kings[1].IsChecked()) {
		CFigure* fg = BMove.pos_new->m_pFigure;
		BMove.pos_old->SetFigure(fg);
		BMove.pos_new->SetFigure(beatedFigure);

		if (beatedFigure) {
			beatedFigure->m_bInGame = true;
		}

		m_ChessBoard.MakeCellsUnderAttack();

		moves.pop_back();
		return false;
	}

	return true;
}
bool CChessGame::CaptureMove(ChessMove& Move, bool bComp) {
	char oldPosLetter;
	char newPosLetter;

	int oldPosNumber;
	int newPosNumber;
	string buf, answer;

	if (bComp) {
		SendCommand(GetMovesString());

		Sleep(1000);

		SendCommand("go movetime 2000");

		ResetEvent(hBestMove);

		WaitForSingleObject(hBestMove, INFINITE);

		buf = bestmove;
	} else {
		cout << "> ";
		cin >> buf;

		while (buf == "help") {
			SendCommand(GetMovesString());

			Sleep(1000);

			SendCommand("go movetime 2000");


			ResetEvent(hBestMove);

			WaitForSingleObject(hBestMove, INFINITE);

			answer = bestmove;

			cout << "Хороший ход по мнению компьютера: " << answer << endl;

			cout << "> ";
			cin >> buf;
		}

	}

	if (sscanf(buf.c_str(), "%c%i%c%i", &oldPosLetter, &oldPosNumber, &newPosLetter, &newPosNumber) != 4) {
		return false;
	}


	int num_old = -1;

	if (oldPosLetter == 'a' || oldPosLetter == 'A') {
		num_old = CELL_LETTER_A;

	} else if (oldPosLetter == 'b' || oldPosLetter == 'B') {
		num_old = CELL_LETTER_B;
	} else if (oldPosLetter == 'c' || oldPosLetter == 'C') {
		num_old = CELL_LETTER_C;
	} else if (oldPosLetter == 'd' || oldPosLetter == 'D') {
		num_old = CELL_LETTER_D;
	} else if (oldPosLetter == 'e' || oldPosLetter == 'E') {
		num_old = CELL_LETTER_E;
	} else if (oldPosLetter == 'f' || oldPosLetter == 'F') {
		num_old = CELL_LETTER_F;
	} else if (oldPosLetter == 'g' || oldPosLetter == 'G') {
		num_old = CELL_LETTER_G;
	} else if (oldPosLetter == 'h' || oldPosLetter == 'H') {
		num_old = CELL_LETTER_H;
	} else {
		return false;
	}

	int num_new = -1;

	if (newPosLetter == 'a' || newPosLetter == 'A') {
		num_new = CELL_LETTER_A;

	} else if (newPosLetter == 'b' || newPosLetter == 'B') {
		num_new = CELL_LETTER_B;
	} else if (newPosLetter == 'c' || newPosLetter == 'C') {
		num_new = CELL_LETTER_C;
	} else if (newPosLetter == 'd' || newPosLetter == 'D') {
		num_new = CELL_LETTER_D;
	} else if (newPosLetter == 'e' || newPosLetter == 'E') {
		num_new = CELL_LETTER_E;
	} else if (newPosLetter == 'f' || newPosLetter == 'F') {
		num_new = CELL_LETTER_F;
	} else if (newPosLetter == 'g' || newPosLetter == 'G') {
		num_new = CELL_LETTER_G;
	} else if (newPosLetter == 'h' || newPosLetter == 'H') {
		num_new = CELL_LETTER_H;
	} else {
		return false;
	}


	Move.pos_old = &m_ChessBoard.m_Cells[oldPosNumber - 1][num_old];
	Move.pos_new = &m_ChessBoard.m_Cells[newPosNumber - 1][num_new];

	if (Move.pos_old->IsValid() && Move.pos_new->IsValid()) {
		moves.push_back(buf);

		if (bComp)
			cout << "\nХод компьютера: " << buf << endl;

		return true;
	}

	return false;
}

string CChessGame::GetMovesString() {
	string s = "position startpos moves ";

	for (size_t i = 0; i < moves.size(); i++) {
		s += moves[i];
		s += " ";
	}
	s += "\n";
	return s;
}

void CChessGame::SendCommand(string str) {
	str += "\n";
	WriteFile(hStdInPipeWrite, str.c_str(), str.length(), NULL, NULL);
}

string CChessGame::ParseBestMove(string read, bool& ok) {
	ok = false;
	int res = read.find("bestmove");
	if (res > -1) ok = true;

	string move = read.substr(res + 9, 4);

	return move;
}

DWORD WINAPI ReadProc(LPVOID params) {
	DWORD dwRead = 0;
	DWORD dwAvail = 0;
	char buf[1024];
	string str;

	CChessGame* game = (CChessGame*)params;

	while (game->bThreadAlive) {
		PeekNamedPipe(hStdOutPipeRead, buf, 1024, &dwRead, &dwAvail, NULL);

		if (dwRead) {
			char* buf = new char[dwAvail];

			ReadFile(hStdOutPipeRead, buf, dwAvail, &dwRead, NULL);

			string str(buf);
			delete buf;

			bool res = false;
			bestmove = game->ParseBestMove(str, res);

			if (res) {
				SetEvent(hBestMove);
			}
			Sleep(100);
		}

	}
	return 0;
}

bool CChessGame::StartUCI() {

	cout << "Выберите сложность игры компьютера (1-10):" << endl;

	int level;

	cout << "> ";
	cin >> level;

	while (cin.fail() || level < 1 || level > 10) {
		cout << "Некорректный ввод. Повторите попытку!" << endl;
		cin.clear();
		cin.ignore(256, '\n');
		cout << "> ";
		cin >> level;
	}

	level *= 400;

	bThreadAlive = true;

	hBestMove = CreateEvent(NULL, TRUE, FALSE, L"BestMove");

	DWORD pReadProc;

	CreateThread(NULL, 0, ReadProc, this, 0, &pReadProc);


	BOOL ok = TRUE;

	// Create two pipes.
	SECURITY_ATTRIBUTES sa = { sizeof(SECURITY_ATTRIBUTES), NULL, TRUE };
	ok = CreatePipe(&hStdInPipeRead, &hStdInPipeWrite, &sa, 0);
	if (ok == FALSE) return false;
	ok = CreatePipe(&hStdOutPipeRead, &hStdOutPipeWrite, &sa, 0);
	if (ok == FALSE) return false;

	// Create the process.
	STARTUPINFO si = { };
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESTDHANDLES;
	si.hStdError = hStdOutPipeWrite;
	si.hStdOutput = hStdOutPipeWrite;
	si.hStdInput = hStdInPipeRead;
	PROCESS_INFORMATION pi = { };
	LPCWSTR lpApplicationName = L"stockfish.exe";
	LPWSTR lpCommandLine = (LPWSTR)L"stockfish.exe";
	LPSECURITY_ATTRIBUTES lpProcessAttributes = NULL;
	LPSECURITY_ATTRIBUTES lpThreadAttribute = NULL;
	BOOL bInheritHandles = TRUE;
	DWORD dwCreationFlags = 0;
	LPVOID lpEnvironment = NULL;
	LPCWSTR lpCurrentDirectory = NULL;
	ok = CreateProcess(
		lpApplicationName,
		lpCommandLine,
		lpProcessAttributes,
		lpThreadAttribute,
		bInheritHandles,
		dwCreationFlags,
		lpEnvironment,
		lpCurrentDirectory,
		&si,
		&pi);
	if (ok == FALSE) return false;


	CloseHandle(hStdOutPipeWrite);
	CloseHandle(hStdInPipeRead);

	SendCommand("uci");

	SendCommand("setoption name Hash value 128");
	SendCommand("setoption name UCI_LimitStrength value true");
	SendCommand("setoption name UCI_Elo value " + to_string(level));

	SendCommand("isready");
	SendCommand("ucinewgame");

	return true;
}
