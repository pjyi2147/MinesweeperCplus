#include <iostream>
#include <vector>
#include "tile.h"
#include "minesweeper.h"
#include <ctime>


MineSweeper::MineSweeper(int col, int row, int mineNum)
{
	this->_mineNum = mineNum;
	this->_col = col;
	this->_row = row;
	
	this->_minefield = new Tile*[row];
	for (int i = 0; i < col; ++i)
	{
		this->_minefield[i] = new Tile[col];
	}

	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < col; ++j)
		{
			this->_minefield[i][j] = Tile(j, i);
		}
	}
}

MineSweeper::~MineSweeper()
{
	for (int i = 0; i < this->returnRow(); ++i)
	{
		delete[] this->_minefield[i];
	}
	delete[] this->_minefield;
}


void MineSweeper::createMinefield(int col, int row)
{
	vector<pair<int, int>> options;

	for (int i = 0; i < this->returnRow(); i++) 
	{
		for (int j = 0; j < this->returnCol(); j++) 
		{
			if (!(i == row && j == col))
			{
				options.push_back(make_pair(i, j));
			}
		}
	}
		
	for (int i = 0; i < this->returnMineNum(); ++i)
	{
		if (options.size() > 0)
		{	
			srand(time(NULL));
			int k = (rand() % options.size());
			auto pair1 = options[k];
			this->_minefield[pair1.first][pair1.second].setMine();
			options.erase(options.begin() + k);;
		}
	}

	for (int i = 0; i < this->returnRow(); i++)
	{
		for (int j = 0; j < this->returnCol(); j++)
		{
			this->countMine(j, i);
		}
	}
	this->revealTile(col, row);
}

void MineSweeper::checkWin()
{
	bool win = true;
	for (int i = 0; i < this->returnRow(); ++i)
	{
		for (int j = 0; j < this->returnCol(); ++j)
		{
			if (!this->_minefield[j][i].isRevealed() && !this->_minefield[j][i].isMine())
				win = false;
		}
	}
	if (win)
	{
		this->setGameEnd(true);
		this->EndGame(true);
	}
}

void MineSweeper::printMineField()
{
	for (int j = 0; j < this->_col * 2 + 1; ++j)
	{
		cout << "-";
	}
	cout << endl;
	for (int i = 0; i < this->_row; ++i)
	{
		for (int j = 0; j < this->_col; ++j)
		{
			this->_minefield[i][j].printTile();
		}
		cout << "|" << endl;
		for (int j = 0; j < this->_col * 2 + 1; ++j)
		{
			cout << "-";
		}
		cout << endl;
	}
}

void MineSweeper::revealTile(int col, int row)
{
	
	this->_minefield[row][col].reveal();
	if (this->_minefield[row][col].isMine())
	{
		this->setGameEnd(true);
		this->EndGame(false);
	}
	if (this->_minefield[row][col].returnNeighborCount() == 0 && !this->_minefield[row][col].isMine())
	{
		for (int yoff = -1; yoff <= 1; ++yoff)
		{
			for (int xoff = -1; xoff <= 1; ++xoff)
			{
				int i = this->_minefield[row][col].returnCol() + xoff;
				int j = this->_minefield[row][col].returnRow() + yoff;

				if (i > -1 && i < this->returnCol() && j > -1 && j < this->returnRow())
				{
					if (!this->_minefield[j][i].isRevealed()) this->revealTile(i, j);
				}
			}
		}
	}
}

void MineSweeper::revealDoubleClick(int col, int row)
{
	auto tile = this->_minefield[row][col];
	if (!tile.isRevealed()) return;

	if (this->countFlag(col, row) != tile.returnNeighborCount()) return;

	for (int yoff = -1; yoff <= 1; ++yoff)
	{
		for (int xoff = -1; xoff <= 1; ++xoff)
		{
			if (xoff != 0 || yoff != 0)
			{
				int i = tile.returnCol() + xoff;
				int j = tile.returnRow() + yoff;

				if (i > -1 && i < this->returnCol() && j > -1 && j < this->returnRow())
				{
					if (!this->_minefield[j][i].isFlagged())
						this->revealTile(i, j);
				}
			}
		}
	}
}


void MineSweeper::countMine(int col, int row)
{
	if (this->_minefield[row][col].isMine())
	{
		this->_minefield[row][col].setNeighborCount(-1);
		return;
	}

	int total = 0;
	for (int yoff = -1; yoff <= 1; ++yoff)
	{
		for (int xoff = -1; xoff <= 1; ++xoff)
		{
			int i = this->_minefield[row][col].returnCol() + xoff;
			int j = this->_minefield[row][col].returnRow() + yoff;

			if (i > -1 && i < this->returnCol() && j > -1 && j < this->returnRow())
			{
				if (this->_minefield[j][i].isMine()) total += 1;
			}
		}
	}
	this->_minefield[row][col].setNeighborCount(total);
}

int MineSweeper::countFlag(int col, int row)
{
	int total = 0;
	for (int yoff = -1; yoff <= 1; ++yoff)
	{
		for (int xoff = -1; xoff <= 1; ++xoff)
		{
			if (xoff != 0 || yoff != 0)
			{
				int i = this->_minefield[row][col].returnCol() + xoff;
				int j = this->_minefield[row][col].returnRow() + yoff;

				if (i > -1 && i < this->returnCol() && j > -1 && j < this->returnRow())
				{
					if (this->_minefield[j][i].isFlagged()) total += 1;
				}
			}
		}
	}
	return total;
}


void MineSweeper::EndGame(bool win)
{
	for (int i = 0; i < this->returnRow(); ++i)
	{
		for (int j = 0; j < this->returnCol(); ++j)
		{
			this->_minefield[i][j].reveal();
		}
	}

	this->printMineField();

	if (win)
	{
		cout << "Won!" << endl;
	}
	else
	{
		cout << "Lost..." << endl;
	}

	this->setGameEnd(true);
}


// set series
// set GameEnd
void MineSweeper::setGameEnd(bool finished)
{
	this->_gameEnd = true;
}

void MineSweeper::setFlag(int col, int row)
{
	if (!this->_minefield[row][col].isRevealed())
		this->_minefield[row][col].setFlag();
	else cout << "You cannot flag tiles that are already open" << endl;
}


// return series
// return MineNum
int MineSweeper::returnMineNum()
{
	return this->_mineNum;
}

// return columns
int MineSweeper::returnCol()
{
	return this->_col;
}

// return row
int MineSweeper::returnRow()
{
	return this->_row;
}

// return gameEnd
bool MineSweeper::returnGameEnd()
{
	return this->_gameEnd;
}
