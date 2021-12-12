#include "Days.h"
#include "AdventUtils.h"
#include <map>
#include <regex>
#include <set>
#include <vector>

using namespace std;

const regex RX_CAVE_CONNECTION(R"((\w+)-(\w+))");
const string CAVE_NAME_START = "start";
const string CAVE_NAME_END = "end";

struct CaveNode
{
	CaveNode(const string& _caveName)
		: caveName(_caveName)
	{
	}

	const string& GetName() const
	{
		return caveName;
	}

	bool IsSmallCave() const
	{
		return islower(caveName[0]);
	}

	void AddConnection(const CaveNode& connection)
	{
		connections.emplace(&connection);
	}

	void FindTargetCavePaths(const CaveNode& target, set<vector<const CaveNode*>>& paths, vector<const CaveNode*> path, Part part) const
	{
		path.push_back(this);
		if (this == &target)
		{
			// If target is reached, add the completed path and stop searching
			paths.emplace(path);
		}
		else
		{
			for (const CaveNode* connection : connections)
			{
				bool bCanVisit = true;
				if(part == Part01)
				{
					// Don't visit small caves more than once per path
					bCanVisit = !connection->IsSmallCave() || !contains(path, connection);
				}
				else if(part == Part02)
				{
					// Don't visit a single small cave more than twice per path (except start/end)
					if (connection->IsSmallCave() && contains(path, connection))
					{
						if (connection->GetName() == CAVE_NAME_START || connection->GetName() == CAVE_NAME_END)
						{
							bCanVisit = false;
						}
						else
						{
							set<const CaveNode*> vistedSmallCaves = {};
							for (const CaveNode* cave : path)
							{
								if (cave->IsSmallCave())
								{
									if (vistedSmallCaves.find(cave) == vistedSmallCaves.end())
									{
										vistedSmallCaves.emplace(cave);
									}
									else
									{
										// A small cave has already been visited twice
										bCanVisit = false;
										break;
									}
								}
							}
						}
					}
				}
				if (bCanVisit)
				{
					connection->FindTargetCavePaths(target, paths, path, part);
				}
			}
		}
	}

private:
	set<const CaveNode*> connections;
	string caveName;
};

template <>
void Run<Day12>(Part part, istream& is, ostream& os)
{
	// Mapping of cave name to its node
	map<string, CaveNode*> caveNodeMap;

	string arg;
	while (getline(is, arg))
	{
		smatch match;
		regex_search(arg, match, RX_CAVE_CONNECTION);
		
		string caveName1 = match.str(1), caveName2 = match.str(2);
		if (caveNodeMap.find(caveName1) == caveNodeMap.end())
		{
			caveNodeMap.emplace(caveName1, new CaveNode(caveName1));
		}
		if (caveNodeMap.find(caveName2) == caveNodeMap.end())
		{
			caveNodeMap.emplace(caveName2, new CaveNode(caveName2));
		}
		caveNodeMap[caveName1]->AddConnection(*caveNodeMap[caveName2]);
		caveNodeMap[caveName2]->AddConnection(*caveNodeMap[caveName1]);
	}

	set<vector<const CaveNode*>> paths = {};
	caveNodeMap[CAVE_NAME_START]->FindTargetCavePaths(*caveNodeMap[CAVE_NAME_END], paths, {}, part);
	os << "Total Cave Paths: " << paths.size() << endl;
}