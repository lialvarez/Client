#include "parseCmdLine.h"
#include "usefulInfo.h"
#include "eventGenerator.h"
#include "genericFSM.h"
#include <string>

#define CALLBACK_ERROR	 0
#define CALLBACK_OK		 1

int inputCallback(char *key, char *value, void * userData);

typedef struct
{
	std::string serverAddress;
	unsigned int argCount;
}userData_t;


int main(int argc, char *argv[])
{
	userData_t userData;

	usefulInfo I(userData.serverAddress);	//Se crea la instancia de usefulInfo
	eventGenerator eg(&I);	//creo la instancia del generador de eventos
	genericEvent *ev;
	genericFSM fsm;

	do
	{
		eg.generateEvent();
		ev = eg.getNextEvent();

		if (ev != nullptr)
		{
			fsm.dispatch(ev);
		}
	} while (fsm.getCurrentState()->getLastEvent() != QUIT);

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