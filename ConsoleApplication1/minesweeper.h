#pragma once
#include <iostream>
#include <vector>

#include "tile.h"

using namespace std;

class MineSweeper
{
private:
	int _col;
	int _row;
	int _mineNum;
	Tile** _minefield;

	bool _gameEnd = false;

public:
	MineSweeper(int col, int row, int mineNum);
	~MineSweeper();

	void countMine(int col, int row);
	int countFlag(int col, int row);

	void createMinefield(int col, int row);
	void printMineField();
	
	void revealTile(int col, int row);
	void revealDoubleClick(int col, int row);

	void EndGame(bool win);
	void checkWin();

	void setGameEnd(bool finished);
	void setFlag(int col, int row);

	bool returnGameEnd();
	int returnCol();
	int returnRow();
	int returnMineNum();
};