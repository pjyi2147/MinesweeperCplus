// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <boost/algorithm/string.hpp>

#include "minesweeper.h"

void readScript(string* command, int* col, int* row, MineSweeper* m)
{
	
	cout << "Input command separated by space: ";
	std::vector<std::string> tokens;
	string str;
	getline(cin, str);
	boost::split(tokens, str, boost::is_any_of("\t "));
	/*
	while (!(cin >> *command) || *command != "D" || *command != "E" || *command != "F")
	{
		cout << "Invaild command" << endl;
		cout << "Input command: ";
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
	cout << "Input col: ";
	while (!(cin >> *col) || *col >= m.returnCol() || *col < 0)
	{
		cout << "Invalid command" << endl; 
		cout << "Input col: ";
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
	cout << "Input row: ";
	while (!(cin >> *row) || *row >= m.returnRow() || *row < 0)
	{
		cout << "Invalid command" << endl;
		cout << "Input row: ";
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
	*/
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
	cout << "Level 3: Expert (16x30, 10)" << endl;	
	cout << "Level 4: Custom" << endl;
	cout << "Input level: ";
	while (!(cin >> order) || order > 4)
	{
		cout << "Input level: ";
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}

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

	cin.ignore();
	readScript(&command, &col, &row, &minesweeper);
	
	minesweeper.createMinefield(col, row);

	while (!minesweeper.returnGameEnd())
	{
		system("cls");

		switch (order)
		{
		case 1:
			cout << endl << endl << "Beginner Mode" << endl << endl;
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

		minesweeper.printMineField();
		minesweeper.checkWin();
		if (minesweeper.returnGameEnd()) break;
		readScript(&command, &col, &row, &minesweeper);
		if (command == "E") 
		{
			minesweeper.revealTile(col, row);
		}
		if (command == "F") 
		{
			minesweeper.setFlag(col, row);
		}
		if (command == "D") 
		{ 
			minesweeper.revealDoubleClick(col, row);
		}
	}

	system("pause");

	return 0;
}

