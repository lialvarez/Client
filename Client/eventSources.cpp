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
	unsigned char blockLow, blockHigh;
	bool ret = false;
	std::vector<char> aux;
	if (networkInterface->receivePackage())	//verifica si se recibio algo
	{
		switch (networkInterface->getInputPackage()[1])	//segun el tipo de paquete devuelvo el tipo de evento
		{
		case DATA_OP:
			aux = std::vector<char>(networkInterface->getInputPackage());
			data = std::vector<char>(aux.begin() + 4, aux.end());
			blockLow = networkInterface->getInputPackage()[3];
			blockHigh = networkInterface->getInputPackage()[2];

			blockNumber = blockHigh;
			blockNumber = (blockNumber << 8) + blockLow;
			
			if (blockNumber != expectedBlockNum)
			{
				ret = true;
				evCode = ERRO;
			}
			else
			{
				ret = true;
				if (data.size() < 512)
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
			blockLow = networkInterface->getInputPackage()[3];
			blockHigh = networkInterface->getInputPackage()[2];

			blockNumber = blockHigh;
			blockNumber = (blockNumber << 8) + blockLow;
		
			if (blockNumber != expectedBlockNum)
			{
				ret = true;
				evCode = ERRO;
			}
			else
			{
				ret = true;
				evCode = ACK;
			}
			break;
		case ERROR_OP:
			errorCode = (errorCodes)networkInterface->getInputPackage()[3];
			if (errorCode == NOT_DEFINED)
			{
				errorMsg = (char *)networkInterface->getInputPackage()[4];
			}
			else
			{
				errorMsg = "";
			}
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
		ret = (genericEvent *) new EV_Data();
		break;
	case LAST_DATA:
		ret = (genericEvent *) new EV_LastData();
		break;
	case ACK:
		ret = (genericEvent *) new EV_Ack();
		break;
	case ERRO:
		ret = (genericEvent *) new EV_Error();
		break;
	default:
		break;
	}
	return ret;
}

/*****  USER EVENT SOURCE  *****/

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

std::string UserEventSource::getOrigFileToTransfer()
{
	return origFileToTransfer;
}

bool UserEventSource::isThereEvent()
{
	std::size_t found;
	std::string auxstr;
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
		fileToTransfer = command.substr(command.find(' ') + 1);
		command = command.substr(0, command.find(' '));
		std::transform(command.begin(), command.end(), command.begin(), tolower);
		origFileToTransfer = fileToTransfer;
		if (command.length() == 0)
		{
			evCode = EMPTY_COMMAND;
			ret = true;
		}
		else
		{
			if (command.compare("help") == 0)
			{
				evCode = HELP;
				ret = true;
			}
			else if (command.compare("quit") == 0)
			{
				evCode = QUIT;
				ret = true;
			}
			else if (command.compare("clear") == 0)
			{
				evCode = CLEAR;
				ret = true;
			}
			else if (command.compare("put") == 0)
			{

				auxFile.open(fileToTransfer);
				if (auxFile.fail())
				{
					ret = true;
					evCode = FILE_ERROR;
				}
				else
				{
					auxFile.close();
					evCode = PUT;
					auxstr = fileToTransfer;
					do
					{
						found = auxstr.find('/');
						auxstr = auxstr.substr(found + 1);
					} while (found != std::string::npos);
					fileToTransfer = auxstr;
					ret = true;
				}
			}
			else if (command.compare("get") == 0)
			{
				evCode = GET;
				auxstr = fileToTransfer;
				do
				{
					found = auxstr.find('/');
					auxstr = auxstr.substr(found + 1);
				} while (found != std::string::npos);
				fileToTransfer = auxstr;
				ret = true;
			}
			else
			{
				evCode = INVALID;
				ret = true;
			}
		}
		terminal->setCommandLine();
		buffer.clear();	//Limpia el buffer
		break;

	default:
		//Si el caracter ingresado es un caracter valido(ningun caracter de control)
		//lo añade al buffer
		if (inputChar > ASCII_START && inputChar < ASCII_END)
		{
			buffer.push_back(inputChar);
			ret = false;
		}
		terminal->inputPos++;
		break;
	}
	return ret;
}

genericEvent * UserEventSource::insertEvent()
{
	genericEvent * ret;
	switch (evCode)
	{
	case PUT:
		ret = (genericEvent *) new EV_Put();
		break;
	case GET:
		ret = (genericEvent *) new EV_Get();
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
		ret = (genericEvent *) new EV_InvalidCommand();
		break;
	case FILE_ERROR:
		ret = (genericEvent *) new EV_FileError();
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
	timeoutsCount = 0;
}

bool TimeoutEventSource::isThereEvent()
{
	if (((clock() - tInicial) > ONE_MINUTE * CLOCKS_PER_SEC) && timerRunning)
	{
		timeout = true;
		timerRunning = false;
		timeoutsCount++;
		if (timeoutsCount == MAX_TIMEOUTS)
		{
			evCode = CONNECTION_FAIL;
		}
		else
		{
			evCode = TIMEOUT;
		}
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
	timeoutsCount = 0;
}

void TimeoutEventSource::stopTimer()
{
	timerRunning = false;
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

bool SoftwareEventSource::isThereEvent() 
{
	if (fileInterface->lastData)
	{
		evCode = LAST_DATA;
		ev= true;
	}
	return ev;
};

genericEvent* SoftwareEventSource::insertEvent()
{
	genericEvent *ret;
	if (evCode == LAST_DATA)
	{
		ret = (genericEvent *) new EV_LastData();
	}
	ev = false;
	return ret;
}