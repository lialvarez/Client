
#include "eventSources.h"
#include "Screen.h"
#include "Events.h"
#include <boost\algorithm\string\classification.hpp>
#include <boost\algorithm\string\split.hpp>
#include <boost\date_time\posix_time\posix_time.hpp>
#include <boost/bind.hpp>

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

	EV_Put *castedEvP;
	EV_Get *castedEvG;
	EV_InvalidCommand * castedEvIC;
	EV_FileError *castedEvFE;
	switch (evCode)
	{
	case PUT:
		ret = (genericEvent *) new EV_Put(terminal);
		castedEvP = (EV_Put *)ret;
		castedEvP->setSelectedFile(command);
		break;
	case GET:
		ret = (genericEvent *) new EV_Get(terminal);
		castedEvG = (EV_Get *)ret;
		castedEvG->setSelectedFile(command);
		break;
	case HELP:
		ret = (genericEvent *) new EV_Help(terminal);
		break;
	case CLEAR:
		ret = (genericEvent *) new EV_Clear(terminal);
		break;
	case EMPTY_COMMAND:
		ret = (genericEvent *) new EV_EmptyCommand(terminal);
		break;
	case INVALID:
		ret = (genericEvent *) new EV_InvalidCommand(terminal);
		castedEvIC = (EV_InvalidCommand *)ret;
		castedEvIC->setInvalidCommand(command);
		break;
	case FILE_ERROR:
		ret = (genericEvent *) new EV_FileError(terminal);
		castedEvFE = (EV_FileError *)ret;
		castedEvFE->setFileNotFound(fileToTransfer);
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

//NO TOCAR LOS DE TIMEOUT, CREO QUE YA ESTAN LISTOS

bool TimeoutEventSource::isThereEvent()
{
	ioForTimer.poll();			//Si se cumple el tiempo del timer, se ejecuta el handler correspondiente.

	bool ret = timeout;			//Para que luego devuelva true si se cumplio timeout y false en caso contrario.

	if (timeout)
	{
		evCode = TIMEOUT;
		//timeout = false;		//vuelvo a poner en false para que espere al siguiente timeout.
		startTimer();			//Se reinicia el timer.
	}
	else
	{
		evCode = NO_EV;
	}
	return ret;					//Se devuelve si hubo timeout o no.
}

void TimeoutEventSource::setTimeout(const boost::system::error_code& /*e*/) //VER DE USARLA
{
	//timeout = true;			//Set timeout modifica una variable de control que indica si ocurrio un timeout
}

//constructor de prueba para el bind
/*
 TimeoutEventSource::TimeoutEventSource()
{
	HANDLER123=(boost::bind(&TimeoutEventSource::handler, this,
		boost::asio::placeholders::error, &t));
}
*/

void TimeoutEventSource::startTimer()
{
	timeout = false;			//Variable de control indicando que no ocurrio un timeout.

	//boost::asio::deadline_timer t(ioForTimer, boost::posix_time::seconds(60)); 

	boost::function<void(const boost::system::error_code&, boost::asio::deadline_timer*)> HANDLER123( //le saque lo segundo
		boost::bind(&TimeoutEventSource::handler, this,
			boost::asio::placeholders::error, &t)); //VER

	t.async_wait(boost::bind(HANDLER123,&t));

	//t.async_wait(boost::bind(HANDLER123,boost::asio::placeholders::error, &t));
}

void TimeoutEventSource::stopTimer()
{
	//timer.cancel();	//Se cancela el timer. No anda esto. ESTOY PROBANDO CON OTRAS
}


//////////ver
void TimeoutEventSource::handler(const boost::system::error_code&, boost::asio::deadline_timer* t)  //PRUEBA TIMER (le agrego 2 params para que repita)
{
	timeout = true;				//Se indica que ocurrió un timeout.
}
/////////////

genericEvent * TimeoutEventSource::insertEvent() 
{
	genericEvent * ret;
	//Hago un switch solo para mantener la estructura del metodo en las otras clases
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