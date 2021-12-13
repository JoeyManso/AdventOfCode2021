#include "Days.h"
#include "AdventUtils.h"
#include <regex>
#include <vector>

using namespace std;

const regex RX_COORD(R"((\d+),(\d+))");
const regex RX_FOLD(R"(fold along ([x|y])=(\d+))");

void PrintDots(ostream& os, const vector<vector<char>>& paperDots)
{
	for (int x = 0, y = 0; y < paperDots[x].size(); ++y)
	{
		for (; x < paperDots.size(); ++x)
		{
			os << paperDots[x][y];
		}
		x = 0;
		os << endl;
	}
	os << endl;
}

template <>
void Run<Day13>(Part part, istream& is, ostream& os)
{
	int maxX = 0, maxY = 0, foldNum = 0;
	vector<vector<char>> paperDots = {};
	
	string arg;
	while (getline(is, arg))
	{
		smatch matchCoord, matchFold;
		if (regex_search(arg, matchCoord, RX_COORD))
		{
			int markedX = stoi(matchCoord.str(1)), markedY = stoi(matchCoord.str(2));

			// Ensure odd Grid size
			maxX = max(markedX % 2 == 0 ? markedX : markedX + 1, maxX);
			maxY = max(markedY % 2 == 0 ? markedY : markedY + 1, maxY);
			for (int x = 0; x <= maxX; ++x)
			{
				if (paperDots.size() <= x)
				{
					paperDots.push_back(vector<char>());
				}
				for (int y = 0; y <= maxY; ++y)
				{
					if (paperDots[x].size() <= y)
					{
						paperDots[x].push_back('.');
					}
				}
			}
			paperDots[markedX][markedY] = '#';
		}
		else if(regex_search(arg, matchFold, RX_FOLD))
		{
			++foldNum;
			char foldAxis = matchFold.str(1)[0];
			int foldLine = stoi(matchFold.str(2));
			if (foldAxis == 'x')
			{
				size_t foldDist = foldLine * 2;
				for (size_t x = paperDots.size() - 1; x >= foldLine; --x)
				{
					for (size_t y = 0; y < paperDots[x].size(); ++y)
					{
						if (x > foldLine && paperDots[x][y] == '#')
						{
							paperDots[x - foldDist][y] = '#';
						}
					}
					paperDots.erase(paperDots.begin() + x);
					foldDist -= 2;
				}
			}
			else if (foldAxis == 'y')
			{
				for (size_t x = 0; x < paperDots.size(); ++x)
				{
					size_t foldDist = foldLine * 2;
					for (size_t y = paperDots[x].size() - 1; y >= foldLine; --y)
					{
						if (y > foldLine && paperDots[x][y] == '#')
						{
							paperDots[x][y - foldDist] = '#';
						}
						paperDots[x].erase(paperDots[x].begin() + y);
						foldDist -= 2;
					}
				}
			}

			int visibleMarks = 0;
			for (int x = 0; x < paperDots.size(); ++x)
			{
				for (int y = 0; y < paperDots[x].size(); ++y)
				{
					if(paperDots[x][y] == '#')
					{
						++visibleMarks;
					}
				}
			}
			os << "Fold " << foldNum << " Total Visible Marks: " << visibleMarks << endl;
			if (part == Part01)
			{
				break;
			}
		}
	}

	if(part == Part02)
	{
		os << endl << "Thermal Code:" << endl;
		PrintDots(os, paperDots);
	}
}