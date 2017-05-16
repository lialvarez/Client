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
	unsigned int blockLow, blockHigh;
	bool ret = false;
	std::vector<char> aux;
	if (networkInterface->receivePackage())	//verifica si se recibio algo
	{
		switch (networkInterface->getInputPackage()[1])	//segun el tipo de paquete devuelvo el tipo de evento
		{
		case DATA_OP:
			aux = std::vector<char>(networkInterface->getInputPackage());
			data = std::vector<char>(aux.begin() + 4, aux.end());
			blockLow = (unsigned int)networkInterface->getInputPackage()[3];
			blockHigh = (unsigned int)networkInterface->getInputPackage()[2];
			blockNumber = ((blockHigh & 0x00FF) << 8) + (blockLow & 0x00FF);
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
			blockNumber = (networkInterface->getInputPackage()[2] << 8) + networkInterface->getInputPackage()[3];
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
		boost::split(words, command, boost::is_any_of(", "), boost::token_compress_on);	//Se separan las palabras ingresadas
		std::transform(words[0].begin(), words[0].end(), words[0].begin(), tolower);	//transformo la linea ingresada a minuscula
		

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
				evCode = GET;
				ret = true;
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
		//lo añade al buffer
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
	bool ret = false;
	if (fileInterface->lastData)
	{
		evCode = LAST_DATA;
		ret = true;
	}
	return ret;
};

genericEvent* SoftwareEventSource::insertEvent()
{
	genericEvent *ret;
	if (evCode == LAST_DATA)
	{
		ret = (genericEvent *) new EV_LastData();
	}
	return ret;
}