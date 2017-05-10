#include "Packages.h"
#include <fstream>

void ReadRequest::setPackage()
{
	package = new MYBYTE[(mode.length() + 1) + (fileName.length() + 1) + 2];
	package[0] = 0x00;
	package[1] = (MYBYTE)RRQ_OP;
	strcpy((char *)package[2], fileName.c_str());
	strcpy((char *)package[(fileName.length() + 1) + 2], mode.c_str());
}

void WriteRequest::setPackage()
{
	package = new MYBYTE[(mode.length() + 1) + (fileName.length() + 1) + 2];
	package[0] = 0x00;
	package[1] = (MYBYTE)WRQ_OP;
	strcpy((char *)package[2], fileName.c_str());
	strcpy((char *)package[(fileName.length() + 1) + 2], mode.c_str());
}

void Acknowledge::setPackage()
{
	package = new MYBYTE[4];
	package[0] = 0x00;
	package[1] = (MYBYTE)code;
	package[2] = (blockNumber & 0xFF00) >> 8;
	package[3] = (blockNumber & 0x00FF);
}

void Data::setPackage()
{
	package = new MYBYTE[data.length() + 4];
	package[0] = 0x00;
	package[1] = (MYBYTE)code;
	package[2] = (blockNumber & 0xFF00) >> 8;
	package[3] = (blockNumber & 0x00FF);
	strcpy_s((char *)package[4], data.length(),data.c_str); //No le debe poner el /0
}

void Error::setPackage()
{
	package = new MYBYTE[errorMsg.length() + 1];
	package[0] = 0x00;
	package[1] = (MYBYTE)ERROR_OP;
	package[2] = 0x00;
	package[3] = (MYBYTE)errorCode;
	strcpy((char *)package[4], errorMsg.c_str());
}