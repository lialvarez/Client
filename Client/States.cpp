#include "States.h"
#include "Screen.h"
#include "Events.h"
#include <fstream>
#include <sstream>

//ST_Idle

genericState *ST_Idle::on_Put(EV_Put *ev, usefulInfo *Info)
{
	Info->networkSrc->expectedBlockNum = 0;	//Setea el block number que deberia tener el proximo ACK
	Info->fileInterface->openFile(ev->getSelectedFile(), READ);	//Abre el archivo y guarda el puntero al mismo en Info.
	Info->networkSrc->networkInterface->sendPackage(ev->WRQPkg);	//Funcion que envia el WRQ con el nombre del archivo
	Info->lastPkg = new WriteRequest(*ev->WRQPkg);	//Copia el ultimo paquete enviado
	Info->timeoutSrc->startTimer();	//Inicia el timer
	genericState *ret = (genericState *) new ST_ReceiveFirstAck();
	return ret;
}

genericState *ST_Idle::on_Get(EV_Get *ev, usefulInfo *Info)
{
	Info->networkSrc->expectedBlockNum = 0;	//block num que debe tener el proximo DATA
	Info->networkSrc->networkInterface->sendPackage(ev->RRQPkg);	//Funcion que envia el RRQ con el nombre del archivo
	Info->timeoutSrc->startTimer();	//Inicia el timer
	genericState *ret = (genericState *) new ST_ReceiveFirstData;
	return ret;
}

genericState *ST_Idle::on_HelpRequest(EV_Help *ev, usefulInfo *Info)
{
	Info->userInterface->outputHelp();	//Rutina que muestra el mensaje de ayuda en pantalla
	genericState *ret = (genericState*) new ST_Idle;
	return ret;
}

genericState *ST_Idle::on_CloseClient(EV_Quit* ev)
{
	genericState *ret = (genericState*) new ST_Idle;
	ret->setLastEvent(QUIT);
	return ret;
}

genericState *ST_Idle::on_ClearTerminal(EV_Clear *ev, usefulInfo *Info)
{
	Info->userInterface->resetTerminal();	//Rutina de accion que borra el terminal
	genericState *ret = (genericState*) new ST_Idle;
	return ret;
}

genericState *ST_Idle::on_FileError(EV_FileError *ev, usefulInfo *Info)
{
	Info->userInterface->fileErrorMsg(Info->userSrc->getFileToTransfer());	//Rutina de accion que indica que se ingreso archivo invalido
	genericState *ret = (genericState*) new ST_Idle();
	return ret;
}

genericState *ST_Idle::on_InvalidCommand(EV_InvalidCommand *ev, usefulInfo *Info)
{
	Info->userInterface->outputInvalid(Info->userSrc->getCommand());	//Rutina que indica por pantalla que se ingreso comando invalido
	genericState * ret = (genericState *) new ST_Idle();
	return ret;
}

genericState *ST_Idle::on_EmptyCommand(EV_EmptyCommand *ev, usefulInfo *Info)
{
	genericState *ret = (genericState *) new ST_Idle();
	Info->userInterface->setCommandLine();
	return ret;
}


//ST_ReceiveFirstAck
genericState * ST_ReceiveFirstAck::on_Ack(EV_Ack * ev, usefulInfo *Info)
{
	//Recibo el Ack y tengo que empezar a enviar la data
	Info->networkSrc->expectedBlockNum++;
	Info->nextPkg = new Data(Info->fileInterface->readData(), Info->networkSrc->expectedBlockNum);
	Info->networkInterface->sendPackage(Info->nextPkg);
	Info->timeoutSrc->startTimer();		//Resetear Timer
	return (genericState*) new ST_ReceiveAck;
}

genericState * ST_ReceiveFirstAck::on_Error(EV_Error * ev, usefulInfo *Info)
{
	Info->closeFile();	//Cerrar el archivo
	Info->timeoutSrc->stopTimer();	//Detener el timer
	genericState *ret = (genericState *) new ST_Idle;
	//Hay que limpiar el buffer de eventos tambien, quizas verificando fuera de la FSM si last Event fue ERROR
	ret->setLastEvent(ERRO);	//Setear el ultimo evento en Error para resetear la FSM
	return (genericState*) new ST_Idle;
}

genericState * ST_ReceiveFirstAck::on_Timeout(EV_Timeout * ev, usefulInfo *Info)
{
	Info->networkInterface->sendPackage(Info->lastPkg);	//Envia el ulimo paquete enviado
	Info->timeoutSrc->startTimer();
	return nullptr;
}

genericState * ST_ReceiveFirstAck::on_ConnectionFailed(EV_ConnectionFailed * ev)
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
	Info->userSrc->terminal->fileSendEnd(Info->userSrc->getFileToTransfer());	//Se muestra en pantalla que el envio fue exitoso
	return (genericState*) new ST_Idle;
}

genericState * ST_ReceiveLastAck::on_Error(genericEvent * ev)
{
	genericState *ret = (genericState*) new ST_Idle;
	ret->setLastEvent(ERRO);
	return ret;;
}

genericState * ST_ReceiveLastAck::on_Timeout(genericEvent * ev, usefulInfo *Info)
{
	Info->networkSrc->networkInterface->reSendData();		//Esta funcion la estaba haciendo Male
	Info->timeoutSrc->startTimer();	//Reinicia el timer
	return nullptr;
}

genericState * ST_ReceiveLastAck::on_ConnectionFailed(genericEvent * ev)
{
	genericState *ret = (genericState *) new ST_Idle();
	ret->setLastEvent(CONNECTION_FAIL);
	return ret;
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