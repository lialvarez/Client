#include "eventSources.h"
#include "Screen.h"
#include "Events.h"
#include <boost\algorithm\string\classification.hpp>
#include <boost\algorithm\string\split.hpp>
#include <boost\date_time\posix_time\posix_time.hpp>

/*****  NETWORK EVENT SOURCE  *****/
NetworkEventSource::NetworkEventSource(Networking *_networkInterface) :networkInterface(_networkInterface) 
{
	serverIP = networkInterface->getServerAddres();
}

bool NetworkEventSource::isThereEvent()
{ 
	bool ret = false;
	std::string errorMsg;
	unsigned int errorCode;
	std::string data;
	unsigned int blockNumber;
	
	if (networkInterface->receivePackage())	//verifica si se recibio algo
	{
		switch (networkInterface->getInputPackage()[1])	//segun el tipo de paquete devuelvo el tipo de evento
		{
		case DATA_OP:
			data = (char *)networkInterface->getInputPackage()[4];
			blockNumber = (networkInterface->getInputPackage[2] << 8) + networkInterface->getInputPackage()[3];
			if (blockNumber != expectedBlockNum)
			{
				pkg = new Error(NOT_DEFINED, "Block number conflict");
				ret = true;
				evCode = ERRO;
			}
			else
			{
				pkg = new Data(data, blockNumber);
				ret = true;
				if (data.length < 512)
				{
					evCode = LAST_DATA;
				}
				else
				{
					evCode = DATA;
				}
			}
			break;
		case ACK_OP:
			blockNumber = (networkInterface->getInputPackage[2] << 8) + networkInterface->getInputPackage()[3];
			if (blockNumber != expectedBlockNum)
			{
				pkg = new Error(NOT_DEFINED, "Block number conflict");
				ret = true;
				evCode = ERRO;
			}
			else
			{
				pkg = new Acknowledge(blockNumber);
				ret = true;
				evCode = ACK;
			}
			break;
		case ERROR_OP:
			errorCode = networkInterface->getInputPackage()[2];
			errorMsg = (char *)networkInterface->getInputPackage()[4];
			pkg = new Error(errorCode, errorMsg);
			ret = true;
			evCode = ERRO;
			break;
		default:
			break;
		}
	}
	return ret;
}

void NetworkEventSource::setServerIP(std::string _serverIP)
{
	serverIP = _serverIP;
}

std::string NetworkEventSource::getServerIP()
{
	return serverIP;
}

genericEvent * NetworkEventSource::insertEvent()
{
	genericEvent * ret;

	switch (evCode)
	{
	case DATA:
		pkg = new Data(networkInterface->getData(), networkInterface->getBlockNumber());
		ret = (genericEvent *) new EV_Data((Data *)pkg);
		break;
	case CONNECTION_FAIL:
		ret = (genericEvent *) new EV_ConnectionFailed();
		break;
	case ACK:
		pkg = new Acknowledge(networkInterface->getBlockNumber());
		ret = (genericEvent *) new EV_Ack((Acknowledge *)pkg);
		break;
	case ERRO:
		pkg = new Error(networkInterface->getErrorCode(), networkInterface->getErrorMsg());
		ret = (genericEvent *) new EV_Error((Error *)pkg);
		break;
	default:
		break;
	}
}

/*****  USER EVENT SOURCE  *****/

//NO TOCAR LOS DE USER, CREO QUE YA ESTAN LISTOS

UserEventSource::UserEventSource(Screen *terminal)
{
	this->terminal = terminal;
	this->terminal->initTerminal();	//Inicializa la terminal por donde interactua el usuario
}

std::string UserEventSource::getCommand()
{
	return command;
}

std::string UserEventSource::getFileToTransfer()
{
	return fileToTransfer;	//Devuelve el nombre del archivo a transferir
}

bool UserEventSource::isThereEvent()
{
	std::fstream auxFile;
	bool ret = false;
	inputChar = getch();

	switch (inputChar)
	{
	case ERR:

		ret = false;	//Si no se ingresa tecla, indica que no hay evento
		break;			//y no carga caracter en buffer.

	case BACKSPACE:

		//Si se apreta "backspace" borra y quita caracteres del buffer
		//hasta que llega el comienzo de la linea de comando
		//Tambien elimina los caracteres ingresados de la teminal
		if (buffer.size() > 0)
		{
			buffer.pop_back();
			delch();
		}
		if (terminal->terminalWindow->_curx == 6)	//Posicion donde arranca la linea de comando
		{
			terminal->terminalWindow->_curx++;	//Para que el cursor no pse vaya del area permitida
		}
		ret = false;	//No hay evento
		break;

	case ENTER:

		command = std::string(buffer.begin(), buffer.end());	//Almaceno la linea de comando ingresada en command
		std::transform(command.begin(), command.end(), command.begin(), tolower);	//transformo la linea ingresada a minuscula
		boost::split(words, command, boost::is_any_of(", "), boost::token_compress_on);	//Se separan las palabras ingresadas

		if (words[0].size() == 0)	//Si no se ingresaron comandos
		{
			evCode = EMPTY_COMMAND;
			ret = true;
		}
		else if (words.size() == 1)	//Si se ingreso un comando de una sola palabra
		{
			if (strcmp(words[0].c_str(), "help") == 0)
			{
				evCode = HELP;
				ret = true;
			}
			else if (strcmp(words[0].c_str(), "quit") == 0)
			{
				evCode = QUIT;
				ret = true;
			}
			else if (strcmp(words[0].c_str(), "clear") == 0)
			{
				evCode = CLEAR;
				ret = true;
			}
			else
			{
				evCode = INVALID;
				ret = true;
			}
		}
		else if (words.size() == 2)	//Si se ingreso un comando con dos "palabras"
		{
			fileToTransfer = words[1];
			auxFile.open(words[1].c_str());
			if (strcmp(words[0].c_str(), "put") == 0)
			{

				if (auxFile.fail())
				{
					ret = true;
					evCode = FILE_ERROR;
				}
				else
				{
					auxFile.close();
					evCode = PUT;
					ret = true;

				}

			}
			else if (strcmp(words[0].c_str(), "get") == 0)
			{
				if (auxFile.fail())
				{
					ret = true;
					evCode = FILE_ERROR;
				}
				else
				{
					auxFile.close();
					evCode = GET;
					ret = true;
				}

			}
			else
			{
				evCode = INVALID;
				ret = true;
			}
		}
		else	//Si no es ninguna de las anteriores estoy seguro de que el comando es invalido
		{
			ret = true;
			evCode = INVALID;
		}
		buffer.clear();	//Limpia el buffer
		words.clear();	//Limpia el vector
		break;

	default:
		//Si el caracter ingresado es un caracter valido(ningun caracter de control)
		//lo a�ade al buffer
		if (inputChar > ASCII_START && inputChar < ASCII_END)
		{
			buffer.push_back(inputChar);
			ret = false;
		}
		break;
	}
	return ret;
}

genericEvent * UserEventSource::insertEvent()
{
	genericEvent * ret;

	EV_Put *castedEvP;
	EV_Get *castedEvG;
	EV_InvalidCommand * castedEvIC;
	EV_FileError *castedEvFE;
	switch (evCode)
	{
	case PUT:
		ret = (genericEvent *) new EV_Put(fileToTransfer);
		break;
	case GET:
		ret = (genericEvent *) new EV_Get(fileToTransfer);
		break;
	case HELP:
		ret = (genericEvent *) new EV_Help();
		break;
	case CLEAR:
		ret = (genericEvent *) new EV_Clear();
		break;
	case EMPTY_COMMAND:
		ret = (genericEvent *) new EV_EmptyCommand();
		break;
	case INVALID:
		ret = (genericEvent *) new EV_InvalidCommand(command);
		break;
	case FILE_ERROR:
		ret = (genericEvent *) new EV_FileError(fileToTransfer);
		break;
	case QUIT:
		ret = (genericEvent *) new EV_Quit;
		break;
	default:
		break;
	}
	return ret;
}

/*****  TIMEOUTS EVENT SOURCE  *****/

TimeoutEventSource::TimeoutEventSource()
{
	timeout = false;
	timerRunning = false;
}

bool TimeoutEventSource::isThereEvent()
{
	if (((clock() - tInicial) > ONE_MINUTE * CLOCKS_PER_SEC) && timerRunning)
	{
		timeout = true;
		timerRunning = false;
		evCode = TIMEOUT;
	}
	else
	{
		timeout = false;
		evCode = NO_EV;
	}
	return timeout;
}

void TimeoutEventSource::startTimer()

{
	timeout = false;	//Se setea la variable de control en false, indicando que no ha ocurrido timeout
	tInicial = clock();
	timerRunning = true;
}

genericEvent * TimeoutEventSource::insertEvent()
{
	genericEvent * ret;

	switch (evCode)
	{
	case TIMEOUT:
		ret = (genericEvent *) new EV_Timeout;
		break;
	default:
		break;
	}
	return ret;
}

/*****  SOFTWARE EVENT SOURCE   *****/

SoftwareEventSource::SoftwareEventSource() {};
SoftwareEventSource::~SoftwareEventSource() {}
bool SoftwareEventSource::isThereEvent() { return false; };