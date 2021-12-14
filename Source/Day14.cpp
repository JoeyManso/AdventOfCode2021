#include "Days.h"
#include <map>
#include <regex>
#include <vector>

using namespace std;

const regex RX_RULE(R"((\w+) -> (\w+))");

template <>
void Run<Day14>(Part part, istream& is, ostream& os)
{
	map<string, uint64_t> polymerPairsMap = {};
	map<char, uint64_t> characterCountMap = {};
	{
		string polymer;
		getline(is, polymer);
		for (int i = 0; i < polymer.length(); ++i)
		{
			if (i < polymer.length() - 1)
			{
				const string pair = polymer.substr(i, 2);
				if (polymerPairsMap.find(pair) == polymerPairsMap.end())
				{
					polymerPairsMap.emplace(pair, 1);
				}
			}
			if (characterCountMap.find(polymer[i]) == characterCountMap.end())
			{
				characterCountMap.emplace(polymer[i], 0);
			}
			++characterCountMap[polymer[i]];
		}
	}

	map<string, char> insertionRuleMap = {};
	{
		string arg;
		while (getline(is, arg))
		{
			smatch matchRule;
			if (regex_search(arg, matchRule, RX_RULE))
			{
				insertionRuleMap.emplace(matchRule.str(1), matchRule.str(2)[0]);
			}
		}
	}

	const int STEPS_MAX = part == Part01 ? 10 : 40;
	for (int step = 1; step <= STEPS_MAX; ++step)
	{
		map<string, uint64_t> polymerPairsMapNew = polymerPairsMap;
		for (const auto& polymerPairsMapIt : polymerPairsMap)
		{
			if (polymerPairsMapIt.second > 0)
			{
				const auto insertionRule = insertionRuleMap.find(polymerPairsMapIt.first);
				if (insertionRule != insertionRuleMap.end())
				{
					char insertionChar = insertionRule->second;
					string newPair1, newPair2;
					newPair1.push_back(polymerPairsMapIt.first[0]);
					newPair1.push_back(insertionChar);
					newPair2.push_back(insertionChar);
					newPair2.push_back(polymerPairsMapIt.first[1]);
				
					// Subtract count for the now split pair
					polymerPairsMapNew[polymerPairsMapIt.first] -= polymerPairsMapIt.second;
				
					// Add count for the newly created pairs
					if (polymerPairsMapNew.find(newPair1) == polymerPairsMapNew.end())
					{
						polymerPairsMapNew.emplace(newPair1, 0);
					}
					if (polymerPairsMapNew.find(newPair2) == polymerPairsMapNew.end())
					{
						polymerPairsMapNew.emplace(newPair2, 0);
					}
					polymerPairsMapNew[newPair1] += polymerPairsMapIt.second;
					polymerPairsMapNew[newPair2] += polymerPairsMapIt.second;

					if (characterCountMap.find(insertionChar) == characterCountMap.end())
					{
						characterCountMap.emplace(insertionChar, 0);
					}
					characterCountMap[insertionChar] += polymerPairsMapIt.second;
				}
			}
		}
		polymerPairsMap = polymerPairsMapNew;
	}

	os << "Character Counts: " << endl;
	uint64_t minCount = UINT64_MAX, maxCount = 0;
	for (const auto& characterCountPair : characterCountMap)
	{
		os << characterCountPair.first << ": " << characterCountPair.second << endl;
		minCount = min(minCount, characterCountPair.second);
		maxCount = max(maxCount, characterCountPair.second);
	}
	os << "Subtracted Max/Min: " << maxCount - minCount << endl;
}