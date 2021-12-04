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