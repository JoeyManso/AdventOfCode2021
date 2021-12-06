#include "Days.h"
#include "AdventUtils.h"
#include <vector>

using namespace std;

const int TIMER_RESET = 7;
const int TIMER_BIRTH = 9;

template <>
void Run<Day06>(Part part, istream& is, ostream& os)
{	
	string arg;
	getline(is, arg);
	vector<int> lanternFishTimers = splitInt(arg, ',');
	
	// Gather seed timers, incrementing the respective index
	long long fishCountPerTimers[TIMER_BIRTH] = { 0 };
	for (int timer : lanternFishTimers)
	{
		++fishCountPerTimers[timer];
	}
	
	const int simulationDays = part == Part01 ? 80 : 256;
	long long totalFish = lanternFishTimers.size();
	
	// Run simulation for required days
	// At each timer step, migrate fish counts and add spawned fish
	for (int day = 0; day < simulationDays; ++day)
	{
		long long spawnedFish = fishCountPerTimers[0];
		for (int i = 0; i < TIMER_BIRTH - 1; ++i)
		{
			fishCountPerTimers[i] = fishCountPerTimers[i + 1];
		}
		
		// Add newly spawned fish
		fishCountPerTimers[TIMER_RESET - 1] += spawnedFish;
		fishCountPerTimers[TIMER_BIRTH - 1] = spawnedFish;
		totalFish += spawnedFish;
	}
	cout << "Lanternfish after " << simulationDays << " days: " << totalFish << endl;
}