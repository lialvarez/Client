
#include "Networking.h"


Networking::Networking(std::string _serverAddress)
{
	IO_handler = new boost::asio::io_service();
	clientSocket = new boost::asio::ip::tcp::socket(*IO_handler);
	clientResolver = new boost::asio::ip::tcp::resolver(*IO_handler);

	packageArrived = false;
	serverAddress = _serverAddress.c_str();		//_serverAddress viene del main, del teclado. Es el ip que se ingresa.
	startConnection(serverAddress);
}

Networking::~Networking()
{
	delete IO_handler;
	delete clientSocket;
	delete clientResolver;
}

void Networking::startConnection(const char* _serverAddress) {

	bool exit;

	endpoint = clientResolver->resolve(boost::asio::ip::tcp::resolver::query(_serverAddress, CONNECTION_PORT));

	do {													//ponerle timer o algo para que no quede para siempre!!!!!!!!!!!!!!!
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
		outputPackage = new _BYTE[(mode.length() + 1) + (fileToTransfer.length() + 1) + 2];
		outputPackage[0] = 0x00;
		outputPackage[1] = (_BYTE)1;
		strcpy((char *)outputPackage + 2, fileToTransfer.c_str());
		strcpy((char *)outputPackage + 2 + (fileToTransfer.length() + 1), mode.c_str());
		break;
	case WRQ_OP:
		outputPackage = new _BYTE[(mode.length() + 1) + (fileToTransfer.length() + 1) + 2];
		outputPackage[0] = 0;
		outputPackage[1] = (_BYTE)2;
		strcpy((char *)outputPackage + 2, fileToTransfer.c_str());
		strcpy((char *)outputPackage + 2 + (fileToTransfer.length() + 1), mode.c_str());
		break;
	case DATA_OP:
		char auxBuffer[512];
		unsigned int bytesToSend = fread(auxBuffer, 1, 512, filePointer);	//cuenta los bytes a enviar
		outputPackage = new _BYTE[bytesToSend + 4];
		outputPackage[0] = 0x00;
		outputPackage[1] = (_BYTE)3;
		outputPackage[2] = (blockNumber & 0xFF00)>>8;
		outputPackage[3] = (blockNumber & 0x00FF);
		strcpy((char *)outputPackage + 4, auxBuffer);
		break;
	case ACK_OP:
		outputPackage = new _BYTE[4];
		outputPackage[0] = 0x00;
		outputPackage[1] = (_BYTE)4;
		outputPackage[2] = (blockNumber & 0xFF00) >> 8;
		outputPackage[3] = (blockNumber & 0x00FF);
		break;
	case ERROR_OP:
		outputPackage = new _BYTE[errorMsg.length() + 5];
		outputPackage[0] = 0x00;
		outputPackage[1] = (_BYTE)5;
		outputPackage[2] = 0x00;
		outputPackage[3] = (_BYTE)errorCode;
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
		boost::bind(&Networking::afterSending, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));

	boost::asio::async_write(*clientSocket, boost::asio::buffer(outputPackage, PACKAGE_MAX_SIZE), handler); //ver si cambiar el 600
}

void Networking::afterSending(const boost::system::error_code& error, std::size_t transfered_bytes) {
}

void Networking::receivePackage()
{
	boost::system::error_code error;
	_BYTE emptyBuf[PACKAGE_MAX_SIZE] = { NULL };
	_BYTE buf[PACKAGE_MAX_SIZE] = { NULL };			

	std::cout << "Receiving package" << std::endl;

	boost::function<void(const boost::system::error_code&, std::size_t)> handler(
		boost::bind(&Networking::afterReceiving, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));

	async_read(*clientSocket, boost::asio::buffer(buf, PACKAGE_MAX_SIZE), handler);		// Si recibe algo, lo guarda en buf


	if (strcmp(buf, emptyBuf))
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
}

void Networking::afterReceiving(const boost::system::error_code& error, std::size_t transfered_bytes) {
}