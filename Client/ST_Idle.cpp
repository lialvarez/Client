#include "States.h"
#include "Screen.h"
#include "Events.h"
#include <fstream>
#include <sstream>

genericState *ST_Idle::on_HelpRequest(genericEvent *ev)
{
	outputHelp();	//Rutina de accion que muestra instrucciones en pantalla
	genericState *ret = (genericState*) new ST_Idle();
	return ret;
}

genericState *ST_Idle::on_CloseClient(genericEvent* ev)
{
	genericState *ret = (genericState*) new ST_Idle();
	ret->setLastEvent(QUIT);
	return ret;
}

genericState *ST_Idle::on_ClearTerminal(genericEvent *ev)
{
	genericState *ret = (genericState*) new ST_Idle();
	resetTerminal();	//Rutina de accion que borra el terminal
	return ret;
}

genericState *ST_Idle::on_FileError(genericEvent *ev)
{
	EV_FileError * FEEvent = (EV_FileError *)ev;	//Rutina de accion que indica que se ingreso archivo invalido
	genericState *ret = (genericState*) new ST_Idle();
	return ret;
}

genericState *ST_Idle::on_InvalidCommand(genericEvent *ev)
{

}
