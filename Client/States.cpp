#include "States.h"
#include "Screen.h"
#include "Events.h"
#include <fstream>
#include <sstream>

//ST_Idle

genericState *ST_Idle::on_Put(genericEvent *ev, usefulInfo *Info)
{
	EV_Put * PEv = (EV_Put *)ev;
	//SendWRQ(PEv->getSelectedFile())	//Funcion que envia el WRQ con el nombre del archivo
	genericState *ret = (genericState *) new ST_ReceiveFirstAck();
	ret->setFileToTransfer(PEv->getSelectedFile());	//Le indica al estado ST_receiveFirstAck cual es el archivo que se envio. En caso de recibir
													//timeout se utilizara esta info para reenviar el WRQ del archivo correspondiente.
	Info->timeout->startTimer();	//Inicia el timer
	return ret;
}

genericState *ST_Idle::on_Get(genericEvent *ev, usefulInfo *Info)
{
	EV_Get * GEv = (EV_Get *)ev;
	//sendRRQ(GEv->getSelectedFile());	//Funcion que envia el RRQ con el nombre del archivo
	genericState *ret = (genericState *) new ST_ReceiveFirstData;
	ret->setFileToTransfer(GEv->getSelectedFile());	//Le indica al estado ST_ReceiveFirstAck cual es el archivo que se envio.
	Info->timeout->startTimer();	//Inicia el timer
	return ret;
}

genericState *ST_Idle::on_HelpRequest(genericEvent *ev)
{
	EV_Help * HEvent = (EV_Help *)ev;	//Rutina de accion que muestra instrucciones en pantalla
	HEvent->terminal->outputHelp();
	genericState *ret = (genericState*) new ST_Idle;
	return ret;
}

genericState *ST_Idle::on_CloseClient(genericEvent* ev)
{
	genericState *ret = (genericState*) new ST_Idle;
	ret->setLastEvent(QUIT);
	return ret;
}

genericState *ST_Idle::on_ClearTerminal(genericEvent *ev)
{
	EV_Clear* CEvent = (EV_Clear *)ev;
	CEvent->terminal->resetTerminal();	//Rutina de accion que borra el terminal
	genericState *ret = (genericState*) new ST_Idle;
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


//ST_ReceiveFirstAck
genericState * ST_ReceiveFirstAck::on_Ack(genericEvent * ev)
{
	//sendData();	//TODO
	return (genericState*) new ST_ReceiveAck;
}

genericState * ST_ReceiveFirstAck::on_Error(genericEvent * ev)
{
	return (genericState*) new ST_Idle;
}

genericState * ST_ReceiveFirstAck::on_Timeout(genericEvent * ev)
{
	//sendWRQ(fileToTransfer);	//TODO
	return nullptr;
}

genericState * ST_ReceiveFirstAck::on_ConnectionFailed(genericEvent * ev)
{
	//y aca qe verga hacemo? //TODO
	//yo diria que deberiamos salir a reintentar conectar
	return nullptr;
}


//ST_ReceiveAck
genericState * ST_ReceiveAck::on_Ack(genericEvent * ev)
{
	//dataReceived();	//TODO	recien cuando llega el ack de la data enviada se mueve el FILE* para apuntar ala siguiente data
	//if(sendData())	//TODO
	//	return (genericState*) new ST_ReceiveLastData;
	return nullptr;
}

genericState * ST_ReceiveAck::on_Error(genericEvent * ev)
{
	return (genericState*) new ST_Idle;
}

genericState * ST_ReceiveAck::on_Timeout(genericEvent * ev)
{
	//sendData();	//TODO. Como no se invoca a la funcion dataReceived, se sigue apuntando al mismo paquete que antes
	return nullptr;
}

genericState * ST_ReceiveAck::on_ConnectionFailed(genericEvent * ev)
{
	return nullptr;
}

genericState * ST_ReceiveAck::on_LastData(genericEvent * ev)
{
	return (genericState*) new ST_ReceiveLastAck;
}

//ST_ReceiveLastAck
genericState * ST_ReceiveLastAck::on_Ack(genericEvent * ev)
{
	//TODO: mostrar que es ya se termino de enviar el mensaje
	return (genericState*) new ST_Idle;
}

genericState * ST_ReceiveLastAck::on_Error(genericEvent * ev)
{
	//TODO: mostrar error
	return (genericState*) new ST_Idle;
}

genericState * ST_ReceiveLastAck::on_Timeout(genericEvent * ev)
{
	//sendData()	//TODO
	return nullptr;
}

genericState * ST_ReceiveLastAck::on_ConnectionFailed(genericEvent * ev)
{
	return nullptr;
}

//ST_ReceiveFirstData
genericState * ST_ReceiveFirstData::on_Data(genericEvent * ev)
{
	//sendAck();
	return (genericState*) new ST_ReceiveData;
}

genericState * ST_ReceiveFirstData::on_Error(genericEvent * ev)
{
	return (genericState*) new ST_Idle;
}

genericState * ST_ReceiveFirstData::on_Timeout(genericEvent * ev)
{
	//sendRRQ()	//TODO
	return nullptr;
}

genericState * ST_ReceiveFirstData::on_ConnectionFailed(genericEvent * ev)
{
	return nullptr;
}

//ST_ReceiveData
genericState * ST_ReceiveData::on_Data(genericEvent * ev)
{
	//sendAck() //TODO
	return nullptr;
}

genericState * ST_ReceiveData::on_Error(genericEvent * ev)
{
	return (genericState*) new ST_Idle;
}

genericState * ST_ReceiveData::on_Timeout(genericEvent * ev)
{
	//sendAck() //TODO
	return nullptr;
}

genericState * ST_ReceiveData::on_ConnectionFailed(genericEvent * ev)
{
	return nullptr;
}

genericState * ST_ReceiveData::on_LastData(genericEvent * ev)
{
	return (genericState*) new ST_Idle;
}