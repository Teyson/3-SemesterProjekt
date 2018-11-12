#include "CRC.h"


CRC::CRC(std::string st)
{
	bitStreng = st;
}

std::string CRC::getbitstreng()
{
	return bitStreng;
}


std::string CRC::crcCheck()
{
	//hvor mange "0"'er  det mangler
	int manglendeBits = divident.length()-1;
	
	//streng der skal udføres CRC-check på
	std::string remainder = bitStreng;
	
	//der appendes "0"'er til 
	for (int i = 0; manglendeBits > i; i++)
	{
		remainder.append("0");
	}

	std::cout << "couter remainder: " << remainder << std::endl;
	//nu skal beskeden XOR'es med dividenten det antal gange der er bitstrengen er lang

	
	
	for(int j = 0; j < bitStreng.length(); j++)
	{

		for (int m = divident.length() -1; m >= 0; m--)
		{
			
			//vi skal have lige så mange bit i remainder som i divident:
			//std::string remDel = remainder.substr(j, divident.length());

			//vi finder ud af om første bit er 1 eller 0
			if(remainder[j] == '1')
			{
				//vi finder det m'te ellement for dividenten 
				char divM = divident[m];

				std::cout << "divM: " << divM << std::endl;
				

				//finder det m'te element af J i remainderen
				char remainM = remainder[j + m];
				
				std::cout << "remainM: " << remainM << std::endl;

				//Der XOR'es med bit by bit

				if (remainM != divM)
				{
					//her gemmes remainderen inden det j+m'te element
					std::string fDel = remainder.substr(0, j + m);
					//her gemmes remainderen efter det j+m'te element
					std::string sDel = remainder.substr(j + m + 1, remainder.length());
					// her indsættes et ettal på det j+m'te element
					remainder = fDel + "1" + sDel;
					std::cout << "vi finder ud af hvornaar der XOR'es" << std::endl;
					std::cout << "1 er der forskel paa remainderen?: " << remainder << std::endl;
					std::cout << "printer m: " << m << std::endl;
					std::cout << "printer j+m: " << j+m << std::endl;

					std::cout << "couter dividenten: " << divident << std::endl;

				}
				else
				{
					//her gemmes remainderen inden det j+m'te element
					std::string fDel = remainder.substr(0, j + m);
					//her gemmes remainderen efter det j+m'te element
					std::string sDel = remainder.substr(j + m + 1, remainder.length());
					// her indsættes et ettal på det j+m'te element
					remainder = fDel + "0" + sDel;
					std::cout << "2 er der forskel paa remainderen?: " << remainder << std::endl;
				}

			}
			//hvis MSB er = 0, skal alle bits bare rykkes da der XOR'es med m antal 0'er, og dette gøres ved at sætte j+1 
		}
	}

	std::cout << "Dette er den fulde remainder: " << remainder << std::endl;

	std::string checksum = remainder.substr(remainder.length() - divident.length() + 1, remainder.length());
	return checksum;
}





CRC::~CRC()
{
}
