#include "Networking.h"
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <conio.h>


Networking::Networking(std::string _serverAddress): serverAddress(_serverAddress)
{
	IO_handler = new boost::asio::io_service();
	clientSocket = new boost::asio::ip::tcp::socket(*IO_handler);
	clientResolver = new boost::asio::ip::tcp::resolver(*IO_handler);

	packageArrived = false;
}

Networking::~Networking()
{
	delete IO_handler;
	delete clientSocket;
	delete clientResolver;
}

//Esto esta comentado pq si pongo la definicion de la funcion en el .cpp tenemos un error.
//Pusimos la definicion en el .h

//void Networking::startConnection() 
//{
//
//	bool exit;
//
//	endpoint = clientResolver->resolve(boost::asio::ip::tcp::resolver::query(serverAddress.c_str(), CONNECTION_PORT));
//
//	do {
//		exit = true;
//		try {
//			boost::asio::connect(*clientSocket, endpoint);
//		}
//		catch (const std::exception& e)
//		{
//			std::cout << "Waiting for server." << std::endl;
//			exit = false;
//		}
//	} while (!exit);
//}

bool Networking::connectionLost()
{
	bool ret = false;

	//armar la funcion aca

	return ret;
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
	this->errorCode = (errorCodes)errorCode;
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

void Networking::sendPackage()
{
	std::cout << std::endl << "Sending package" << std::endl;

	boost::function<void(const boost::system::error_code&, std::size_t)> handler(
		boost::bind(&Networking::callback1, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));

	boost::asio::async_write(*clientSocket, boost::asio::buffer(outputPackage, PACKAGE_MAX_SIZE), handler); //ver si cambiar el 600
}

void Networking::callback1(const boost::system::error_code& error, std::size_t transfered_bytes) {}

bool Networking::receivePackage()
{
	//Recibe buf como referencia para almacenar el paquete recibido (en caso de haberlo) en el buffer indicado
	bool ret;	//Variable donde se indica si se recibio algo


	boost::system::error_code error;
	MYBYTE emptyBuf[PACKAGE_MAX_SIZE] = { NULL };
	MYBYTE buf[PACKAGE_MAX_SIZE] = { NULL };			

	std::cout << "Receiving package" << std::endl;

	boost::function<void(const boost::system::error_code&, std::size_t)> handler(
		boost::bind(&Networking::callback2, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));

	async_read(*clientSocket, boost::asio::buffer(buf, PACKAGE_MAX_SIZE), handler);		// Si recibe algo, lo guarda en buf.
	


	if (!strcmp(buf, emptyBuf))
	{
		packageArrived = false;
	}
	else
	{
		packageArrived = true;

		if (error != boost::asio::error::eof)											//Se guarda buf en inputPackage (el paquete recibido)
		{
			for (unsigned int i = 0; i < PACKAGE_MAX_SIZE; i++)
			{
				inputPackage[i] = buf[i];
			}
		}
		else
			std::cout << "Error while trying to connect to receive package %d" << error.message() << std::endl;
	}

	return ret;
}

MYBYTE * Networking::getInputPackage()
{
	return inputPackage;
}

void Networking::callback2(const boost::system::error_code& error, std::size_t transfered_bytes) {
}