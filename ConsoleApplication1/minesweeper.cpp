#include <iostream>
#include <vector>
#include "tile.h"
#include "minesweeper.h"
#include <ctime>
#include <sstream>

MineSweeper::MineSweeper(int col, int row, int mineNum)
{
	this->_mineNum = mineNum;
	this->_col = col;
	this->_row = row;

	this->_minefield = new Tile*[row];
	for (int r = 0; r < row; ++r)
	{
		this->_minefield[r] = new Tile[col];
	}

	for (int r = 0; r < row; ++r)
	{
		for (int c = 0; c < col; ++c)
		{
			this->_minefield[r][c] = Tile(c, r);
		}
	}
}

MineSweeper::~MineSweeper()
{
	for (int r = 0; r < this->returnRow(); ++r)
	{
		delete[] this->_minefield[r];
	}
	delete[] this->_minefield;
}


void MineSweeper::createMinefield(int col, int row)
{
	vector<pair<int, int>> options;

	for (int r = 0; r < this->returnRow(); ++r)
	{
		for (int c = 0; c < this->returnCol(); ++c)
		{
			if (!(r == row && c == col))
			{
				options.push_back(make_pair(c, r));
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
			this->_minefield[pair1.second][pair1.first].setMine();
			options.erase(options.begin() + k);;
		}
	}

	for (int r = 0; r < this->returnRow(); ++r)
	{
		for (int c = 0; c < this->returnCol(); ++c)
		{
			this->countMine(c, r);
		}
	}

	this->revealTile(col, row);
}

void MineSweeper::checkWin()
{
	bool win = true;
	for (int r = 0; r < this->returnRow(); ++r)
	{
		for (int c = 0; c < this->returnCol(); ++c)
		{
			if (!this->_minefield[r][c].isRevealed()
				&& !this->_minefield[r][c].isMine())
				win = false;
		}
	}
	if (win)
	{
		this->setGameEnd(true);
		this->setWin(true);
	}
}

void MineSweeper::printMineField()
{
	for (int c = 0; c < this->returnCol() * 2 + 1; ++c)
	{
		cout << "-";
	}
	cout << endl;
	for (int r = 0; r < this->returnRow(); ++r)
	{
		for (int c = 0; c < this->returnCol(); ++c)
		{
			this->_minefield[r][c].printTile();
		}
		cout << "|" << endl;
		for (int c = 0; c < this->returnCol() * 2 + 1; ++c)
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
	if (this->_minefield[row][col].returnNeighborCount() == 0
		&& !this->_minefield[row][col].isMine())
	{
		for (int yoff = -1; yoff <= 1; ++yoff)
		{
			for (int xoff = -1; xoff <= 1; ++xoff)
			{
				int c = this->_minefield[row][col].returnCol() + xoff;
				int r = this->_minefield[row][col].returnRow() + yoff;

				if (c > -1 && c < this->returnCol() && r > -1
					&& r < this->returnRow())
				{
					if (!this->_minefield[r][c].isRevealed())
						this->revealTile(c, r);
				}
			}
		}
	}
}

void MineSweeper::revealDoubleClick(int col, int row)
{
	auto tile = this->_minefield[row][col];
	// not revealed -> do nothing
	if (!tile.isRevealed()) return;

	// the flag count does not match the mine num -> do nothing
	if (this->countFlag(col, row) != tile.returnNeighborCount()) return;

	for (int yoff = -1; yoff <= 1; ++yoff)
	{
		for (int xoff = -1; xoff <= 1; ++xoff)
		{
			if (xoff != 0 || yoff != 0)
			{
				int c = tile.returnCol() + xoff;
				int r = tile.returnRow() + yoff;

				// within range
				if (c > -1 && c < this->returnCol() && r > -1
					&& r < this->returnRow())
				{
					if (!this->_minefield[r][c].isFlagged())
						this->revealTile(c, r);
				}
			}
		}
	}
}

void MineSweeper::countMine(int col, int row)
{
	// if the tile is mine
	if (this->_minefield[row][col].isMine())
	{
		this->_minefield[row][col].setNeighborCount(-1);
		return;
	}

	// not mine
	int total = 0;
	for (int yoff = -1; yoff <= 1; ++yoff)
	{
		for (int xoff = -1; xoff <= 1; ++xoff)
		{
			int c = this->_minefield[row][col].returnCol() + xoff;
			int r = this->_minefield[row][col].returnRow() + yoff;

			if (c > -1 && c < this->returnCol()
				&& r > -1 && r < this->returnRow())
			{
				if (this->_minefield[r][c].isMine()) total += 1;
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
				int c = this->_minefield[row][col].returnCol() + xoff;
				int r = this->_minefield[row][col].returnRow() + yoff;

				if (c > -1 && c < this->returnCol()
					&& r > -1 && r < this->returnRow())
				{
					if (this->_minefield[r][c].isFlagged()) total += 1;
				}
			}
		}
	}
	return total;
}


void MineSweeper::EndGame(bool win)
{
	for (int r = 0; r < this->returnRow(); ++r)
	{
		for (int c = 0; c < this->returnCol(); ++c)
		{
			this->_minefield[r][c].reveal();
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
	this->_gameEnd = finished;
}

void MineSweeper::setFlag(int col, int row)
{
	if (!this->_minefield[row][col].isRevealed())
		this->_minefield[row][col].setFlag();
}

void MineSweeper::setWin(bool win)
{
	this->_win = win;
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

// return win
bool MineSweeper::returnWin()
{
	return this->_win;
}

// return state of the minefield (for AI)
string MineSweeper::returnMineState()
{
	stringstream s;

	for (int r = 0; r < this->returnRow(); ++r)
	{
		for (int c = 0; c < this->returnCol(); ++c)
		{
			s << this->_minefield[r][c].returnTile();
		}
	}

	return s.str();
}