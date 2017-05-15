#include "Packages.h"
#include "Networking.h"
#include <fstream>

void ReadRequest::setPackage()
{
	package.clear();
	package.push_back(0x00);
	package.push_back((MYBYTE)RRQ_OP);
	std::copy(fileName.begin(), fileName.end(), std::back_inserter(package));
	std::copy(mode.begin(), mode.end(), std::back_inserter(package));
}

void WriteRequest::setPackage()
{
	package.clear();
	package.push_back(0x00);
	package.push_back((MYBYTE)WRQ_OP);
	std::copy(fileName.begin(), fileName.end(), std::back_inserter(package));
	std::copy(mode.begin(), mode.end(), std::back_inserter(package));
}

void Acknowledge::setPackage()
{
	package.clear();
	package.push_back(0x00);
	package.push_back((MYBYTE)ACK_OP);
	package.push_back((blockNumber & 0xFF00) >> 8);
	package.push_back(blockNumber & 0x00FF);
}

Data::Data(std::vector<char> _data, unsigned int _blockNumber) : data(_data), blockNumber(_blockNumber), packageLength(_data.size() + 4) 
{
	code = DATA_OP;
}

void Data::setPackage()
{
	package.push_back(0x00);
	package.push_back((MYBYTE)DATA_OP);
	package.push_back((blockNumber & 0xFF00) >> 8);
	package.push_back(blockNumber & 0x00FF);
	package.insert(package.end(), data.begin(), data.end());
}

void Error::setPackage()
{
	package.clear();
	package.push_back(0x00);
	package.push_back((MYBYTE)ERROR_OP);
	package.push_back(0x00);
	package.push_back((MYBYTE)errorCode);
	std::copy(errorMsg.begin(), errorMsg.end(), std::back_inserter(package));
}