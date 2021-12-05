#include "Days.h"
#include "Point2D.h"
#include <regex>
#include <vector>

using namespace std;

const regex RX_VENT_LINE(R"((\d+),(\d+) -> (\d+),(\d+))");

const int GRID_SIZE = 1000;

struct FissureLine
{
	FissureLine() {};
	FissureLine(const Point2D& _start, const Point2D& _end)
		: start(_start), end(_end)
	{
	}

	const Point2D& GetStart() const { return start; }
	const Point2D& GetEnd() const { return end; }

private:
	Point2D start;
	Point2D end;
};

template <>
void Run<Day05>(Part part, istream& is, ostream& os)
{
	vector<vector<int>> overlapGrid = {};
	vector<FissureLine> fissureLines = {};
	
	string arg;
	while (getline(is, arg))
	{
		smatch match;
		regex_search(arg, match, RX_VENT_LINE);
		fissureLines.push_back(
			{
				Point2D(stoi(match.str(1)), stoi(match.str(2))),
				Point2D(stoi(match.str(3)), stoi(match.str(4)))
			}
		);
	}

	for (int x = 0; x < GRID_SIZE; ++x)
	{
		overlapGrid.push_back({});
		for (int y = 0; y < GRID_SIZE; ++y)
		{
			overlapGrid[x].push_back(0);
		}
	}

	for (const FissureLine& fissureLine : fissureLines)
	{
		if (fissureLine.GetStart().x == fissureLine.GetEnd().x)
		{
			// Mark vertical lines
			for (
				int y = fissureLine.GetStart().y;
				fissureLine.GetEnd().y > fissureLine.GetStart().y ? y <= fissureLine.GetEnd().y : y >= fissureLine.GetEnd().y;
				fissureLine.GetEnd().y > fissureLine.GetStart().y ? ++y : --y
			)
			{
				++overlapGrid[fissureLine.GetStart().x][y];
			}
		}
		if (fissureLine.GetStart().y == fissureLine.GetEnd().y)
		{
			// Mark horizontal lines
			for (
				int x = fissureLine.GetStart().x;
				fissureLine.GetEnd().x > fissureLine.GetStart().x ? x <= fissureLine.GetEnd().x : x >= fissureLine.GetEnd().x;
				fissureLine.GetEnd().x > fissureLine.GetStart().x ? ++x : --x
			)
			{
				++overlapGrid[x][fissureLine.GetStart().y];
			}
		}

		if (part == Part02)
		{
			// Mark 45 degree diagonal lines
			if (abs(fissureLine.GetStart().x - fissureLine.GetEnd().x) == abs(fissureLine.GetStart().y - fissureLine.GetEnd().y))
			{
				for (
					int x = fissureLine.GetStart().x, y = fissureLine.GetStart().y;
					fissureLine.GetEnd().x > fissureLine.GetStart().x ? x <= fissureLine.GetEnd().x : x >= fissureLine.GetEnd().x;
					fissureLine.GetEnd().x > fissureLine.GetStart().x ? ++x : --x, 
					fissureLine.GetEnd().y > fissureLine.GetStart().y ? ++y : --y
				)
				{
					++overlapGrid[x][y];
				}
			}
		}
	}

	int overlapCount = 0;
	for (int y = 0; y < GRID_SIZE; ++y)
	{
		for (int x = 0; x < GRID_SIZE; ++x)
		{
			//cout << overlapGrid[x][y];
			if (overlapGrid[x][y] >= 2)
			{
				++overlapCount;
			}
		}
		//cout << endl;
	}
	cout << "Overlap Count: " << overlapCount << endl;
}