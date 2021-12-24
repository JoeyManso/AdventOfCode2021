#include "Days.h"
#include "AdventUtils.h"
#include <vector>

using namespace std;

char GetOutputPixel(const string& enhancementAlgorithm, const vector<vector<char>>& inputImage, const char infinitePixel, int targetX, int targetY)
{
	string sequenceBin = "";
	for (int y = targetY - 1; y <= targetY + 1; ++y)
	{
		for (int x = targetX - 1; x <= targetX + 1; ++x)
		{
			// Default to infinite pixel
			char inputPixel = infinitePixel;
			if (x >= 0 && x < inputImage.size() && y >= 0 && y < inputImage[x].size())
			{
				// Assign pixel from input if available
				inputPixel = inputImage[x][y];
			}
			sequenceBin.push_back(inputPixel == '#' ? '1' : '0');
		}
	}
	size_t sequenceDec = BinaryToDecimal<size_t>(sequenceBin);
	return enhancementAlgorithm[sequenceDec];
}

vector<vector<char>> GetOutputImage(const string& enhancementAlgorithm, const vector<vector<char>>& inputImage, char infinitePixel)
{
	const int outputPadding = 3;
	const int inputImageGrid = static_cast<int>(inputImage.size());
	const int outputImageGrid = inputImageGrid + outputPadding * 2;
	vector<vector<char>> outputImage = {};
	outputImage.resize(outputImageGrid);
	for (int outX = 0; outX < outputImageGrid; ++outX)
	{
		for (int outY = 0; outY < outputImageGrid; ++outY)
		{
			if (outputImage[outX].size() < outputImageGrid)
			{
				outputImage[outX].resize(outputImageGrid);
			}
			outputImage[outX][outY] = GetOutputPixel(enhancementAlgorithm, inputImage, infinitePixel, outX - outputPadding, outY - outputPadding);
		}
	}
	return outputImage;
}

size_t GetLitPixelsCount(const vector<vector<char>>& image)
{
	size_t litPixelsCount = 0;
	for (size_t x = 0; x < image.size(); ++x)
	{
		for (size_t y = 0; y < image[x].size(); ++y)
		{
			if (image[x][y] == '#')
				++litPixelsCount;
		}
	}
	return litPixelsCount;
}

void Print(ostream& os, const vector<vector<char>>& image)
{
	for (int x = 0, y = 0; y < image[x].size(); ++y)
	{
		for (; x < image.size(); ++x)
		{
			os << image[x][y];
		}
		x = 0;
		os << endl;
	}
	os << endl;
}

template <>
void Run<Day20>(Part part, istream& is, ostream& os)
{
	string enhancementAlgorithm = "";
	vector<vector<char>> inputImage = {};
	
	string arg;
	while (getline(is, arg))
	{
		if (enhancementAlgorithm.size() < 512)
		{
			enhancementAlgorithm.append(arg);
		}
		else
		{
			for (size_t x = 0; x < arg.length(); ++x)
			{
				if (inputImage.size() <= x)
				{
					inputImage.push_back({});
				}
				inputImage[x].push_back(arg[x]);
			}
		}
	}

	//os << "Input Image:" << endl;
	//Print(os, inputImage);

	size_t iterationsMax = part == Part01 ? 2 : 50;
	char infinitePixel = '.';
	for (size_t i = 1; i <= iterationsMax; ++i)
	{
		auto outputImage = GetOutputImage(enhancementAlgorithm, inputImage, infinitePixel);
		os << "Enhance " << i << " Lit Count: " << GetLitPixelsCount(outputImage) << endl;
		//os << "Output Image:" << endl;
		//Print(os, outputImage);
		inputImage = outputImage;
		infinitePixel = infinitePixel == '.' ? enhancementAlgorithm.front() : enhancementAlgorithm.back();
	}
}