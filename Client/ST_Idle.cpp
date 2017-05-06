#include "States.h"
#include "Screen.h"
#include "Events.h"
#include <fstream>
#include <sstream>

genericState *ST_Idle::on_HelpRequest(genericEvent *ev)
{
	EV_Help * HEvent = (EV_Help *)ev;	//Rutina de accion que muestra instrucciones en pantalla
	HEvent->terminal->outputHelp();
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
	EV_Clear* CEvent = (EV_Clear *)ev;
	CEvent->terminal->resetTerminal();	//Rutina de accion que borra el terminal
	genericState *ret = (genericState*) new ST_Idle();
	return ret;
}

genericState *ST_Idle::on_FileError(genericEvent *ev)
{
	EV_FileError * FEEvent = (EV_FileError *)ev;	//Rutina de accion que indica que se ingreso archivo invalido
	FEEvent->terminal->fileErrorMsg(FEEvent->getFileNotFound());
	genericState *ret = (genericState*) new ST_Idle();
	return ret;
}

genericState *ST_Idle::on_InvalidCommand(genericEvent *ev)
{
	EV_InvalidCommand * ICEvent = (EV_InvalidCommand *)ev;
	ICEvent->terminal->outputInvalid(ICEvent->getInvalidCommand());
	genericState * ret = (genericState *) new ST_Idle();
	return ret;
}

genericState *ST_Idle::on_EmptyCommand(genericEvent *ev)
{
	genericState *ret = (genericState *) new ST_Idle();
	EV_InvalidCommand *ECEvent = (EV_InvalidCommand *)ev;
	ECEvent->terminal->setCommandLine();
	return ret;
}
