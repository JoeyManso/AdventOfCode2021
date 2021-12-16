#ifndef ADVENT_UTILS_H
#define ADVENT_UTILS_H

#include <algorithm>
#include <cctype>
#include <iterator>
#include <sstream>
#include <vector>

inline char IntToChar(const int& i)
{
	return('0' + i);
}

inline char CharToInt(const char& c)
{
	return (c - '0');
}

static inline bool IsNumber(const string& s)
{
    return find_if(s.begin(),  s.end(), [](unsigned char c) { return !::isdigit(c); }) == s.end();
}

inline vector<string> split(string str, char delimiter)
{
	vector<string> outArray;
	stringstream ss(str); // Turn the string into a stream.
	string tok;

	while(getline(ss, tok, delimiter))
	{
		if (tok.length() > 0)
		{
			outArray.push_back(tok);
		}
	}
	return outArray;
}

inline vector<int> splitInt(string str, char delimiter)
{
	vector<string> stringArray = split(str, delimiter);
	vector<int> outArray;
	for (const string& s : stringArray)
	{
		if (s.length() > 0)
		{
			outArray.push_back(stoi(s.c_str()));
		}
	}
	return outArray;
}

template <typename T>
inline T BinaryToDecimal(const string& binaryStr)
{
	T dec = 0;
	size_t exponent = static_cast<size_t>(binaryStr.length()) - 1;
	for (size_t i = 0; i < binaryStr.length(); ++i)
	{
		T bitValue = CharToInt(binaryStr[i]);
		dec += bitValue * static_cast<T>(pow(2, exponent));
		--exponent;
	}
	return dec;
}

inline string HexidecimalToBinary(const string& hexStr)
{
    string binaryStr = "";
    for (size_t i = 0; i < hexStr.length(); ++i)
    {
        switch (hexStr[i]) 
        {
            case '0':
                binaryStr.append("0000");
                break;
            case '1':
                binaryStr.append("0001");
                break;
            case '2':
                binaryStr.append("0010");
                break;
            case '3':
                binaryStr.append("0011");
                break;
            case '4':
                binaryStr.append("0100");
                break;
            case '5':
                binaryStr.append("0101");
                break;
            case '6':
                binaryStr.append("0110");
                break;
            case '7':
                binaryStr.append("0111");
                break;
            case '8':
                binaryStr.append("1000");
                break;
            case '9':
                binaryStr.append("1001");
                break;
            case 'A':
            case 'a':
                binaryStr.append("1010");
                break;
            case 'B':
            case 'b':
                binaryStr.append("1011");
                break;
            case 'C':
            case 'c':
                binaryStr.append("1100");
                break;
            case 'D':
            case 'd':
                binaryStr.append("1101");
                break;
            case 'E':
            case 'e':
                binaryStr.append("1110");
                break;
            case 'F':
            case 'f':
                binaryStr.append("1111");
                break;
            default:
                cout << "\nInvalid hexadecimal digit " << hexStr[i];
        }
    }
    return binaryStr;
}

// trim from start (in place)
static inline void ltrim(string &s)
{
	s.erase(s.begin(), find_if(s.begin(), s.end(), [](int ch)
	{
		return !isspace(ch);
	}));
}

// trim from end (in place)
static inline void rtrim(string &s)
{
	s.erase(find_if(s.rbegin(), s.rend(), [](int ch)
	{
		return !isspace(ch);
	}).base(), s.end());
}

template <typename T>
inline bool contains(vector<T> v, T x)
{
	return(find(v.begin(), v.end(), x) != v.end());
}

#endif // ADVENT_UTILS_H