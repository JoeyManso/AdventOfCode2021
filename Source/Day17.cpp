#include "Days.h"
#include "Point2D.h"
#include "Vector2D.h"
#include <map>
#include <set>
#include <regex>

using namespace std;

const regex RX_TARGET_AREA(R"(target area: x=(-?\d+)..(-?\d+), y=(-?\d+)..(-?\d+))");

template <>
void Run<Day17>(Part part, istream& is, ostream& os)
{
	string arg;
	smatch match;
	getline(is, arg);
	regex_search(arg, match, RX_TARGET_AREA);
	
	Point2D targetAreaMin = Point2D(stoi(match.str(1)), stoi(match.str(3)));
	Point2D targetAreaMax = Point2D(stoi(match.str(2)), stoi(match.str(4)));

	// Mapping of vStart with associated maxY values for all initial velocities that hit the target
	map<Vector2D, int> vStartMaxYMap = {};
	for (int x = 0; x <= targetAreaMax.x; ++x)
	{
		for (int y = targetAreaMin.y; y <= abs(targetAreaMin.y); ++y)
		{
			Point2D p = Point2D(0, 0);
			Vector2D vStart = Vector2D(x, y);
			Vector2D v = vStart;
			int maxY = -INT_MAX;
			for (int stepCount = 1; p.x <= targetAreaMax.x && p.y >= targetAreaMin.y; ++stepCount)
			{
				p += v;
				maxY = max(maxY, p.y);
				if (p.x >= targetAreaMin.x && p.x <= targetAreaMax.x && p.y >= targetAreaMin.y && p.y <= targetAreaMax.y)
				{
					// Within target, add to the map
					vStartMaxYMap.emplace(vStart, maxY);
					break;
				}
				else
				{
					// Drag
					if (v.x > 0)
						--v.x;
					else if (v.x < 0)
						++v.x;

					// Gravity
					--v.y;
				}
			}
		}
	}

	if(part == Part01)
	{
		Vector2D vStartBest;
		int maxY = 0;
		for (const auto& vStartMaxYPair : vStartMaxYMap)
		{
			if (vStartMaxYPair.second > maxY)
			{
				vStartBest = vStartMaxYPair.first;
				maxY = vStartMaxYPair.second;
			}
		}
		os << "vStart " << vStartBest << " has maxY: " << maxY << endl;
	}
	else if(part == Part02)
	{
		os << "Total Starting Velocity Values: " << vStartMaxYMap.size() << endl;
	}
}