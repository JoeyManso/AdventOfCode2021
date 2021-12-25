#include "Days.h"
#include "AdventMath.h"
#include <algorithm>
#include <map>
#include <regex>
#include <vector>

using namespace std;

const int ROLLS_PER_TURN = 3;
const int SPACE_MIN = 1;
const int SPACE_MAX = 10;
const int DICE_MIN = 1;
static int DICE_MAX;
static int WINNING_SCORE;

// Sum of three rolls of the Dirac Die
const vector<int> POTENTIAL_TURN_VALUES = {3,4,5,6,7,8,9};
// Frequency of each sum - an optimization.
const vector<int> FREQUENCY_SCALE = {1,3,6,7,6,3,1};

const regex RX_STARTING_POS(R"(Player (\d+) starting position: (\d+))");

struct Player
{
	Player()
		: Player(0, 0)
	{
	}

	Player(int _num, int _pos)
		: num(_num), pos(_pos), score(0)
	{
	}

	int GetNum() const
	{
		return num;
	}

	int GetPos() const
	{
		return pos;
	}

	int GetScore() const
	{
		return score;
	}

	void Move(int spaces)
	{
		if (spaces > 0)
		{
			pos = ((pos + spaces - 1) % SPACE_MAX) + SPACE_MIN;
			score += pos;
		}
	}

	static uint64_t CreateKey(int num, const vector<Player>& players)
	{
		uint64_t key = num;
		for (const Player& player : players)
		{
			key = (key * 100) + player.GetPos();
			key = (key * 100) + player.GetScore();
		}
		return key;
	}

private:
	int num;
	int pos;
	int score;
};

struct WinCounts
{
	WinCounts(uint64_t _p1WinCounts, uint64_t _p2WinCounts)
		: p1WinCounts(_p1WinCounts), p2WinCounts(_p2WinCounts)
	{
	}
	
	WinCounts()
	{
	}
	
	uint64_t p1WinCounts = 0;
	uint64_t p2WinCounts = 0;

	WinCounts operator*(int s)
	{
		return WinCounts(p1WinCounts * s, p2WinCounts * s);
	}

	WinCounts& operator+=(const WinCounts& other)
	{
		p1WinCounts += other.p1WinCounts;
		p2WinCounts += other.p2WinCounts;
		return *this;
	}
};

WinCounts SimulateGame(
	map<size_t, WinCounts>& solvedGameStates,
	vector<Player> players,
	int turnValue = 0,
	int turn = 0
)
{
	WinCounts winCounts;
	Player& player = turn % 2 == 1 ? players[0] : players[1];
	player.Move(turnValue);
	if (player.GetScore() >= WINNING_SCORE)
	{
		if (turn % 2 == 1)
		{
			++winCounts.p1WinCounts;
		}
		else
		{
			++winCounts.p2WinCounts;
		}
	}
	else
	{
		uint64_t hashKey = Player::CreateKey(player.GetNum(), players);
		if (solvedGameStates.find(hashKey) != solvedGameStates.end())
		{
			return solvedGameStates[hashKey];
		}
		else
		{
			for (size_t i = 0; i < POTENTIAL_TURN_VALUES.size(); ++i)
			{
				winCounts += SimulateGame(solvedGameStates, players, POTENTIAL_TURN_VALUES[i], turn + 1) * FREQUENCY_SCALE[i];
			}
		}
		solvedGameStates.emplace(hashKey, winCounts);
	}
	return winCounts;
}

template <>
void Run<Day21>(Part part, istream& is, ostream& os)
{
	DICE_MAX = part == Part01 ? 100 : 3;
	WINNING_SCORE = part == Part01 ? 1000 : 21;
	vector<Player> players = vector<Player>(2);

	string arg;
	size_t i = 0;
	while (getline(is, arg))
	{
		smatch match;
		regex_search(arg, match, RX_STARTING_POS);
		players[i] = Player(stoi(match.str(1)), stoi(match.str(2)));
		++i;
	}

	if(part == Part01)
	{
		int rollStart = 0;
		int diceRollCount = 0;
		int turn = 1;
		while (true)
		{
			diceRollCount += ROLLS_PER_TURN;
			Player& player = turn % 2 == 1 ? players[0] : players[1];
			int moveSpaces = 0;
			int rollEnd = rollStart;

			for (int i = 0; i < ROLLS_PER_TURN; ++i)
			{
				++rollEnd;
				rollEnd %= ((DICE_MAX - DICE_MIN) + 1);
				moveSpaces += rollEnd;
			}
		
			player.Move(moveSpaces);
			if (player.GetScore() >= WINNING_SCORE)
			{
				break;
			}
			rollStart = rollEnd;
			++turn;
		}
		os << "Dice roll count: " << diceRollCount << endl;
		os << "P1 Score: " << players[0].GetScore() << endl;
		os << "P2 Score: " << players[1].GetScore() << endl;
		uint64_t losingScore = players[0].GetScore() > players[1].GetScore() ? players[1].GetScore() : players[0].GetScore();
		os << diceRollCount << " * " << losingScore << " = " << diceRollCount * losingScore << endl;
	}
	else if(part == Part02)
	{
		map<uint64_t, WinCounts> solvedGameStates = {};
		WinCounts winCounts = SimulateGame(solvedGameStates, players);
		os << "P1 Win Counts: " << winCounts.p1WinCounts << endl;
		os << "P2 Win Counts: " << winCounts.p2WinCounts << endl;
	}
}