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

typedef enum { RRQ_OP = 1, WRQ_OP, DATA_OP, ACK_OP, ERROR_OP }opCodes;
typedef enum { NOT_DEFINED = 1, FILE_NOT_FOUND, FILE_ALREADY_EXISTS }errorCodes;

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
	errorCodes getErrorCode();
	std::string getData();
	std::string getErrorMsg();

	void callback1(const boost::system::error_code& error, std::size_t transfered_bytes);
	void callback2(const boost::system::error_code& error, std::size_t transfered_bytes);

	void startConnection();
	void startConnection()
	{
		bool exit;
		endpoint = clientResolver->resolve(boost::asio::ip::tcp::resolver::query(serverAddress.c_str(), CONNECTION_PORT));

		do 
		{
			exit = true;
			try {
				boost::asio::connect(*clientSocket, endpoint);
			}
			catch (const std::exception& e)
			{
				std::cout << "Waiting for server." << std::endl;
				exit = false;
			}
		} while (!exit);
	}

private:
	boost::asio::io_service* IO_handler;
	boost::asio::ip::tcp::socket* clientSocket;
	boost::asio::ip::tcp::resolver* clientResolver;
	boost::asio::ip::tcp::resolver::iterator endpoint;

	void packageSET(opCodes opCode, unsigned int blockNumber = 0, FILE *filePointer = NULL);
	void sendPackage();
	void packageDecode();

	std::string serverAddress;
	std::string fileToTransfer;

	opCodes receivedPackageType;
	std::string data;	//Se almacena la data en caso de recibir DATA
	std::string errorMsg;
	errorCodes errorCode;
	unsigned int blockNumber;

	_BYTE *inputPackage;
	_BYTE *outputPackage;
	
	bool packageArrived;
};
#endif // !NETWORKING_H
