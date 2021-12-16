#include "Days.h"
#include "AdventUtils.h"
#include <map>
#include <vector>

using namespace std;

enum BitCriteria
{
	Gamma,
	Epsilon,
	Oxygen,
	C02,
};

map<int, int> GetBitCountMap(const vector<string>& binaryStringArray, size_t bitIndex)
{
	map<int, int> bitCountMap = {
		{0, 0},
		{1, 0},
	};
	for (const string& binaryStr : binaryStringArray)
	{
		int bitValue = CharToInt(binaryStr[bitIndex]);
		bitCountMap[bitValue]++;
	}
	return bitCountMap;
}

string GetBinaryStringForCriteria(vector<string> binaryStringArray, size_t binaryStringLen, BitCriteria criteria)
{
	string binaryStr = "";
	for (size_t i = 0; i < binaryStringLen && binaryStringArray.size() > 1; ++i)
	{
		const map<int, int> bitCountMap = GetBitCountMap(binaryStringArray, i);
		int favoredBit = -1;
		if (bitCountMap.at(0) > bitCountMap.at(1))
		{
			switch (criteria)
			{
				case Gamma:
				case Oxygen:
					favoredBit = 1;
					break;
				case Epsilon:
				case C02:
					favoredBit = 0;
					break;
			}
		}
		else
		{
			switch (criteria)
			{
				case Gamma:
				case Oxygen:
					favoredBit = 0;
					break;
				case Epsilon:
				case C02:
					favoredBit = 1;
					break;
			}
		}
		switch (criteria)
		{
			case Gamma:
			case Epsilon:
				binaryStr.push_back(IntToChar(favoredBit));
				break;
			case Oxygen:
			case C02:
				for (size_t j = 0; j < binaryStringArray.size(); ++j)
				{
					if (binaryStringArray.size() == 1)
					{
						binaryStr = binaryStringArray[0];
					}
					else if (binaryStringArray.size() > j)
					{
						const string& binaryStr = binaryStringArray[j];
						if (CharToInt(binaryStr[i]) != favoredBit)
						{
							binaryStringArray.erase(binaryStringArray.begin() + j);
							--j;
						}
					}
				}
				break;
		}
		
	}
	return binaryStr;
}

template <>
void Run<Day03>(Part part, istream& is, ostream& os)
{
	string arg;

	// Array of input binary strings
	vector<string> binaryStringArray;

	// Length of each binary string
	size_t binaryStringLen = 0;
	
	while(getline(is, arg))
	{
		binaryStringArray.push_back(arg);
	}
	binaryStringLen = binaryStringArray[0].length();

	if(part == Part01)
	{
		string gammaRateBinaryStr = GetBinaryStringForCriteria(binaryStringArray, binaryStringLen, Gamma);
		string epsilonRateBinaryStr = GetBinaryStringForCriteria(binaryStringArray, binaryStringLen, Epsilon);
		int gammaRateDec = BinaryToDecimal<int>(gammaRateBinaryStr);
		int epsilonRateDec = BinaryToDecimal<int>(epsilonRateBinaryStr);
		cout << "Gamma Rate: Binary - " << gammaRateBinaryStr << ", Decimal - " << gammaRateDec << endl;
		cout << "Epsilon Rate: Binary - " << epsilonRateBinaryStr << ", Decimal - " << epsilonRateDec << endl;
		cout << "Power Consumption: " << gammaRateDec * epsilonRateDec << endl;
	}
	else if(part == Part02)
	{
		string oxygenBinaryStr = GetBinaryStringForCriteria(binaryStringArray, binaryStringLen, Oxygen);
		string c02BinaryStr = GetBinaryStringForCriteria(binaryStringArray, binaryStringLen, C02);
		int oxygenRateDec = BinaryToDecimal<int>(oxygenBinaryStr);
		int c02RateDec = BinaryToDecimal<int>(c02BinaryStr);
		cout << "Oxygen Rate: Binary - " << c02BinaryStr << ", Decimal - " << oxygenRateDec << endl;
		cout << "C02 Rate: Binary - " << c02BinaryStr << ", Decimal - " << c02RateDec << endl;
		cout << "Life Support Rating: " << oxygenRateDec * c02RateDec << endl;
	}
}