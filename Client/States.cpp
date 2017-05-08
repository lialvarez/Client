#include "States.h"
#include "Screen.h"
#include "Events.h"
#include <fstream>
#include <sstream>

//ST_Idle

genericState *ST_Idle::on_Put(genericEvent *ev, usefulInfo *Info)
{
	EV_Put * PEv = (EV_Put *)ev;
	Info->setFilePointer(PEv->getSelectedFile());	//Abre el archivo y guarda el puntero al mismo en Info.
	Info->blockNumber = 0;
	Info->networkSrc->networkInterface->sendWRQ(PEv->getSelectedFile());	//Funcion que envia el WRQ con el nombre del archivo
	genericState *ret = (genericState *) new ST_ReceiveFirstAck();
	ret->setFileToTransfer(PEv->getSelectedFile());	//Le indica al estado ST_receiveFirstAck cual es el archivo que se envio. En caso de recibir
													//timeout se utilizara esta info para reenviar el WRQ del archivo correspondiente.
	Info->timeoutSrc->startTimer();	//Inicia el timer
	return ret;
}

genericState *ST_Idle::on_Get(genericEvent *ev, usefulInfo *Info)
{
	EV_Get * GEv = (EV_Get *)ev;
	Info->blockNumber = 0;
	Info->networkSrc->networkInterface->sendRRQ(GEv->getSelectedFile());	//Funcion que envia el RRQ con el nombre del archivo
	Info->timeoutSrc->startTimer();	//Inicia el timer
	genericState *ret = (genericState *) new ST_ReceiveFirstData;
	ret->setFileToTransfer(GEv->getSelectedFile());	//Le indica al estado ST_ReceiveFirstAck cual es el archivo que se envio.
	return ret;
}

genericState *ST_Idle::on_HelpRequest(genericEvent *ev, usefulInfo *Info)
{
	Info->userSrc->terminal->outputHelp();	//Rutina que muestra el mensaje de ayuda en pantalla
	genericState *ret = (genericState*) new ST_Idle;
	return ret;
}

genericState *ST_Idle::on_CloseClient(genericEvent* ev)
{
	genericState *ret = (genericState*) new ST_Idle;
	ret->setLastEvent(QUIT);
	return ret;
}

genericState *ST_Idle::on_ClearTerminal(genericEvent *ev, usefulInfo *Info)
{
	Info->userSrc->terminal->resetTerminal();	//Rutina de accion que borra el terminal
	genericState *ret = (genericState*) new ST_Idle;
	return ret;
}

genericState *ST_Idle::on_FileError(genericEvent *ev, usefulInfo *Info)
{
	Info->userSrc->terminal->fileErrorMsg(Info->userSrc->getFileToTransfer());	//Rutina de accion que indica que se ingreso archivo invalido
	genericState *ret = (genericState*) new ST_Idle();
	return ret;
}

genericState *ST_Idle::on_InvalidCommand(genericEvent *ev, usefulInfo *Info)
{
	Info->userSrc->terminal->outputInvalid(Info->userSrc->getCommand());	//Rutina que indica por pantalla que se ingreso comando invalido
	genericState * ret = (genericState *) new ST_Idle();
	return ret;
}

genericState *ST_Idle::on_EmptyCommand(genericEvent *ev, usefulInfo *Info)
{
	genericState *ret = (genericState *) new ST_Idle();
	Info->userSrc->terminal->setCommandLine();
	return ret;
}


//ST_ReceiveFirstAck
genericState * ST_ReceiveFirstAck::on_Ack(genericEvent * ev, usefulInfo *Info)
{
	//Recibo el Ack y tengo que empezar a enviar la data
	Info->blockNumber++;
	Info->networkSrc->networkInterface->sendData(Info->getFilePointer(), Info->blockNumber);
	Info->timeoutSrc->startTimer();		//Resetear Timer
	return (genericState*) new ST_ReceiveAck;
}

genericState * ST_ReceiveFirstAck::on_Error(genericEvent * ev, usefulInfo *Info)
{
	Info->closeFile();	//Cerrar el archivo
	Info->timeoutSrc->stopTimer();	//Detener el timer
	genericState *ret = (genericState *) new ST_Idle;
	//Hay que limpiar el buffer de eventos tambien, quizas verificando fuera de la FSM si last Event fue ERROR
	ret->setLastEvent(ERRO);	//Setear el ultimo evento en Error para resetear la FSM
	return (genericState*) new ST_Idle;
}

genericState * ST_ReceiveFirstAck::on_Timeout(genericEvent * ev, usefulInfo *Info)
{
	//sendWRQ(fileToTransfer);	//TODO
	Info->networkSrc->networkInterface->sendWRQ(fileToTransfer);
	Info->timeoutSrc->startTimer();
	return nullptr;
}

genericState * ST_ReceiveFirstAck::on_ConnectionFailed(genericEvent * ev)
{
	//y aca qe verga hacemo? //TODO
	//yo diria que deberiamos salir a reintentar conectar
	return nullptr;
}


//ST_ReceiveAck
genericState * ST_ReceiveAck::on_Ack(genericEvent * ev, usefulInfo *Info)
{
	Info->blockNumber++;	//Incrementa el numero de bloque
	Info->networkSrc->networkInterface->sendData(Info->getFilePointer(), Info->blockNumber);	//Se envia el siguiente bloque
	Info->timeoutSrc->startTimer();	//Inicia el timer
	return nullptr;
}

genericState * ST_ReceiveAck::on_Error(genericEvent * ev)
{
	genericState *ret = (genericState*) new ST_Idle;
	ret->setLastEvent(ERRO);
	return ret;
}

genericState * ST_ReceiveAck::on_Timeout(genericEvent * ev, usefulInfo *Info)
{	
	Info->networkSrc->networkInterface->reSendData();		//Esta funcion la estaba haciendo Male
	Info->timeoutSrc->startTimer();	//Reinicia el timer
	return nullptr;
}

genericState * ST_ReceiveAck::on_ConnectionFailed(genericEvent * ev)
{
	genericState *ret = (genericState *) new ST_Idle();
	ret->setLastEvent(CONNECTION_FAIL);
	return ret;
}

genericState * ST_ReceiveAck::on_LastData(genericEvent * ev, usefulInfo *Info)
{
	Info->blockNumber++;	//Incremento el numero de bloque
	Info->networkSrc->networkInterface->sendData(Info->getFilePointer(), Info->blockNumber);	//envio la data
	Info->timeoutSrc->startTimer();	//Reinicio el timer
	return (genericState*) new ST_ReceiveLastAck;
}

//ST_ReceiveLastAck
genericState * ST_ReceiveLastAck::on_Ack(genericEvent * ev, usefulInfo *Info)
{	
	Info->closeFile();	//Como se finalizo el envio, se cierra el archivo.
	Info->userSrc->terminal->
	//TODO: mostrar que es ya se termino de enviar el mensaje
	return (genericState*) new ST_Idle;
}

genericState * ST_ReceiveLastAck::on_Error(genericEvent * ev)
{
	//TODO: mostrar error
	return (genericState*) new ST_Idle;
}

genericState * ST_ReceiveLastAck::on_Timeout(genericEvent * ev, usefulInfo *Info)
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