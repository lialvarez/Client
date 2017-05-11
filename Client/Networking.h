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
#include "Packages.h"

#define CONNECTION_PORT "69"	//puerto TFTP
#define PACKAGE_MAX_SIZE 516	

typedef char MYBYTE;

typedef enum { RRQ_OP = 1, WRQ_OP, DATA_OP, ACK_OP, ERROR_OP }opCodes;
typedef enum { NOT_DEFINED = 1, FILE_NOT_FOUND, FILE_ALREADY_EXISTS }errorCodes;

class Networking
{
public:
	Networking(std::string _serverAddress);
	~Networking(); //Ver cuando llamarlo
	std::string getServerAddres();
	void sendPackage(genericPackage *Pkg);
	bool receivePackage();
	MYBYTE *getInputPackage();
	errorCodes getErrorCode();
	std::string getData();
	std::string getErrorMsg();
	unsigned int getBlockNumber();


	void startConnection();

private:
	boost::asio::io_service* IO_handler;
	boost::asio::ip::tcp::socket* clientSocket;
	boost::asio::ip::tcp::resolver* clientResolver;
	boost::asio::ip::tcp::resolver::iterator endpoint;

	void packageDecode();

	std::string serverAddress;
	std::string fileToTransfer;

	opCodes receivedPackageType;
	std::string data;	//Se almacena la data en caso de recibir DATA
	std::string errorMsg;
	errorCodes errorCode;
	unsigned int blockNumber;

	MYBYTE *inputPackage;
	MYBYTE *outputPackage;
};
#endif // !NETWORKING_H
