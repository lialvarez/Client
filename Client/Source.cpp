#include "parseCmdLine.h"
#include "usefulInfo.h"
#include "eventGenerator.h"
#include "genericFSM.h"
#include "eventSources.h"
#include "Screen.h"
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
	Screen Terminal;
	UserEventSource userSource(&Terminal);
	TimeoutEventSource Timeout;
	usefulInfo Info(userData.serverAddress, &userSource, &Timeout);	//Se crea la instancia de usefulInfo
	eventGenerator evGen(&Info);	//creo la instancia del generador de eventos
	genericEvent *ev;
	genericFSM FSM;

	unsigned int connectAttempt = 0;
	do
	{
		//do
		//{
		//	//conectar()
		//	//Seguramente va a haber que manejar una excepcion si no logra conecatarse
		//	connectAttempt++;
		//} while (connectAttempt < MAX_ATTEMPT && /*connection failed*/);	//Intento conectar un numero maximo de intentos
		//connectAttempt = 0;	//Reseteo el connect attempt
		//
		//if (/*connection failed*/)	//Si no hay conexion salgo del programa
		//{
		//	//Salir del programa, no se pudo establecer conexion
		//}

		do //Si hay conexion, entro en la FSM
		{
			evGen.generateEvent();	//Chequea todas las fuentes de estados
			ev = evGen.getNextEvent();	//Carga el evento en ev

			if (ev != nullptr)
			{
				FSM.dispatch(ev, &Info);
			}

		} while (FSM.getCurrentState()->getLastEvent() != QUIT);

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