#pragma once
#include <vector>
#include <string>
#include <iostream>

class PacketSelection
{
public:
	PacketSelection();

	std::vector<int> selectPackets(std::vector<std::string> vecS);

	//simple get-kommandoer
	int getPacketToSendIndex();
	
	//til test
	void print(std::vector<int> vec);

	~PacketSelection();

protected:
	int nextPacketToSendIndex = 0;
	int packetsPerSend = 3;
};

