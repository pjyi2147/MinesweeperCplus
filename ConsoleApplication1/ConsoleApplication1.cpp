#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>

#include "minesweeper.h"
#include "server.h"
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

void GameLoop(int order, MineSweeper* m)
{
	while (!m->returnGameEnd())
	{
		string command;
		json orders;
		int col, row;
		// clear console
		system("cls"); 
		// mode print
		switch (order)
		{
		case 1:
			cout << endl << "Beginner Mode" << endl << endl;
			break;
		case 2:
			cout << endl << endl << "Intermediate Mode" << endl << endl;
			break;
		case 3:
			cout << endl << endl << "Expert Mode" << endl << endl;
			break;
		case 4:
			cout << endl << endl << "Custom Mode" << endl << endl;
		}
		// print minefield
		m->printMineField();

		// check win
		m->checkWin();
		// if game ended somehow break
		if (m->returnGameEnd()) {
			ingameTransfer(&orders, m);
			break;
		}
		// read command
		if (!m->returnGameEnd() &&
			!m->returnWin())
			ingameTransfer(&orders, m);
	}
}

int main()
{
	int col, row, mineNum, order;
	string command;
	string orders;

	vector<string> k;
	k.push_back("any string");
	cout << "Level 1: Beginner (9x9, 10)" << endl;
	cout << "Level 2: Intermediate (16x16, 40)" << endl;
	cout << "Level 3: Expert (30x16, 10)" << endl;
	cout << "Level 4: Custom" << endl;
	startGamefunction(order);

	switch (order)
	{
	case 1:
		col = 9, row = 9, mineNum = 10;
		cout << "Beginner Mode" << endl << endl;
		break;
	case 2:
		col = 16, row = 16, mineNum = 40;
		cout << "Intermediate Mode" << endl << endl;
		break;
	case 3:
		col = 30, row = 16, mineNum = 99;
		cout << "Expert Mode" << endl << endl;
		break;
	case 4:
		cout << "Custom Mode" << endl << endl;
		cout << "Input col: ";
		cin >> col;
		cout << "Input row: ";
		cin >> row;
		cout << "Input mineNum: ";
		cin >> mineNum;
		if (mineNum > col * row - 1) {
			cout << "Error: mineNum too big" << endl;
			cout << "Input mineNum: ";

			cin >> mineNum;
		}
	}

	auto minesweeper = MineSweeper(col, row, mineNum);

	cout << "Minefield is generated after the first input" << endl;
	cout << "command E: open the tile" << endl;
	cout << "command F: Flag the tile" << endl;
	cout << "command D: Double cilck a known tile" << endl;
	cout << "for the first input, it is always command E" << endl;

	// transfer first script;
	firstScriptTransfer(col, row, command, &minesweeper);
	// loop until the game ends
	GameLoop(order, &minesweeper);

	minesweeper.EndGame(minesweeper.returnWin());

	// system("pause");

	return 0;
}
