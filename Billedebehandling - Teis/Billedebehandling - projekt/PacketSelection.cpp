#include "PacketSelection.h"



PacketSelection::PacketSelection(int n)
{
	datapakkeAntal = n;
}

std::vector<int> PacketSelection::selectPackets(std::vector<std::string> stringS)
{
	int realSeqNum;
	int seqNum;
	int a = 0;
	std::vector<int> returnVector;
	int graense = 99999;
	if (stringS.size() == 1) {
		graense = -1;
	}
	for (int v = 0; v < stringS.size() - 1; v++) {
		if (stoi(stringS[v], nullptr, 2) > stoi(stringS[v + 1], nullptr, 2))
			graense = v;
	}

	for (int i = 0; i < stringS.size(); i++) {
		seqNum = stoi(stringS[i], nullptr, 2);

		if (i <= graense) {
			a = -1;
		}
		else {
			a = 0;
		}
		realSeqNum = 15 * ((lastPackIndex) / 15 + a) + seqNum;
		returnVector.push_back(realSeqNum);
	}

	return returnVector;
}

int PacketSelection::getPacketToSendIndex()
{
	int returner = lastPackIndex + 1;
	lastPackIndex += packetsPerSend;
	return returner;
}

int PacketSelection::getResendIndex()
{
	return lastPackIndex-2;
}

int PacketSelection::getResendIndexLast(int frames)
{
	return lastPackIndex - frames + 1;
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
