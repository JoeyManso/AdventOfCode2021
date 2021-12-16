#include "Days.h"
#include "AdventUtils.h"
#include <vector>

using namespace std;

struct PacketResult
{
	PacketResult()
		:  bits(0), versionSum(0), value(0)
	{
	}
	
	size_t bits;
	uint64_t versionSum;
	uint64_t value;
};

PacketResult ProcessPacket(string packetBin)
{
	size_t packetIdx = 0;
	
	// Parse the Packet header
	string packetVersBin = packetBin.substr(packetIdx, 3);
	int packetVers = BinaryToDecimal<int>(packetVersBin);
	packetIdx += packetVersBin.length();
	
	string packetTypeBin = packetBin.substr(packetIdx, 3);
	int packetTypeId = BinaryToDecimal<int>(packetTypeBin);
	packetIdx += packetTypeBin.length();
		
	PacketResult result;
	result.versionSum += packetVers;
	if (packetTypeId == 4)
	{
		// Literal Value
		// Read each group of 4, prefixed by a 0/1 bit, until the prefix is 0
		string literalValueBin = "";
		while (packetIdx <= packetBin.length() - 5)
		{
			char prefixBit = packetBin[packetIdx];
			literalValueBin.append(packetBin.substr(packetIdx + 1, 4));
			packetIdx += 5;
			if (prefixBit == '0')
			{
				break;
			}
		}
		result.value = BinaryToDecimal<uint64_t>(literalValueBin);
		result.bits += packetIdx;
	}
	else
	{
		// Operator
		char lengthTypeId = packetBin[packetIdx++];
		vector<long long> subPacketValues = {};
		if (lengthTypeId == '0')
		{
			// Next 15 bits represent total length of sub-packets
			string subPacketLenBin = packetBin.substr(packetIdx, 15);
			size_t subPacketLen = BinaryToDecimal<size_t>(subPacketLenBin);
			packetIdx += subPacketLenBin.length();
			result.bits += packetIdx;
			while (subPacketLen > 0)
			{
				PacketResult subPacketResult = ProcessPacket(packetBin.substr(packetIdx, subPacketLen));
				packetIdx += subPacketResult.bits;
				subPacketLen -= subPacketResult.bits;
				result.bits += subPacketResult.bits;
				result.versionSum += subPacketResult.versionSum;
				subPacketValues.push_back(subPacketResult.value);
			}
		}
		else
		{
			// Next 11 bits represent number of sub-packets
			string subPacketCountBin = packetBin.substr(packetIdx, 11);
			int subPacketCount = BinaryToDecimal<int>(subPacketCountBin);
			packetIdx += subPacketCountBin.length();
			result.bits += packetIdx;
			for (int i = 0; i < subPacketCount; ++i)
			{
				PacketResult subPacketResult = ProcessPacket(packetBin.substr(packetIdx, string::npos));
				packetIdx += subPacketResult.bits;
				result.bits += subPacketResult.bits;
				result.versionSum += subPacketResult.versionSum;
				subPacketValues.push_back(subPacketResult.value);
			}
		}

		switch (packetTypeId)
		{
			// Sum
			case 0:
				for (uint64_t value : subPacketValues)
				{
					result.value += value;
				}
				break;
			// Product
			case 1:
				result.value = 1;
				for (uint64_t value : subPacketValues)
				{
					result.value *= value;
				}
				break;
			// Min
			case 2:
				result.value = UINT64_MAX;
				for (uint64_t value : subPacketValues)
				{
					result.value = min(result.value, value);
				}
				break;
			// Max
			case 3:
				result.value = 0;
				for (uint64_t value : subPacketValues)
				{
					result.value = max(result.value, value);
				}
				break;
			// Greater Than
			case 5:
				result.value = subPacketValues[0] > subPacketValues[1] ? 1 : 0;
				break;
			// Less Than
			case 6:
				result.value = subPacketValues[0] < subPacketValues[1] ? 1 : 0;
				break;
			// Equal
			case 7:
				result.value = subPacketValues[0] == subPacketValues[1] ? 1 : 0;
				break;
		}
	}
	return result;
}

template <>
void Run<Day16>(Part part, istream& is, ostream& os)
{
	string packetHex;
	getline(is, packetHex);

	string packetBin = HexidecimalToBinary(packetHex);
	PacketResult result = ProcessPacket(packetBin);

	if(part == Part01)
	{
		os << "Packet Version Sum: " << result.versionSum << endl;
	}
	else if(part == Part02)
	{
		os << "Packet Value: " << result.value << endl;
	}
}