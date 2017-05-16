#include "parseCmdLine.h"
#include "usefulInfo.h"
#include "eventGenerator.h"
#include "genericFSM.h"
#include "eventSources.h"
#include "Screen.h"
#include "Networking.h"
#include <string>

#define CALLBACK_ERROR	0
#define CALLBACK_OK		1
#define MAX_ATTEMPT		5

int inputCallback(char *key, char *value, void * userData);

typedef struct
{
	std::string serverAddress;
	unsigned int argCount;
}userData_t;


int main(int argc, char *argv[])
{
	userData_t userData;
	userData.serverAddress = std::string(argv[1]);


	Screen Terminal;
	FileSystem fileSystem;
	Networking Network(userData.serverAddress);
	NetworkEventSource networkSource(&Network);
	UserEventSource userSource(&Terminal);
	TimeoutEventSource timeoutSource;
	SoftwareEventSource Software;
	usefulInfo Info(userData.serverAddress, &userSource, &timeoutSource, &networkSource, &fileSystem, &Software);	//Se crea la instancia de usefulInfo
	eventGenerator evGen(&Info);	//creo la instancia del generador de eventos
	genericEvent *ev;
	genericFSM FSM;

	

	unsigned int connectAttempt = 0;
	do
	{
		Network.startConnection();

		do //Si hay conexion, entro en la FSM
		{
			evGen.generateEvent();	//Chequea todas las fuentes de estados
			ev = evGen.getNextEvent();	//Carga el evento en ev

			if (ev != nullptr)
			{
				FSM.dispatch(ev, &Info);
			}

		} while (FSM.getCurrentState()->getLastEvent() != QUIT);
		evGen.clearBuffer();

	} while (FSM.getCurrentState()->getLastEvent() != QUIT);
	
}

int inputCallback(char *key, char *value, void * userData)
{
	int ret;
	userData_t * ud = (userData_t *)userData;
	std::string val(value);
	ud->argCount++;
	if (ud->argCount < 2)
	{
		ud->serverAddress = val;
		ret = CALLBACK_OK;
	}
	else
	{
		ret = CALLBACK_ERROR;
	}
	return ret;
}