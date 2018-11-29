#pragma once
#include <vector>
#include <string>
#include <iostream>

class PacketSelection
{
public:
	PacketSelection(int n);

	std::vector<int> selectPackets(std::vector<std::string> stringS);
	
	//simple get-kommandoer
	int getPacketToSendIndex();
	int getResendIndex();
	int getResendIndexLast(int frames);
	
	//til test
	void print(std::vector<int> vec);

	~PacketSelection();

protected:
	int lastPackIndex = -1;
	int packetsPerSend = 3;
	int datapakkeAntal;
};

