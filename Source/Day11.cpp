#include "Days.h"
#include "AdventUtils.h"
#include <vector>

using namespace std;

int totalFlashCount = 0;
vector<vector<int>> octopusEnergy = {};

void HandleFlash(int x, int y, bool bCanIncrement)
{
	if (x >= 0 && x < octopusEnergy.size() && y >= 0 && y < octopusEnergy[x].size())
	{
		if (bCanIncrement && octopusEnergy[x][y] != 0)
		{
			++octopusEnergy[x][y];
		}
		if (octopusEnergy[x][y] > 9)
		{
			octopusEnergy[x][y] = 0;
			++totalFlashCount;

			// Up
			HandleFlash(x, y - 1, true);
			// Up-Right
			HandleFlash(x + 1, y - 1, true);
			// Right
			HandleFlash(x + 1, y, true);
			// Down-Right
			HandleFlash(x + 1, y + 1, true);
			// Down
			HandleFlash(x, y + 1, true);
			// Down-Left
			HandleFlash(x - 1, y + 1, true);
			// Left
			HandleFlash(x - 1, y, true);
			// Up-Left
			HandleFlash(x - 1, y - 1, true);
		}
	}
}

void PrintEnergy(ostream& os)
{
	for (int x = 0, y = 0; y < octopusEnergy[x].size(); ++y)
	{
		for (; x < octopusEnergy.size(); ++x)
		{
			os << octopusEnergy[x][y];
		}
		x = 0;
		os << endl;
	}
	os << endl;
}

template <>
void Run<Day11>(Part part, istream& is, ostream& os)
{
	string arg;

	totalFlashCount = 0;
	octopusEnergy.clear();
	while (getline(is, arg))
	{
		for (int x = 0; x < arg.length(); ++x)
		{
			if (octopusEnergy.size() <= x)
			{
				octopusEnergy.push_back(vector<int>());
			}
			octopusEnergy[x].push_back(CharToInt(arg[x]));
		};
	}
	
	//os << "Start" << endl;
	//PrintEnergy(os);
	const int TOTAL_STEPS = part == Part01 ? 100 : 500;
	for (int step = 1; step <= TOTAL_STEPS; ++step)
	{
		// Increment each energy by one
		for (int x = 0; x < octopusEnergy.size(); ++x)
		{
			for (int y = 0; y < octopusEnergy[x].size(); ++y)
			{
				++octopusEnergy[x][y];
			}
		}

		// Increment energy adjacent to flashed octopuses
		for (int x = 0; x < octopusEnergy.size(); ++x)
		{
			for (int y = 0; y < octopusEnergy[x].size(); ++y)
			{
				HandleFlash(x, y, false);
			}
		}

		if(part == Part02)
		{
			bool bAllFlashed = true;
			for (int x = 0; x < octopusEnergy.size(); ++x)
			{
				for (int y = 0; y < octopusEnergy[x].size(); ++y)
				{
					bAllFlashed &= octopusEnergy[x][y] == 0;
				}
			}
			
			if (bAllFlashed)
			{
				os << "All Flashed on Step: " << step << endl;
			}
		}
		
		//os << "After Step: " << step << endl;
		//PrintEnergy(os);
	}

	if(part == Part01)
	{
		cout << "Total Octopus Flash Count after " << TOTAL_STEPS << " steps: " << totalFlashCount << endl;
	}
}