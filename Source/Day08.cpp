#include "Days.h"
#include "AdventUtils.h"
#include <algorithm>
#include <regex>
#include <set>
#include <vector>

using namespace std;

const regex RX_PATTERN(R"(([a-g]+))");
const int SIGNALS_COUNT = 10;

set<char> StringToSet(const string& str)
{
	set<char> charSet = {};
	for (char c : str)
	{
		charSet.emplace(c);
	}
	return charSet;
}

struct SegmentDisplay
{
	void AddSignalPattern(const string& str)
	{
		set<char> numberPattern = StringToSet(str);
		switch (str.size())
		{
			case 2:
				numberPatterns[1] = numberPattern;
				break;
			case 3:
				numberPatterns[7] = numberPattern;
				break;
			case 4:
				numberPatterns[4] = numberPattern;
				break;
			case 7:
				numberPatterns[8] = numberPattern;
				break;
			case 5:
				// 2, 3, 5
			case 6:
				// 0, 6, 9
			default:
				numberPatternPool.push_back(numberPattern);
				break;
		}
	}

	void AddOutputPattern(const string& str)
	{
		outputPatterns.push_back(str);
	}

	const vector<string>& GetOutputPatterns() const
	{
		return outputPatterns;
	}

	int GetOutputNumber()
	{
		for (int i = 0; i < numberPatternPool.size();)
		{
			switch (numberPatternPool[i].size())
			{
				case 5:
					{
						set<char> intersectOne, intersectFour, intersectSeven;
						set_intersection(numberPatternPool[i].begin(), numberPatternPool[i].end(), numberPatterns[1].begin(), numberPatterns[1].end(), inserter(intersectOne, intersectOne.begin()));
						set_intersection(numberPatternPool[i].begin(), numberPatternPool[i].end(), numberPatterns[4].begin(), numberPatterns[4].end(), inserter(intersectFour, intersectFour.begin()));
						set_intersection(numberPatternPool[i].begin(), numberPatternPool[i].end(), numberPatterns[7].begin(), numberPatterns[7].end(), inserter(intersectSeven, intersectSeven.begin()));
						if(intersectOne.size() == 2 && intersectSeven.size() == 3)
						{
							// Pattern "3" will have 2 intersections with Pattern "1" and 3 intersections with Pattern "7"
							numberPatterns[3] = numberPatternPool[i];
							numberPatternPool.erase(numberPatternPool.begin() + i);
							--i;
						}
						else if (intersectOne.size() == 1 && intersectFour.size() == 2)
						{
							// Pattern "2" will have 1 intersections with Pattern "1" and 2 intersections with Pattern "4"
							numberPatterns[2] = numberPatternPool[i];
							numberPatternPool.erase(numberPatternPool.begin() + i);
							--i;
						}
						else if (intersectOne.size() == 1 && intersectFour.size() == 3)
						{
							// Pattern "5" will have 1 intersections with Pattern "1" and 3 intersections with Pattern "4"
							numberPatterns[5] = numberPatternPool[i];
							numberPatternPool.erase(numberPatternPool.begin() + i);
							--i;
						}
					}
					break;
				case 6:
					{
						set<char> intersectOne, intersectFour, intersectSeven;
						set_intersection(numberPatternPool[i].begin(), numberPatternPool[i].end(), numberPatterns[1].begin(), numberPatterns[1].end(), inserter(intersectOne, intersectOne.begin()));
						set_intersection(numberPatternPool[i].begin(), numberPatternPool[i].end(), numberPatterns[4].begin(), numberPatterns[4].end(), inserter(intersectFour, intersectFour.begin()));
						set_intersection(numberPatternPool[i].begin(), numberPatternPool[i].end(), numberPatterns[7].begin(), numberPatterns[7].end(), inserter(intersectSeven, intersectSeven.begin()));
						if(intersectOne.size() == 1 && intersectSeven.size() == 2)
						{
							// Pattern "6" will have 1 intersections with Pattern "1" and 2 intersections with Pattern "7"
							numberPatterns[6] = numberPatternPool[i];
							numberPatternPool.erase(numberPatternPool.begin() + i);
							--i;
						}
						else if (intersectOne.size() == 2 && intersectFour.size() == 3)
						{
							// Pattern "0" will have 2 intersections with Pattern "1" and 3 intersections with Pattern "4"
							numberPatterns[0] = numberPatternPool[i];
							numberPatternPool.erase(numberPatternPool.begin() + i);
							--i;
						}
						else if (intersectFour.size() == 4)
						{
							// Pattern "9" will have 4 intersections with Pattern "4"
							numberPatterns[9] = numberPatternPool[i];
							numberPatternPool.erase(numberPatternPool.begin() + i);
							--i;
						}
					}
					break;
			}
			i >= numberPatternPool.size() - 1 ? i = 0 : ++i;
		}

		//const set<char> NUMBER_PATTERNS[] = {
		//	{'c', 'a', 'g', 'e', 'd', 'b'},
		//	{'a', 'b'},
		//	{'g', 'c', 'd', 'f', 'a'},
		//	{'f', 'b', 'c', 'a', 'd'},
		//	{'e', 'a', 'f', 'b'},
		//	{'c', 'd', 'f', 'b', 'e'},
		//	{'c', 'd', 'f', 'g', 'e', 'b'},
		//	{'d', 'a', 'b'},
		//	{'a', 'c', 'e', 'd', 'g', 'f', 'b'},
		//	{'c', 'e', 'f', 'a', 'b', 'd'},
		//};

		
		string outputNumberStr = "";
		for (const string& outputPattern : outputPatterns)
		{
			set<char> outputPatternSet = StringToSet(outputPattern);
			for (int i = 0; i < SIGNALS_COUNT; ++i)
			{
				if (outputPatternSet == numberPatterns[i])
				{
					outputNumberStr.push_back(IntToChar(i));
					break;
				}
			}
		}
		return stoi(outputNumberStr);
	}

private:
	vector<string> outputPatterns;
	set<char> numberPatterns[SIGNALS_COUNT] = { set<char>{} };
	vector<set<char>> numberPatternPool;
};

template <>
void Run<Day08>(Part part, istream& is, ostream& os)
{
	vector<SegmentDisplay> segmentDisplays;

	string arg;
	while (getline(is, arg))
	{
		smatch match;
		SegmentDisplay segmentDisplay = SegmentDisplay();
		int i = 0;
		while (regex_search(arg, match, RX_PATTERN))
		{
			if (i < SIGNALS_COUNT)
			{
				segmentDisplay.AddSignalPattern(match.str(1));
				++i;
			}
			else
			{
				segmentDisplay.AddOutputPattern(match.str(1));
			}
			arg = match.suffix();
		}
		segmentDisplays.push_back(segmentDisplay);
	}

	if(part == Part01)
	{
		int uniqueSegmentCount = 0;
		for (const SegmentDisplay& segmentDisplay : segmentDisplays)
		{
			for (const string& outputPattern : segmentDisplay.GetOutputPatterns())
			{
				if (outputPattern.size() == 2 || outputPattern.size() == 3 || outputPattern.size() == 4 || outputPattern.size() == 7)
				{
					++uniqueSegmentCount;
				}
			}
		}
		cout << "Unique Segment Count: " << uniqueSegmentCount << endl;
	}
	else if(part == Part02)
	{
		int outputNumbersSumTotal = 0;
		for (SegmentDisplay& segmentDisplay : segmentDisplays)
		{
			outputNumbersSumTotal += segmentDisplay.GetOutputNumber();
		}
		cout << "Output Numbers Sum Total: " << outputNumbersSumTotal << endl;
	}
}