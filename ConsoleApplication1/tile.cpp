#include <iostream>
#include <vector>
#include <string>

#include "tile.h"

using namespace std;
Tile::Tile()
{
	this->_col = 0;
	this->_row = 0;
	//this->_revealed = true;
}

Tile::Tile(int col, int row)
{
	this->_col = col;
	this->_row = row;
	//this->_revealed = true;
}

void Tile::printTile()
{
	std::cout << "|";
	if (this->isRevealed() && this->isMine()) cout << "M";
	else if (this->isRevealed())
		cout << this->returnNeighborCount();
	else if (this->isFlagged()) cout << "F";			// Flag
	else cout << "C";									// Covered 
}

void Tile::reveal()
{
	this->_revealed = true;
}

// is series
// is it mine?
bool Tile::isMine()
{
	return this->_mine;
}

// is it Flagged?
bool Tile::isFlagged()
{
	return this->_flagged;
}

// is it revealed?
bool Tile::isRevealed()
{
	return this->_revealed;
}

// return series
// return its column
int Tile::returnCol()
{
	return this->_col;
}

// return its row
int Tile::returnRow()
{
	return this->_row;
}

// return neighborCount;
int Tile::returnNeighborCount()
{
	return this->_neighborCount;
}

string Tile::returnTile()
{
	if (this->isRevealed() && this->isMine()) return "M";
	else if (this->isRevealed())
		return to_string(this->returnNeighborCount());
	else if (this->isFlagged()) return "F";				// Flag
	else return "C";									// Covered
}

// set series
// set Mine
void Tile::setMine()
{
	this->_mine = true;
}

// set Flag
void Tile::setFlag()
{
	if (this->_flagged) this->_flagged = false;
	else this->_flagged = true;
}

// set NeighborCount
void Tile::setNeighborCount(int count)
{
	this->_neighborCount = count;
}

