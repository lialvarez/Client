#ifndef NETWORKING_H
#define NETWORKING_H

#include <string>

typedef char _BYTE;

typedef enum { RRQ_OP, WRQ_OP, DATA_OP, ACK_OP, ERROR_OP }opCodes;

class Networking
{
public:
	Networking(std::string _serverAddres):serverAddress(_serverAddres){}
	void sendWRQ(std::string fileToTransfer);
	void sendRRQ(std::string fileToTransfer);
	void sendData(FILE *filePointer, unsigned int blockNumber);
	void sendAck(unsigned int blockNumber = 0);
	void sendError(std::string errorMsg, unsigned int errorCode);

private:
	void packageSET(opCodes opCode, unsigned int blockNumber = 0, FILE *filePointer = NULL);
	void sendPackage();
	std::string serverAddress;
	std::string fileToTransfer;
	std::string errorMsg;
	unsigned int errorCode;
	_BYTE *package;
};
#endif // !NETWORKING_H
