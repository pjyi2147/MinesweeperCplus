#pragma once

class Tile
{
private:
	bool _mine = false;				// is it mine?
	bool _revealed = false;			// is it revealed?
	bool _flagged = false;

	int _neighborCount = 0;				// how many mines does the neighbors have?
	int _col;
	int _row;
	
public:
	Tile();
	Tile(int col, int row);

	void reveal();
	void printTile();
	
	bool isMine();
	bool isRevealed();
	bool isFlagged();

	int returnRow();
	int returnCol();
	int returnNeighborCount();
	std::string returnTile();

	void setMine();
	void setFlag();
	void setNeighborCount(int count);
};