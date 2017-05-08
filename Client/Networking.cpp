#include "Networking.h"

void Networking::sendWRQ(std::string fileToTransfer)
{
	this->fileToTransfer = fileToTransfer;
	packageSET(WRQ_OP);
	sendPackage();
}

void Networking::sendDATA(FILE *filePointer, unsigned int blockNumber)
{
	packageSET(DATA_OP, blockNumber, filePointer);
	sendPackage();
}

void Networking::packageSET(opCodes opCode, unsigned int blockNumber /*= 0*/, FILE *filePointer /*= NULL*/)
{	
	std::string mode("octet");
	switch (opCode)
	{
	case RRQ_OP:
		package = new _BYTE[(mode.length() + 1) + (fileToTransfer.length() + 1) + 2];
		package[0] = 0x00;
		package[1] = (_BYTE)opCode;
		strcpy((char *)package + 2, fileToTransfer.c_str());
		strcpy((char *)package + 2 + (fileToTransfer.length() + 1), mode.c_str());
		break;
	case WRQ_OP:
		package = new _BYTE[(mode.length() + 1) + (fileToTransfer.length() + 1) + 2];
		package[0] = 0;
		package[1] = (_BYTE)opCode;
		strcpy((char *)package + 2, fileToTransfer.c_str());
		strcpy((char *)package + 2 + (fileToTransfer.length() + 1), mode.c_str());
		break;
	case DATA_OP:
		char auxBuffer[512];
		unsigned int bytesToSend = fread(auxBuffer, 1, 512, filePointer);	//cuenta los bytes a enviar
		package = new _BYTE[bytesToSend + 2];
		package[0] = 0x00;
		package[1] = opCode;

		strcpy((char *)package + 2, auxBuffer);
	default:
		break;
	}
}