#pragma once
#include <vector>
#include <string>
#include <iostream>

class PacketSelection
{
public:
	PacketSelection();

	std::vector<int> selectPackets(std::vector<std::string> stringS);

	//simple get-kommandoer
	int getPacketToSendIndex();
	
	//til test
	void print(std::vector<int> vec);

	~PacketSelection();

protected:
	int lastPackIndex = -1;
	int packetsPerSend = 3;
};

