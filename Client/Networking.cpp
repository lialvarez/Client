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

//Esta funcion ya no va
void Networking::packageSET(opCodes opCode, unsigned int blockNumber /*= 0*/, FILE *filePointer /*= NULL*/)
{	
	std::string mode("octet");
	switch (opCode)
	{
	case RRQ_OP:
		outputPackage = new MYBYTE[(mode.length() + 1) + (fileToTransfer.length() + 1) + 2];
		outputPackage[0] = 0x00;
		outputPackage[1] = (MYBYTE)RRQ_OP;
		strcpy((char *)outputPackage + 2, fileToTransfer.c_str());
		strcpy((char *)outputPackage + 2 + (fileToTransfer.length() + 1), mode.c_str());
		break;
	case WRQ_OP:
		outputPackage = new MYBYTE[(mode.length() + 1) + (fileToTransfer.length() + 1) + 2];
		outputPackage[0] = 0x00;
		outputPackage[1] = (MYBYTE)WRQ_OP;
		strcpy((char *)outputPackage + 2, fileToTransfer.c_str());
		strcpy((char *)outputPackage + 2 + (fileToTransfer.length() + 1), mode.c_str());
		break;
	case DATA_OP:
		char auxBuffer[512];
		unsigned int bytesToSend = fread(auxBuffer, 1, 512, filePointer);	//cuenta los bytes a enviar
		outputPackage = new MYBYTE[bytesToSend + 4];
		outputPackage[0] = 0x00;
		outputPackage[1] = (MYBYTE)DATA_OP;
		outputPackage[2] = (blockNumber & 0xFF00)>>8;
		outputPackage[3] = (blockNumber & 0x00FF);
		strcpy((char *)outputPackage + 4, auxBuffer);
		break;
	case ACK_OP:
		outputPackage = new MYBYTE[4];
		outputPackage[0] = 0x00;
		outputPackage[1] = (MYBYTE)ACK_OP;
		outputPackage[2] = (blockNumber & 0xFF00) >> 8;
		outputPackage[3] = (blockNumber & 0x00FF);
		break;
	case ERROR_OP:
		outputPackage = new MYBYTE[errorMsg.length() + 5];
		outputPackage[0] = 0x00;
		outputPackage[1] = (MYBYTE)ERROR_OP;
		outputPackage[2] = 0x00;
		outputPackage[3] = (MYBYTE)errorCode;
		strcpy((char *)outputPackage + 4, errorMsg.c_str());
		break;
	default:
		break;
	}
}

void Networking::sendPackage(genericPackage *Pkg)
{
	Pkg->setPackage();

	boost::function<void(const boost::system::error_code&, std::size_t)> handler(
		boost::bind(&Networking::callback1, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));

	boost::asio::async_write(*clientSocket, boost::asio::buffer(Pkg->package, PACKAGE_MAX_SIZE), handler); //ver si cambiar el 600
}

bool Networking::receivePackage()
{
	bool ret = false;
	boost::system::error_code error;
	char buf[516];
	size_t len = 0;
	do
	{
		len = clientSocket->read_some(boost::asio::buffer(buf), error);
	} while (error.value() == WSAEWOULDBLOCK);

	if (!error)
	{
		strcpy_s(inputPackage, len, buf);
		ret = true;
	}
	else
	{
		ret = false;
	}
	return ret;
}

MYBYTE * Networking::getInputPackage()
{
	return inputPackage;
}

void Networking::callback2(const boost::system::error_code& error, std::size_t transfered_bytes) {
}