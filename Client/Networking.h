#ifndef NETWORKING_H
#define NETWORKING_H

#include <iostream>
#include <string>
#include <exception>
#include <array>
#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <Windows.h> //ver si sacar
#include <iostream>

#define CONNECTION_PORT 69		//puerto TFTP


typedef char _BYTE;

typedef enum { RRQ_OP, WRQ_OP, DATA_OP, ACK_OP, ERROR_OP }opCodes;

class Networking
{
public:
	Networking(std::string _serverAddress);
	~Networking(); //Ver cuando llamarlo
	void sendWRQ(std::string fileToTransfer);
	void sendRRQ(std::string fileToTransfer);
	void sendData(FILE *filePointer, unsigned int blockNumber);
	void sendAck(unsigned int blockNumber = 0);
	void sendError(std::string errorMsg, unsigned int errorCode);
	void receivePackage();
	void callback1(const boost::system::error_code& error, std::size_t transfered_bytes);
	void callback2(const boost::system::error_code& error, std::size_t transfered_bytes);
	void startConnection(const char* IP);

private:
	boost::asio::io_service* IO_handler;
	boost::asio::ip::tcp::socket* clientSocket;
	boost::asio::ip::tcp::resolver* clientResolver;
	boost::asio::ip::tcp::resolver::iterator endpoint;

	void packageSET(opCodes opCode, unsigned int blockNumber = 0, FILE *filePointer = NULL);
	void sendPackage();

	const char* serverAddress;
	std::string fileToTransfer;
	std::string errorMsg;

	unsigned int errorCode;
	_BYTE *package;
	bool packageArrived;
};
#endif // !NETWORKING_H
