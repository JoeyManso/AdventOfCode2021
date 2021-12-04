#include "Days.h"
#include "AdventUtils.h"

using namespace std;

const int GRID_SIZE = 5;

struct BingoGrid
{
public:
	struct Tile
	{
		Tile() : Tile(-1) {}
		
		Tile(int _number) 
			: number(_number), marked(false) {}
		
		bool IsMarked() const { return marked; }
		void SetMarked(bool _marked) { marked = _marked; }
		int GetNumber() const { return number; }
		
	private:
		int number;
		bool marked;
	};

	void TrySetMarked(int ballNumber)
	{
		for (int rowIdx = 0; rowIdx < GRID_SIZE; ++rowIdx)
		{
			for (int colIdx = 0; colIdx < GRID_SIZE; ++colIdx)
			{
				Tile& tile = tiles[rowIdx][colIdx];
				if (tile.GetNumber() == ballNumber)
				{
					tile.SetMarked(true);
				}
			}
		}
	}

	int GetUnmarkedSum() const
	{
		int unmarkedSum = 0;
		for (int rowIdx = 0; rowIdx < GRID_SIZE; ++rowIdx)
		{
			for (int colIdx = 0; colIdx < GRID_SIZE; ++colIdx)
			{
				const Tile& tile = tiles[rowIdx][colIdx];
				if (!tile.IsMarked())
				{
					unmarkedSum += tile.GetNumber();
				}
			}
		}
		return unmarkedSum;
	}

	bool IsWinner() const
	{
		int rowMarkCount[GRID_SIZE], colMarkCount[GRID_SIZE];
		for (int i = 0; i < GRID_SIZE; ++i)
		{
			rowMarkCount[i] = 0;
			colMarkCount[i] = 0;
		}
		for (int rowIdx = 0; rowIdx < GRID_SIZE; ++rowIdx)
		{
			for (int colIdx = 0; colIdx < GRID_SIZE; ++colIdx)
			{
				const Tile& tile = tiles[rowIdx][colIdx];
				if (tile.IsMarked())
				{
					++rowMarkCount[rowIdx];
					++colMarkCount[colIdx];
				}
			}
		}
		for (int i = 0; i < GRID_SIZE; ++i)
		{
			if (rowMarkCount[i] == GRID_SIZE || colMarkCount[i] == GRID_SIZE)
			{
				return true;
			}
		}
		return false;
	}
	
	Tile tiles[GRID_SIZE][GRID_SIZE];
};

template <>
void Run<Day04>(Part part, istream& is, ostream& os)
{	
	int rowIdx = 0;
	int colIdx = 0;
	int gridIdx = 0;
	vector<int> bingoBalls = {};
	vector<BingoGrid> bingoGrids = {};
	
	string arg;
	while(getline(is, arg))
	{
		// Bingo ball row is split by ','
		if (arg.find(',') != string::npos)
		{
			bingoBalls = splitInt(arg, ',');
		}

		// All other rows are split by ' '
		vector<int> bingoGridNumbers = splitInt(arg, ' ');
		if (bingoGridNumbers.size() == GRID_SIZE)
		{
			while (bingoGrids.size() <= gridIdx)
			{
				bingoGrids.push_back(BingoGrid());
			}
			for (int colIdx = 0; colIdx < bingoGridNumbers.size(); ++colIdx)
			{
				const int& bingoGridNumber = bingoGridNumbers[colIdx];
				bingoGrids[gridIdx].tiles[rowIdx][colIdx] = BingoGrid::Tile(bingoGridNumber);
			}
			++rowIdx;
			if (rowIdx == GRID_SIZE)
			{
				// Bingo Grid parsing complete
				rowIdx = 0;
				++gridIdx;
			}
		}
	}
	
	// Draw bingo balls and and mark each Grid
	for (const int& bingoBall : bingoBalls)
	{
		for (BingoGrid& bingoGrid : bingoGrids)
		{
			bingoGrid.TrySetMarked(bingoBall);
		}

		// Check for winning Grids
		for (int i = 0; i < bingoGrids.size();)
		{	
			const BingoGrid& bingoGrid = bingoGrids[i];
			if (bingoGrid.IsWinner())
			{
				if (part == Part01)
				{
					if (bingoGrids.size() == gridIdx)
					{
						cout << "Bingo Grid " << i + 1 << " is the first winner" << endl;
						cout << "Bingo Grid Unmarked Sum: " << bingoGrid.GetUnmarkedSum() << endl;
						cout << "Bingo Grid Score: " << bingoGrid.GetUnmarkedSum() * bingoBall << endl;
					}
					return;
				}
				else if(part == Part02)
				{
					if (bingoGrids.size() == 1)
					{
						cout << "Bingo Grid " << i + 1 << " is the last winner" << endl;
						cout << "Bingo Grid Unmarked Sum: " << bingoGrid.GetUnmarkedSum() << endl;
						cout << "Bingo Grid Score: " << bingoGrid.GetUnmarkedSum() * bingoBall << endl;
					}
					bingoGrids.erase(bingoGrids.begin() + i);
				}
			}
			else
			{
				++i;
			}
		}
	}	
}