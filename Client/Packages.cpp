#include "Packages.h"
#include "Networking.h"
#include <fstream>

void ReadRequest::setPackage()
{
	package.clear();
	package[0] = 0x00;
	package[1] = (MYBYTE)RRQ_OP;
	strcpy(&package[2], fileName.c_str());
	strcpy(&package[(fileName.length() + 1) + 2], mode.c_str());
}

void WriteRequest::setPackage()
{
	package[0] = 0x00;
	package[1] = (MYBYTE)WRQ_OP;
	strcpy(&package[2], fileName.c_str());
	strcpy(&package[(fileName.length() + 1) + 2], mode.c_str());
}

void Acknowledge::setPackage()
{
	package[0] = 0x00;
	package[1] = (MYBYTE)code;
	package[2] = (blockNumber & 0xFF00) >> 8;
	package[3] = (blockNumber & 0x00FF);
}

Data::Data(std::vector<char> _data, unsigned int _blockNumber) : data(_data), blockNumber(_blockNumber), packageLength(_data.size() + 4) 
{
	code = DATA_OP;
}

void Data::setPackage()
{
	package[0] = 0x00;
	package[1] = (MYBYTE)code;
	package[2] = (blockNumber & 0xFF00) >> 8;
	package[3] = (blockNumber & 0x00FF);
	strcpy_s(&package[4], data.size(), &data[0]);
}

void Error::setPackage()
{
	package[0] = 0x00;
	package[1] = (MYBYTE)ERROR_OP;
	package[2] = 0x00;
	package[3] = (MYBYTE)errorCode;
	strcpy(&package[4], errorMsg.c_str());
}