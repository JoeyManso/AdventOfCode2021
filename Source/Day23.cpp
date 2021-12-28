#include "Days.h"
#include "Point2D.h"
#include "Vector2D.h"
#include <map>
#include <vector>

using namespace std;

const int HALLWAY_ROW = 1;
const map<char, uint64_t> MOVE_ENERGY_MAP = {{'A', 1}, {'B', 10}, {'C', 100}, {'D', 1000}};
const map<char, int> ROOM_COLUMNS_MAP = {{'A', 3}, {'B', 5}, {'C', 7}, {'D', 9}};
const vector<Point2D> HALLWAY_SPOTS = {
	Point2D(1,HALLWAY_ROW),
	Point2D(2,HALLWAY_ROW),
	Point2D(4,HALLWAY_ROW),
	Point2D(6,HALLWAY_ROW),
	Point2D(8,HALLWAY_ROW),
	Point2D(10,HALLWAY_ROW),
	Point2D(11,HALLWAY_ROW)
};

struct BurrowNode
{
	BurrowNode(char _c)
		: c(_c)
	{
	}
	
	BurrowNode()
		: BurrowNode(' ')
	{
	}

	bool IsOpen() const
	{
		return c == '.';
	}

	bool IsAmphipod() const
	{
		return isalpha(c);
	}

	uint64_t GetMoveEnergy() const
	{
		return MOVE_ENERGY_MAP.find(c)->second;
	}

	int GetRoomColumn() const
	{
		return ROOM_COLUMNS_MAP.find(c)->second;
	}

	friend ostream& operator<<(ostream& os, const BurrowNode& n)
	{
		os << n.c;
		return os;
	}

	char c;
};

struct Burrow
{
	void Parse(const string& arg)
	{
		for (int x = 0; x < arg.length(); ++x)
		{
			if (grid.size() <= x)
			{
				grid.push_back({});
			}
			grid[x].push_back(BurrowNode(arg[x]));
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

	uint64_t GetEnergyCost() const
	{
		return energyCost;
	}

	bool CanEnterRoom(const BurrowNode& node) const
	{
		const int roomColumn = node.GetRoomColumn();
		for (size_t y = 2; y < Rows() - 1; ++y)
		{
			if (!grid[roomColumn][y].IsOpen() && grid[roomColumn][y].c != node.c)
			{
				// Room is occupied by the wrong Amphipod(s), correct ones cannot enter
				return false;
			}
		}
		return true;
	}

	bool CanExitRoom(const Point2D& roomSpot) const
	{
		const BurrowNode& node = (*this)[roomSpot];
		if (node.IsAmphipod())
		{
			const int roomColumn = node.GetRoomColumn();
			if (roomSpot.x != roomColumn)
			{
				// Node is in the wrong room
				return true;
			}
			else
			{
				// Node is in the right room, but can move if it blocks another node
				for (size_t y = roomSpot.y; y < Rows() - 1; ++y)
				{
					if (grid[roomColumn][y].c != node.c)
					{
						return true;
					}
				}
			}
		}
		return false;
	}

	bool IsOrdered() const
	{
		for (const auto& orderedColumnsPair : ROOM_COLUMNS_MAP)
		{
			char expectedOccupant = orderedColumnsPair.first;
			size_t x = orderedColumnsPair.second;
			for (size_t y = 2; y < Rows() - 1; ++y)
			{
				if (grid[x][y].c != expectedOccupant)
				{
					return false;
				}
			}
		}
		return true;
	}
	
	int CanVisit(const Point2D& p) const
	{
		if (p.x >= 0 && p.x < Cols() && p.y >= 0 && p.y < Rows())
		{
			return (*this)[p].IsOpen();
		}
		return false;
	}

	uint64_t GetEnergyCost(const Point2D& pStart, const Point2D& pTarget) const
	{
		// Ensure the path is open
		const Vector2D delta = pTarget - pStart;
		Point2D pCur = pStart;
		while(pCur != pTarget)
		{
			if (pTarget.y > pStart.y)
			{
				if (pCur.x != pTarget.x)
				{
					pCur.x += delta.x > 0 ? 1 : -1;
				}
				else
				{
					pCur.y += delta.y > 0 ? 1 : -1;
				}
			}
			else
			{
				if (pCur.y != pTarget.y)
				{
					pCur.y += delta.y > 0 ? 1 : -1;
				}
				else
				{
					pCur.x += delta.x > 0 ? 1 : -1;
				}
			}
			if (!(*this)[pCur].IsOpen())
			{
				return INT_MAX;
			}
		}
		// Return energy cost for the move
		uint64_t energyCost = (abs(delta.x) + abs(delta.y)) * (*this)[pStart].GetMoveEnergy();
		return energyCost;
	}

	bool TryMove(const Point2D& pStart, const Point2D& pTarget)
	{
		const uint64_t cost = GetEnergyCost(pStart, pTarget);
		if (cost < INT_MAX)
		{
			Burrow& burrow = (*this);
			burrow[pTarget].c = burrow[pStart].c;
			burrow[pStart].c = '.';
			energyCost += cost;
			return true;
		}
		return false;
	}

	uint64_t CreateKey() const
	{
		uint64_t key = energyCost;
		for (size_t x = 0; x < Cols(); ++x)
		{
			for (size_t y = 0; y < Rows(); ++y)
			{
				if (grid[x][y].IsAmphipod())
				{
					key += (key * grid[x][y].GetMoveEnergy()) + (x * 10) + (y * 100);
				}
			}
		}
		return key;
	}

	string ToString() const
	{
		string s;
		for (int x = 0, y = 0; y < Rows(); ++y)
		{
			for (; x < Cols(); ++x)
			{
				s.push_back(grid[x][y].c);
			}
			s.push_back('\n');
			x = 0;
		}
		return s;
	}

	BurrowNode& operator[](const Point2D& p)
	{
		return grid[p.x][p.y];
	}

	const BurrowNode& operator[](const Point2D& p) const
	{
		return grid[p.x][p.y];
	}

	friend ostream& operator<<(ostream& os, const Burrow& b)
	{
		os << b.ToString();
		return os;
	}

private:
	uint64_t energyCost = 0;
	vector<vector<BurrowNode>> grid;
};

void TryOrderBurrow(Burrow burrow, uint64_t& minCost, map<uint64_t, uint64_t>& solvedBurrowsMap)
{
	const uint64_t burrowKey = burrow.CreateKey();
	auto solvedPair = solvedBurrowsMap.find(burrowKey);
	if (solvedPair != solvedBurrowsMap.end())
	{
		// Burrow already solved, skip parsing
		minCost = solvedPair->second;
	}
	else if (burrow.IsOrdered())
	{
		minCost = min(burrow.GetEnergyCost(), minCost);
	}
	// Continue parsing burrow as long as it's under the current min
	else if (burrow.GetEnergyCost() < minCost)
	{
		// For each Amphipod in the hallway, try to enter the correct room
		for (const Point2D& hallwaySpot : HALLWAY_SPOTS)
		{
			const BurrowNode& node = burrow[hallwaySpot];
			if (node.IsAmphipod() && burrow.CanEnterRoom(node))
			{
				// Retrieve "deepest" target point in the room
				Point2D roomSpot;
				const int roomColumn = node.GetRoomColumn();
				for (int y = static_cast<int>(burrow.Rows()) - 2; y >= 2; --y)
				{
					roomSpot = Point2D(roomColumn, y);
					if (burrow[roomSpot].IsOpen())
					{
						break;
					}
				}
				Burrow burrowCopy = burrow;
				if (burrowCopy.TryMove(hallwaySpot, roomSpot))
				{
					uint64_t minCostCopy = INT_MAX;
					TryOrderBurrow(burrowCopy, minCostCopy, solvedBurrowsMap);
					solvedBurrowsMap.emplace(burrowCopy.CreateKey(), minCostCopy);
					minCost = min(minCost, minCostCopy);
				}
			}
		}

		// For each Amphipod in a room, try to enter a spot in the hallway
		for (const auto& roomColumnsPair : ROOM_COLUMNS_MAP)
		{
			int roomColumn = roomColumnsPair.second;
			for (int y = 2; y < burrow.Rows() - 1; ++y)
			{
				const Point2D roomSpot = Point2D(roomColumn, y);
				if (burrow.CanExitRoom(roomSpot))
				{
					for (const Point2D& hallwaySpot : HALLWAY_SPOTS)
					{
						Burrow burrowCopy = burrow;
						if (burrowCopy.TryMove(roomSpot, hallwaySpot))
						{
							uint64_t minCostCopy = INT_MAX;
							TryOrderBurrow(burrowCopy, minCostCopy, solvedBurrowsMap);
							solvedBurrowsMap.emplace(burrowCopy.CreateKey(), minCostCopy);
							minCost = min(minCost, minCostCopy);
						}
					}
				}
			}
		}
	}
}

template <>
void Run<Day23>(Part part, istream& is, ostream& os)
{
	string arg;
	
	Burrow burrow;
	int i = 0;
	while(getline(is, arg))
	{
		burrow.Parse(arg);
		++i;
		if (part == Part02 && i == 3)
		{
			burrow.Parse("  #D#C#B#A#  ");
			burrow.Parse("  #D#B#A#C#  ");
		}
	}

	uint64_t minEnergyCost = INT_MAX;
	map<uint64_t, uint64_t> solvedBurrowsMap = {};
	TryOrderBurrow(burrow, minEnergyCost, solvedBurrowsMap);
	os << "Minimum Energy Cost: " << minEnergyCost << endl;
}