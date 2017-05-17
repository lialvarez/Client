#include "Networking.h"
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <conio.h>


Networking::Networking(std::string _serverAddress): serverAddress(_serverAddress)
{
	IO_handler = new boost::asio::io_service();
	clientSocket = new boost::asio::ip::tcp::socket(*IO_handler);
	clientResolver = new boost::asio::ip::tcp::resolver(*IO_handler);
}

Networking::~Networking()
{
	clientSocket->close();

	delete clientResolver;
	delete clientSocket;
	delete IO_handler;
}

void Networking::startConnection()
{
	endpoint = clientResolver->resolve(
		boost::asio::ip::tcp::resolver::query(serverAddress.c_str(), CONNECTION_PORT));
	boost::asio::connect(*clientSocket, endpoint);
	clientSocket->non_blocking(true);
}

std::string Networking::getServerAddres()
{
	return serverAddress;
}

unsigned int Networking::getBlockNumber()
{
	return blockNumber;
}

void Networking::packageDecode()
{
	receivedPackageType = (opCodes)inputPackage[1];
	switch (receivedPackageType)
	{
	case DATA_OP:
		blockNumber = (inputPackage[2] << 8) + inputPackage[3];
		data = inputPackage[4];
		break;
	case ACK_OP:
		blockNumber = (inputPackage[2] << 8) + inputPackage[3];
		break;
	case ERROR_OP:
		errorCode = (errorCodes)inputPackage[3];
		errorMsg = inputPackage[4];
		break;
	default:
		break;
	}
}

errorCodes Networking::getErrorCode()
{
	return errorCode;
}

std::string Networking::getErrorMsg()
{
	return errorMsg;
}

std::string Networking::getData()
{
	return data;
}

void Networking::sendPackage(genericPackage *Pkg)
{
	Pkg->setPackage();

	size_t len;
	boost::system::error_code error;

	do
	{													
		len = clientSocket->write_some(boost::asio::buffer(Pkg->package, Pkg->package.size()), error); 
	} while ((error.value() == WSAEWOULDBLOCK));
	if (error)
	{ 
		/*std::cout << "Error while trying to connect to server " << error.message() << std::endl;*/
	}
		
}

bool Networking::receivePackage()
{
	bool ret = false;
	boost::system::error_code error;
	char buf[PACKAGE_MAX_SIZE];
	size_t len = 0;
	len = clientSocket->read_some(boost::asio::buffer(buf), error);
	//do
	//{
	//	len = clientSocket->read_some(boost::asio::buffer(buf), error);
	//} while (error.value() != WSAEWOULDBLOCK);

	if (!error)
	{
		//strcpy_s(&inputPackage[0], len, buf);
		inputPackage.clear();
		inputPackage.insert(inputPackage.end(), buf, buf + len);
		ret = true;
	}
	else
	{
		ret = false;
	}
	return ret;
}

std::vector<char> Networking::getInputPackage()
{
	return inputPackage;
}
