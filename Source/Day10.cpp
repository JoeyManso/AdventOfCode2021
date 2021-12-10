#include "Days.h"
#include <algorithm>
#include <stack>
#include <vector>

using namespace std;
template <>
void Run<Day10>(Part part, istream& is, ostream& os)
{
	long long errorPointsTotal = 0;
	vector <long long> completePointsArray = {};
	
	string arg;
	while (getline(is, arg))
	{
		bool error = false;
		stack<char> characterStack = {};
		for (char c : arg)
		{
			switch (c)
			{
				case '(':
				case '[':
				case '{':
				case '<':
					characterStack.push(c);
					break;
				case ')':
					if (characterStack.top() == '(')
					{
						characterStack.pop();
					}
					else
					{
						errorPointsTotal += 3;
						error = true;
					}
					break;
				case ']':
					if (characterStack.top() == '[')
					{
						characterStack.pop();
					}
					else
					{
						errorPointsTotal += 57;
						error = true;
					}
					break;
				case '}':
					if (characterStack.top() == '{')
					{
						characterStack.pop();
					}
					else
					{
						errorPointsTotal += 1197;
						error = true;
					}
					break;
				case '>':
					if (characterStack.top() == '<')
					{
						characterStack.pop();
					}
					else
					{
						errorPointsTotal += 25137;
						error = true;
					}
					break;
			}
			if (error)
			{
				break;
			}
		}

		if (!error)
		{
			long long completePoints = 0;
			while (characterStack.size() > 0)
			{
				completePoints *= 5;
				switch (characterStack.top())
				{
					case '(':
						completePoints += 1;
						break;
					case '[':
						completePoints += 2;
						break;
					case '{':
						completePoints += 3;
						break;
					case '<':
						completePoints += 4;
						break;
				}
				characterStack.pop();
			}
			completePointsArray.push_back(completePoints);
		}
	}

	if(part == Part01)
	{
		cout << "Error Points: " << errorPointsTotal << endl;
	}
	else if(part == Part02)
	{
		sort(completePointsArray.begin(), completePointsArray.end());
		size_t mid = completePointsArray.size() / 2;
		cout << "Complete Points: " << completePointsArray[mid] << endl;
	}
}