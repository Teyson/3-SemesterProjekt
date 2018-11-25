#include "PacketSelection.h"



PacketSelection::PacketSelection()
{
}

std::vector<int> PacketSelection::selectPackets(std::vector<std::string> vecS)
{
	std::vector<int> returnVector;

	for (int i = 0; i < vecS.size(); i++)
	{
		int seqNum = std::stoi(vecS[i], nullptr, 2);
		returnVector.push_back(seqNum);
	}

	if (returnVector.size() == 0)
	{
		for (int i = 0; i < packetsPerSend; i++)
		{
			returnVector.push_back(nextPacketToSendIndex);
			nextPacketToSendIndex++;
		}
	}

	return returnVector;
}

int PacketSelection::getPacketToSendIndex()
{
	return nextPacketToSendIndex;
}


void PacketSelection::print(std::vector<int> vec)
{
	for (int i = 0; i < vec.size(); i++)
	{
		std::cout << vec[i] << std::endl;
	}
}

PacketSelection::~PacketSelection()
{
}
