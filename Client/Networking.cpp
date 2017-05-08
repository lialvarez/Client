#include "Networking.h"

void Networking::sendWRQ(std::string fileToTransfer)
{
	this->fileToTransfer = fileToTransfer;
	packageSET(WRQ_OP);
	sendPackage();
}

void Networking::sendRRQ(std::string fileToTransfer)
{
	this->fileToTransfer = fileToTransfer;
	packageSET(RRQ_OP);
	sendPackage();
}

void Networking::sendData(FILE *filePointer, unsigned int blockNumber)
{
	packageSET(DATA_OP, blockNumber, filePointer);
	sendPackage();
}

void Networking::sendAck(unsigned int blockNumber /*= 0*/)
{
	packageSET(ACK_OP, blockNumber);
	sendPackage();
}

void Networking::sendError(std::string errorMsg, unsigned int errorCode)
{
	this->errorCode = errorCode;
	this->errorMsg = errorMsg;
	packageSET(ERROR_OP);
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
		package[1] = (_BYTE)1;
		strcpy((char *)package + 2, fileToTransfer.c_str());
		strcpy((char *)package + 2 + (fileToTransfer.length() + 1), mode.c_str());
		break;
	case WRQ_OP:
		package = new _BYTE[(mode.length() + 1) + (fileToTransfer.length() + 1) + 2];
		package[0] = 0;
		package[1] = (_BYTE)2;
		strcpy((char *)package + 2, fileToTransfer.c_str());
		strcpy((char *)package + 2 + (fileToTransfer.length() + 1), mode.c_str());
		break;
	case DATA_OP:
		char auxBuffer[512];
		unsigned int bytesToSend = fread(auxBuffer, 1, 512, filePointer);	//cuenta los bytes a enviar
		package = new _BYTE[bytesToSend + 4];
		package[0] = 0x00;
		package[1] = (_BYTE)3;
		package[2] = (blockNumber & 0xFF00)>>8;
		package[3] = (blockNumber & 0x00FF);
		strcpy((char *)package + 4, auxBuffer);
		break;
	case ACK_OP:
		package = new _BYTE[4];
		package[0] = 0x00;
		package[1] = (_BYTE)4;
		package[2] = (blockNumber & 0xFF00) >> 8;
		package[3] = (blockNumber & 0x00FF);
		break;
	case ERROR_OP:
		package = new _BYTE[errorMsg.length() + 5];
		package[0] = 0x00;
		package[1] = (_BYTE)5;
		package[2] = 0x00;
		package[3] = (_BYTE)errorCode;
		strcpy((char *)package + 4, errorMsg.c_str());
		break;
	default:
		break;
	}
}