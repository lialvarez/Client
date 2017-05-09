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
#define PACKAGE_MAX_SIZE 516	


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
	void afterSending(const boost::system::error_code& error, std::size_t transfered_bytes);   //callback que no se usa (NO BORRAR)
	void afterReceiving(const boost::system::error_code& error, std::size_t transfered_bytes); //callback que no se usa (NO BORRAR)
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
	_BYTE *inputPackage;
	_BYTE *outputPackage;
	bool packageArrived;
};
#endif // !NETWORKING_H
