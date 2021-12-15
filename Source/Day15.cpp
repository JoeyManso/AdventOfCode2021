#include "Days.h"
#include "AdventUtils.h"
#include "Point2D.h"
#include <map>
#include <queue>
#include <vector>

using namespace std;

/** Return lowest risk value for the given Point */
int IsValidPoint(const vector<vector<int>>& caveRiskGrid, const Point2D& p)
{
	return (p.x >= 0 && p.x < caveRiskGrid.size() && p.y >= 0 && p.y < caveRiskGrid[p.x].size());
}

int GetRisk(const vector<vector<int>>& caveRiskGrid, const Point2D& p)
{
	if (IsValidPoint(caveRiskGrid, p))
	{
		return caveRiskGrid[p.x][p.y];
	}
	return INT_MAX;
}

template <>
void Run<Day15>(Part part, istream& is, ostream& os)
{
	vector<vector<int>> caveRiskGrid = {};

	string arg;
	while (getline(is, arg))
	{
		for (int x = 0; x < arg.length(); ++x)
		{
			if (caveRiskGrid.size() <= x)
			{
				caveRiskGrid.push_back({});
			}
			caveRiskGrid[x].push_back(CharToInt(arg[x]));
		}
	}
	
	if (part == Part02)
	{
		// Expand grid by 5x
		size_t caveSizeOriginal = caveRiskGrid.size();
		size_t caveSizeNew = caveSizeOriginal * 5;
		caveRiskGrid.resize(caveSizeNew);
		for (size_t x = 0; x < caveSizeNew; ++x)
		{
			caveRiskGrid[x].resize(caveSizeNew);
			for (size_t y = 0; y < caveSizeNew; ++y)
			{
				for (int i = 1; i < 5; ++i)
				{
					int newValue = caveRiskGrid[x][y] + i;
					if (newValue > 9)
					{
						newValue -= 9;
					}
					
					size_t transposeX = x + i * caveSizeOriginal;
					size_t transposeY = y + i * caveSizeOriginal;

					if (transposeX < caveSizeNew)
					{	
						if (caveRiskGrid[transposeX].size() < caveSizeNew)
						{
							caveRiskGrid[transposeX].resize(caveSizeNew);
						}
						caveRiskGrid[transposeX][y] = newValue;
					}
					
					if (transposeY < caveSizeNew)
					{
						caveRiskGrid[x][transposeY] = newValue;
					}
				}
			}
		}
	}
	
	/**
	 * Dijkstra                                                                      
	*/
	
	// Distances from origin (0,0) to each point
	map<Point2D, int> pointRiskMap = {};
	for (int x = 0; x < caveRiskGrid.size(); ++x)
	{
		for (int y = 0; y < caveRiskGrid[x].size(); ++y)
		{
			Point2D p = Point2D(x, y);
			pointRiskMap.emplace(p, INT_MAX);
		}
	}	

	queue<Point2D> pointQueue = {};
	pointRiskMap[Point2D(0, 0)] = 0;
	pointQueue.push(Point2D(0, 0));
	while (pointQueue.size())
	{
		const Point2D p = pointQueue.front();
		pointQueue.pop();

		// Visit adjacent points of the selected point
		const Point2D nextPoints[] = {
			// Up
			Point2D(p.x, p.y - 1),
			// Right
			Point2D(p.x + 1, p.y),
			// Down
			Point2D(p.x, p.y + 1),
			// Left
			Point2D(p.x - 1, p.y)
		};
		for (const Point2D& nextPoint : nextPoints)
		{
			if (IsValidPoint(caveRiskGrid, nextPoint))
			{
				int p2Risk = pointRiskMap[p] + GetRisk(caveRiskGrid, nextPoint);
				if (p2Risk < pointRiskMap[nextPoint])
				{
					pointRiskMap[nextPoint] = p2Risk;
					pointQueue.push(nextPoint);
				}
			}
		}
	}

	int gridSize = static_cast<int>(caveRiskGrid.size()) - 1;
	Point2D endPoint = Point2D(gridSize, gridSize);
	os << "Lowest Risk: " << pointRiskMap[endPoint] << endl;
}