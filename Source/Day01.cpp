#include "Days.h"
#include <vector>

using namespace std;

template <>
void Run<Day01>(Part part, istream& is, ostream& os)
{
	string arg;
	vector<int32_t> inputNumbers = {};
	while(getline(is, arg))
	{
		inputNumbers.push_back(stoi(arg));
	}
	
	size_t increaseTotal = 0;
	if(part == Part01)
	{
		for (int32_t i = 1; i < inputNumbers.size(); ++i)
		{
			//cout << inputNumbers[i];
			if (inputNumbers[i] > inputNumbers[i - 1])
			{
				//cout << " (increased)" << endl;
				++increaseTotal;
			}
			else
			{
				//cout << " (decreased)" << endl;
			}
		}
	}
	else if(part == Part02)
	{
		for (int32_t i = 3; i < inputNumbers.size(); ++i)
		{
			int32_t sumCur = inputNumbers[i - 2] + inputNumbers[i - 1] + inputNumbers[i];
			int32_t sumPrev = inputNumbers[i - 3] + inputNumbers[i - 2] + inputNumbers[i - 1];
			//cout << sumCur;
			if (sumCur > sumPrev)
			{
				//cout << " (increased)" << endl;
				++increaseTotal;
			}
			else
			{
				//cout << " (decreased)" << endl;
			}
		}
	}

	cout << endl << "Increase Total: " << increaseTotal;
}