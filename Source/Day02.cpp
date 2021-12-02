#include "Days.h"
#include "Point2D.h"
#include <regex>

using namespace std;

const regex RX_COMMAND(R"((\w+) (\d+))");

template <>
void Run<Day02>(Part part, istream& is, ostream& os)
{
	string arg;

	Point2D p = Point2D();
	int aim = 0;
	while (getline(is, arg))
	{
		smatch match;
		regex_search(arg, match, RX_COMMAND);

		string direction = match.str(1);
		int units = stoi(match.str(2));

		if (part == Part01)
		{
			if (direction == "forward")
			{
				p.x += units;
			}
			else if (direction == "down")
			{
				p.y += units;
			}
			else if (direction == "up")
			{
				p.y -= units;
			}
		}
		else if (part == Part02)
		{
			if (direction == "forward")
			{
				p.x += units;
				p.y += aim * units;
			}
			else if (direction == "down")
			{
				aim += units;
			}
			else if (direction == "up")
			{
				aim -= units;
			}
		}
	}
	
	cout << "Final Position: " << p.ToString() << endl;// << p.x << ", " << p.y << ")" << endl;
	cout << "Position Product: " << p.x * p.y << endl;
}