#include "Days.h"
#include <regex>
#include <vector>

using namespace std;

const regex RX_DIV(R"(div [a-z] (-?\d+))");
const regex RX_ADD(R"(add [a-z] (-?\d+))");

struct ConstValues
{
    int div;
    int add1;
    int add2;
};

static vector<ConstValues> constValuesArray = {};

int64_t GetMaxZOut(int prog)
{
    int64_t z = 1;
    for (int p = prog; p < 14; ++p)
    {
        z *= constValuesArray[p].div;
    }
    return z;
}

// Each stage in input code basically boils down to this function. The values of 
// the X and Y registers are irrelevant. We only care about the digit read into W and 
// the existing value in Z, Zin. It turns out that DIV is either 1 or 26. Zout will be 
// at most ~Zin * 26.
int64_t calc_zout(int prog, int64_t digit, int64_t zin)
{
    const ConstValues& constValues = constValuesArray[prog];
    if (((zin % 26) + constValues.add1) == digit)
        return zin / constValues.div;
    return (zin / constValues.div) * 26 + digit + constValues.add2;
}

// DFS to find the MONAD we need.
void solve(int prog, int64_t zin, int64_t working, bool& done, int64_t& result, bool up)
{   
    if (prog == 14)
    {
        if (zin == 0)
        {
            done   = true;
            result = working;
        }
        return;
    }

    if (done) return;

    if (zin >= GetMaxZOut(prog))
        return;

    // Part 1 counts digits downwards to find largest solution first.
    // Part 2 counts digits upwards to find smallest solution first.
    int wfrom = up ?  1 :  9;
    int wto   = up ? 10 :  0;
    int wstep = up ?  1 : -1;

    for (int digit = wfrom; digit != wto; digit += wstep)
    {
        auto zout = calc_zout(prog, digit, zin);
        auto working2 = working * 10 + digit;
        solve(prog+1, zout, working2, done, result, up);
    }
}

template <>
void Run<Day24>(Part part, istream& is, ostream& os)
{
    constValuesArray.clear();
    
    string arg;
	size_t i = 0;
	while (getline(is, arg))
	{
        smatch match;
        switch (i)
        {
            case 4:
                constValuesArray.push_back({});
                regex_search(arg, match, RX_DIV);
                constValuesArray.back().div = stoi(match.str(1));
                break;
            case 5:
                regex_search(arg, match, RX_ADD);
                constValuesArray.back().add1 = stoi(match.str(1));
                break;
            case 15:
                regex_search(arg, match, RX_ADD);
                constValuesArray.back().add2 = stoi(match.str(1));
                break;
            case 18:
                i = 0;
                break;
        }
		++i;
	}

    bool done = false;
    int64_t result = 0;
    
    // Taken from UnicycleBloke because I don't care for this problem
    solve(0, 0, 0, done, result, part == Part02);
    os << result << endl;
}