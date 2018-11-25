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
	
	//streng der skal udf�res CRC-check p�
	std::string remainder = bitStreng;
	
	//der appendes "0"'er til 
	for (int i = 0; manglendeBits > i; i++)
	{
		remainder.append("0");
	}

	//nu skal beskeden XOR'es med dividenten det antal gange der er bitstrengen er lang

	
	
	for(int j = 0; j < bitStreng.length(); j++)
	{

		for (int m = divident.length() -1; m >= 0; m--)
		{
			
			
			//vi finder ud af om f�rste bit er 1 eller 0
			if(remainder[j] == '1')
			{
				//vi finder det m'te ellement for dividenten 
				char divM = divident[m];
				

				//finder det m'te element af J i remainderen
				char remainM = remainder[j + m];

				//Der XOR'es med bit by bit

				if (remainM != divM)
				{
					//her gemmes remainderen inden det j+m'te element
					std::string fDel = remainder.substr(0, j + m);
					//her gemmes remainderen efter det j+m'te element
					std::string sDel = remainder.substr(j + m + 1, remainder.length());
					// her inds�ttes et ettal p� det j+m'te element
					remainder = fDel + "1" + sDel;
				}
				else
				{
					//her gemmes remainderen inden det j+m'te element
					std::string fDel = remainder.substr(0, j + m);
					//her gemmes remainderen efter det j+m'te element
					std::string sDel = remainder.substr(j + m + 1, remainder.length());
					// her inds�ttes et ettal p� det j+m'te element
					remainder = fDel + "0" + sDel;
				}

			}
			//hvis MSB er = 0, skal alle bits bare rykkes da der XOR'es med m antal 0'er, og dette g�res ved at s�tte j+1 
		}
	}

	std::string checksum = remainder.substr(remainder.length() - divident.length() + 1, remainder.length());
	return checksum;
}

std::string CRC::crcCheckReciever()
{
	//streng der skal udf�res CRC-check p�
	std::string remainder = bitStreng;

	//nu skal beskeden XOR'es med dividenten det antal gange der er bitstrengen er lang

	for (int j = 0; j < bitStreng.length() - (divident.length() - 1); j++)
	{
		for (int m = divident.length() - 1; m > 0; m--)
		{
			
			//vi finder ud af om f�rste bit er 1 eller 0
			if (remainder[j] == '1')
			{
				//vi finder det m'te ellement for dividenten 
				char divM = divident[m];


				//finder det m'te element af J i remainderen
				char remainM = remainder[j + m];



				//Der XOR'es med bit by bit

				if (remainM != divM)
				{
					//her gemmes remainderen inden det j+m'te element
					std::string fDel = remainder.substr(0, j + m);
					//her gemmes remainderen efter det j+m'te element
					std::string sDel = remainder.substr(j + m + 1, remainder.length() - (j + m + 1));
					// her inds�ttes et ettal p� det j+m'te element
					remainder = fDel + "1" + sDel;

				}
				else
				{
					//her gemmes remainderen inden det j+m'te element
					std::string fDel = remainder.substr(0, j + m);
					//her gemmes remainderen efter det j+m'te element
					std::string sDel = remainder.substr(j + m + 1, remainder.length());
					// her inds�ttes et ettal p� det j+m'te element
					remainder = fDel + "0" + sDel;

				}

			}
			//hvis MSB er = 0, skal alle bits bare rykkes da der XOR'es med m antal 0'er, og dette g�res ved at s�tte j+1 
		}
	}


	checkCiffer = remainder.substr(remainder.length() - divident.length() + 1, remainder.length());
	return checkCiffer;
}

CRC::~CRC()
{
}
