#include "Days.h"
#include "AdventUtils.h"
#include <vector>

using namespace std;

int GetFuelCost(const vector<int>& crabPositions, int targetPosition, Part part)
{
	int fuelCost = 0;
	for (int crabPosition : crabPositions)
	{
		int positionDelta = abs(targetPosition - crabPosition);
		if(part == Part01)
		{
			fuelCost += positionDelta;
		}
		else if(part == Part02 && positionDelta > 0)
		{
			for (int i = 1; i <= positionDelta; ++i)
			{
				fuelCost += i;
			}
		}
	}
	return fuelCost;
}

template <>
void Run<Day07>(Part part, istream& is, ostream& os)
{ 
	string arg;
	getline(is, arg);
	vector<int> crabPositions = splitInt(arg, ',');

	int positionSum = 0;
	for (int crabPosition : crabPositions)
	{
		positionSum += crabPosition;
	}
	int positionAverage = positionSum / static_cast<int>(crabPositions.size());
	int lowestFuelCost = INT_MAX;
	int targetPosition = INT_MIN;
	
	// Search low from the average
	for (int i = positionAverage; i >= 0; --i)
	{
		int newFuelCost = GetFuelCost(crabPositions, i, part);
		if (newFuelCost < lowestFuelCost)
		{
			lowestFuelCost = newFuelCost;
			targetPosition = i;
		}
		else
		{
			break;
		}
	}
	// Search high
	for (int i = positionAverage + 1; i < positionSum; ++i)
	{
		int newFuelCost = GetFuelCost(crabPositions, i, part);
		if (newFuelCost < lowestFuelCost)
		{
			lowestFuelCost = newFuelCost;
			targetPosition = i;
		}
		else
		{
			break;
		}
	}
	cout << " TargetPosition: " << targetPosition << " LowestFuelCost: " << lowestFuelCost << endl;
}