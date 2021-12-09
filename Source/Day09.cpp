#include "Days.h"
#include "AdventUtils.h"
#include "Point2D.h"
#include <algorithm>
#include <set>
#include <vector>

using namespace std;

size_t GetBasinPoints(int x, int y, const vector<vector<int>>& caveHeightMap, set<Point2D>& basinPoints)
{
	if (x >= 0 && x < caveHeightMap.size() && y >= 0 && y < caveHeightMap[x].size() && caveHeightMap[x][y] < 9)
	{
		Point2D p = Point2D(x, y);
		if (basinPoints.find(p) == basinPoints.end())
		{
			basinPoints.emplace(p);

			// Left
			GetBasinPoints(x - 1, y, caveHeightMap, basinPoints);
			// Up
			GetBasinPoints(x, y - 1, caveHeightMap, basinPoints);
			// Right
			GetBasinPoints(x + 1, y, caveHeightMap, basinPoints);
			// Down
			GetBasinPoints(x, y + 1, caveHeightMap, basinPoints);
		}
	}
	return basinPoints.size();
}

template <>
void Run<Day09>(Part part, istream& is, ostream& os)
{
	vector<vector<int>> caveHeightMap = {};

	string arg;

	while (getline(is, arg))
	{
		for (int x = 0; x < arg.length(); ++x)
		{
			if (caveHeightMap.size() <= x)
			{
				caveHeightMap.push_back(vector<int>());
			}
			caveHeightMap[x].push_back(CharToInt(arg[x]));
		};
	}

	vector<int> lowestValues = {};
	vector<size_t> basinSizes = {};
	for (int x = 0; x < caveHeightMap.size(); ++x)
	{
		for (int y = 0; y < caveHeightMap[x].size(); ++y)
		{
			int value = caveHeightMap[x][y];
			if (
				(x == 0 || value < caveHeightMap[x-1][y])
				&& (y == 0 || value < caveHeightMap[x][y-1])
				&& (x == caveHeightMap.size() - 1 || value < caveHeightMap[x+1][y])
				&& (y == caveHeightMap[x].size() - 1 || value < caveHeightMap[x][y+1])
			)
			{
				lowestValues.push_back(value);

				set<Point2D> basinPoints = {};
				basinSizes.push_back(GetBasinPoints(x, y, caveHeightMap, basinPoints));
			}
		}
	}

	if(part == Part01)
	{
		int riskSum = 0;
		for (int value : lowestValues)
		{
			riskSum += value + 1;
		}
		cout << "Risk Sum: " << riskSum << endl;
	}
	else if(part == Part02)
	{
		sort(basinSizes.begin(), basinSizes.end(), greater<size_t>());
		size_t multipliedBasins = basinSizes.size() > 0 ? 1 : 0;
		for (size_t i = 0; i < 3 && i < basinSizes.size(); ++i)
		{
			multipliedBasins *= basinSizes[i];
		}
		cout << "Multiplied Basins: " << multipliedBasins << endl;
	}
}