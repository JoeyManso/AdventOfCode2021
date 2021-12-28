#include "Days.h"
#include <vector>

using namespace std;

struct SeaCucumbers
{
	void Parse(const string& arg)
	{
		for (int x = 0; x < arg.length(); ++x)
		{
			if (grid.size() <= x)
			{
				grid.push_back({});
			}
			grid[x].push_back(arg[x]);
		};
	}
	
	size_t Cols() const
	{
		return grid.size();
	}

	size_t Rows() const
	{
		return grid[0].size();
	}

	bool MoveEast()
	{
		bool bDidMove = false;
		for (size_t y = 0; y < Rows(); ++y)
		{
			// Cache character in first column for accurate wrapping
			char cZero = grid[0][y];
			for (size_t x = 0; x < Cols(); ++x)
			{
				size_t adjX = (x + 1 < Cols()) ? x + 1 : 0;
				if (grid[x][y] == '>' && (adjX == 0 ? cZero : grid[adjX][y]) == '.')
				{
					grid[adjX][y] = grid[x][y];
					grid[x][y] = '.';
					++x;
					bDidMove = true;
				}
			}
		}
		return bDidMove;
	}

	bool MoveSouth()
	{
		bool bDidMove = false;
		
		for (size_t x = 0; x < Cols(); ++x)
		{
			// Cache character in first row for accurate wrapping
			char cZero = grid[x][0];
			for (size_t y = 0; y < Rows(); ++y)
			{
				size_t adjY = (y + 1 < Rows()) ? y + 1 : 0;
				if (grid[x][y] == 'v' && (adjY == 0 ? cZero : grid[x][adjY]) == '.')
				{
					grid[x][adjY] = grid[x][y];
					grid[x][y] = '.';
					++y;
					bDidMove = true;
				}
			}
		}
		return bDidMove;
	}

	bool TryMove()
	{
		bool bDidMove = false;
		bDidMove |= MoveEast();
		bDidMove |= MoveSouth();
		return bDidMove;
	}

	string ToString() const
	{
		string s;
		for (int x = 0, y = 0; y < Rows(); ++y)
		{
			for (; x < Cols(); ++x)
			{
				s.push_back(grid[x][y]);
			}
			s.push_back('\n');
			x = 0;
		}
		return s;
	}

	vector<char>& operator[](size_t x)
	{
		return grid[x];
	}

	const vector<char>& operator[](size_t x) const
	{
		return grid[x];
	}

	friend ostream& operator<<(ostream& os, const SeaCucumbers& b)
	{
		os << b.ToString();
		return os;
	}

private:
	vector<vector<char>> grid;
};

template <>
void Run<Day25>(Part part, istream& is, ostream& os)
{
	if (part == Part01)
	{
		SeaCucumbers seaCucumbers;
		
		string arg;
		while (getline(is, arg))
		{
			seaCucumbers.Parse(arg);
		}

		//os << "Start: " << endl << seaCucumbers << endl;
		int step = 0;
		while (seaCucumbers.TryMove())
		{
			//cin.ignore();
			++step;
			//os << "Step " << step << ": " << endl << seaCucumbers << endl;
		}
		os << "Sea Cucumbers stop moving after " << step + 1 << " steps" << endl;
	}
}