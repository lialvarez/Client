#include "eventSources.h"
#include "Screen.h"
#include "Events.h"
#include <boost\algorithm\string\classification.hpp>
#include <boost\algorithm\string\split.hpp>
#include <boost\date_time\posix_time\posix_time.hpp>

/*****  NETWORK EVENT SOURCE  *****/

NetworkEventSource::NetworkEventSource(){};
NetworkEventSource::~NetworkEventSource() {};
bool NetworkEventSource::isThereEvent() { return false; } //MALE: esta es la funcion que lee lo que le envian por red
void NetworkEventSource::setServerIP(std::string _serverIP)
{
	serverIP = _serverIP;
}

std::string NetworkEventSource::getServerIP()
{
	return serverIP;
}

/*****  USER EVENT SOURCE  *****/

UserEventSource::UserEventSource()
{
	initTerminal();	//Inicializa la terminal por donde interactua el usuario
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
		if (terminalWindow->_curx == 6)	//Posicion donde arranca la linea de comando
		{
			terminalWindow->_curx++;	//Para que el cursor no pse vaya del area permitida
		}
		ret = false;	//No hay evento
		break;

	case ENTER:

		command = std::string(buffer.begin(), buffer.end());	//Almaceno la linea de comando ingresada en command
		std::transform(command.begin(), command.end(), command.begin(), tolower);	//
		boost::split(words, command, boost::is_any_of(", "), boost::token_compress_on);	//Se separan las palabras ingresadas

		if (words.size() == 0)
		{
			evCode = EMPTY_COMMAND;
			ret = true;
		}
		else if (words.size() == 1)
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
		else if (words.size() == 2)
		{
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
					fileToTransfer = words[1];
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
					fileToTransfer = words[1];
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
		else
		{
			ret = true;
			evCode = INVALID;
		}
		buffer.clear();
		words.clear();
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
	case NO_EV:
		ret = (genericEvent *) new EV_NoEvent;
		break;
	case PUT:
		ret = (genericEvent *) new EV_SendWRQ;
		break;
	case GET:
		ret = (genericEvent *) new EV_SendRRQ;
		break;
	case HELP:
		ret = (genericEvent *) new EV_HelpRequest;
		break;
	case CLEAR:
		ret = (genericEvent *) new EV_ClearScreen;
		break;
	case INVALID:
		ret = (genericEvent *) new EV_InvalidCommand;
		break;
	case FILE_ERROR:
		ret = (genericEvent *) new EV_FileError;
		break;
	default:
		break;
	}
	return ret;
}

/*****  TIMEOUTS EVENT SOURCE  *****/

bool TimeoutEventSource::isThereEvent()
{
	if (timeout)
	{
		evCode = TIMEOUT;
		return true;
	}
	else
	{
		evCode = NO_EV;
		return false;
	}
}

void TimeoutEventSource::setTimeout()
{
	timeout = true;	//Set timeout modifica una variable de control que indica si ocurrio un timeout
}

void TimeoutEventSource::startTimer()
{
	timer.async_wait(&setTimeout);	//Cuando transcurra el tiempo seteado, se llamara al metodo "setTimeout"
	timeout = false;	//Se setea la variable de control en false, indicando que no ha ocurrido timeout
}

void TimeoutEventSource::stopTimer()
{
	timer.cancel();	//Se cancela el timer
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
		ret = 
		break;
	}
}
/*****  SOFTWARE EVENT SOURCE   *****/

SoftwareEventSource::SoftwareEventSource() {};
SoftwareEventSource::~SoftwareEventSource() {}
bool SoftwareEventSource::isThereEvent() { return false; };