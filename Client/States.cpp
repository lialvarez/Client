#include "States.h"
#include "Screen.h"
#include <fstream>
#include <sstream>

//ST_Idle

genericState *ST_Idle::on_Put(EV_Put *ev, usefulInfo *Info)
{
	Info->networkSrc->expectedBlockNum = 0;	//Setea el block number que deberia tener el proximo ACK
	Info->fileInterface->openFile(ev->getSelectedFile(), READ);	//Abre el archivo en el modo indicado
	Info->nextPkg = new WriteRequest(*ev->WRQPkg);	//Copia el ultimo paquete enviado
	Info->networkInterface->sendPackage((WriteRequest *)Info->nextPkg);	//Funcion que envia el WRQ con el nombre del archivo
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
	delete Info->nextPkg;
	Info->nextPkg =(Data *) new Data(Info->fileInterface->readData(), Info->networkSrc->expectedBlockNum);	//Armo el paquete a enviar
	Info->networkInterface->sendPackage(Info->nextPkg);
	Info->timeoutSrc->startTimer();		//Resetear Timer
	return (genericState*) new ST_ReceiveAck;
}

genericState * ST_ReceiveFirstAck::on_Error(EV_Error * ev, usefulInfo *Info)
{
	Info->fileInterface->closeFile();	//Cerrar el archivo
	delete Info->nextPkg;	//Elimino el ultimo paquete enviado
	Info->timeoutSrc->stopTimer();	//Detener el timer
	Info->userInterface->errorMsg((errorCodes)ev->errorPkg->errorCode, ev->errorPkg->errorMsg);
	genericState *ret = (genericState *) new ST_Idle;
	ret->setLastEvent(ERRO);	//Setear el ultimo evento en Error para resetear la FSM
	return ret;
}

genericState * ST_ReceiveFirstAck::on_Timeout(EV_Timeout * ev, usefulInfo *Info)
{
	Info->networkInterface->sendPackage(Info->nextPkg);	//Envia el ulimo paquete enviado
	Info->timeoutSrc->startTimer();
	return nullptr;
}

genericState * ST_ReceiveFirstAck::on_ConnectionFailed(EV_ConnectionFailed * ev, usefulInfo *Info)
{
	//y aca qe verga hacemo? //TODO
	//yo diria que deberiamos salir a reintentar conectar
	delete Info->nextPkg;
	Info->fileInterface->closeFile();
	return nullptr;
}


//ST_ReceiveAck
genericState * ST_ReceiveAck::on_Ack(EV_Ack * ev, usefulInfo *Info)
{
	Info->networkSrc->expectedBlockNum++;	//Incrementa el numero de bloque
	delete Info->nextPkg;
	Info->nextPkg = new Data(Info->fileInterface->readData(), Info->networkSrc->expectedBlockNum);
	Info->networkSrc->networkInterface->sendPackage(Info->nextPkg);	//Se envia el siguiente bloque
	Info->timeoutSrc->startTimer();	//Inicia el timer
	return nullptr;
}

genericState * ST_ReceiveAck::on_Error(EV_Error * ev, usefulInfo *Info)
{
	delete Info->nextPkg;
	Info->timeoutSrc->stopTimer();
	genericState *ret = (genericState*) new ST_Idle;
	ret->setLastEvent(ERRO);
	return ret;
}

genericState * ST_ReceiveAck::on_Timeout(EV_Timeout * ev, usefulInfo *Info)
{	
	Info->networkSrc->networkInterface->sendPackage(Info->nextPkg);		
	Info->timeoutSrc->startTimer();	//Reinicia el timer
	return nullptr;
}

genericState * ST_ReceiveAck::on_ConnectionFailed(EV_ConnectionFailed * ev, usefulInfo *Info)
{
	delete Info->nextPkg;
	genericState *ret = (genericState *) new ST_Idle();
	ret->setLastEvent(CONNECTION_FAIL);
	return ret;
}

genericState * ST_ReceiveAck::on_LastData(EV_LastData * ev, usefulInfo *Info)
{
	delete Info->nextPkg;
	Info->nextPkg = new Data(*ev->dataPkg);
	Info->networkSrc->expectedBlockNum++;	//Incremento el numero de bloque
	Info->networkInterface->sendPackage(Info->nextPkg);
	Info->timeoutSrc->startTimer();	//Reinicio el timer
	genericState *ret = (genericState*) new ST_ReceiveLastAck;
	return ret;
}

//ST_ReceiveLastAck
genericState * ST_ReceiveLastAck::on_Ack(EV_Ack * ev, usefulInfo *Info)
{	
	Info->fileInterface->closeFile();
	delete Info->nextPkg;	//Como se finalizo el envio, se cierra el archivo.
	Info->timeoutSrc->stopTimer();
	Info->userSrc->terminal->fileSendEnd(Info->userSrc->getFileToTransfer());	//Se muestra en pantalla que el envio fue exitoso
	genericState *ret = (genericState*) new ST_Idle;
	return ret;
}

genericState * ST_ReceiveLastAck::on_Error(EV_Error * ev, usefulInfo *Info)
{
	Info->fileInterface->closeFile();
	delete Info->nextPkg;
	Info->timeoutSrc->stopTimer();
	genericState *ret = (genericState*) new ST_Idle;
	ret->setLastEvent(ERRO);
	return ret;;
}

genericState * ST_ReceiveLastAck::on_Timeout(EV_Timeout * ev, usefulInfo *Info)
{
	Info->networkInterface->sendPackage(Info->nextPkg);	
	Info->timeoutSrc->startTimer();	//Reinicia el timer
	return nullptr;
}

genericState * ST_ReceiveLastAck::on_ConnectionFailed(EV_ConnectionFailed * ev, usefulInfo *Info)
{
	Info->fileInterface->closeFile();
	delete Info->nextPkg;
	Info->timeoutSrc->stopTimer();
	genericState *ret = (genericState *) new ST_Idle();
	ret->setLastEvent(CONNECTION_FAIL);
	return ret;
}

//ST_ReceiveFirstData
genericState * ST_ReceiveFirstData::on_Data(EV_Data * ev, usefulInfo *Info)
{
	Info->fileInterface->openFile(Info->userSrc->getFileToTransfer().c_str(), WRITE);	//abro el archivo.
	Info->fileInterface->saveData(ev->dataPkg->package);	//Escribo el bloque de data en el archivo
	Info->networkSrc->expectedBlockNum++;	//Incremento el blockNumber
	Info->nextPkg = new Acknowledge(Info->networkSrc->expectedBlockNum);	//Construyo el Ack a enviar.
	Info->networkInterface->sendPackage(Info->nextPkg);	//Envio el Ack
	Info->timeoutSrc->startTimer();	//Reseteo timer.
	genericState * ret = (genericState *) new ST_ReceiveData();
	return ret;
}

genericState * ST_ReceiveFirstData::on_Error(EV_Error * ev, usefulInfo *Info)
{
	Info->fileInterface->closeFile();
	delete Info->nextPkg;
	Info->timeoutSrc->stopTimer();
	genericState *ret = (genericState*) new ST_Idle;
	ret->setLastEvent(ERRO);
	return ret;
}

genericState * ST_ReceiveFirstData::on_Timeout(EV_Timeout * ev, usefulInfo *Info)
{
	Info->networkInterface->sendPackage(Info->nextPkg);	
	Info->timeoutSrc->startTimer();	//Reinicia el timer
	return nullptr;
}

genericState * ST_ReceiveFirstData::on_ConnectionFailed(EV_ConnectionFailed * ev, usefulInfo *Info)
{
	delete Info->nextPkg;
	genericState *ret = (genericState *) new ST_Idle();
	ret->setLastEvent(CONNECTION_FAIL);
	return ret;
}

//ST_ReceiveData
genericState * ST_ReceiveData::on_Data(EV_Data * ev, usefulInfo *Info)
{
	Info->fileInterface->saveData(ev->dataPkg->package);	//Escribo el bloque de data en el archivo
	Info->networkSrc->expectedBlockNum++;
	delete Info->nextPkg;	//Elimino el paquete anterior
	Info->nextPkg = new Acknowledge(Info->networkSrc->expectedBlockNum);	//Construyo el Ack a enviar
	Info->networkInterface->sendPackage(Info->nextPkg);
	Info->timeoutSrc->startTimer();
	return nullptr;
}

genericState * ST_ReceiveData::on_Error(EV_Error * ev, usefulInfo *Info)
{
	Info->fileInterface->closeFile();
	delete Info->nextPkg;
	Info->timeoutSrc->stopTimer();
	genericState *ret = (genericState*) new ST_Idle;
	ret->setLastEvent(ERRO);
	return ret;
}

genericState * ST_ReceiveData::on_Timeout(EV_Timeout * ev, usefulInfo *Info)
{
	Info->networkInterface->sendPackage(Info->nextPkg);
	Info->timeoutSrc->startTimer();	//Reinicia el timer
	return nullptr;
}

genericState * ST_ReceiveData::on_ConnectionFailed(EV_ConnectionFailed * ev, usefulInfo *Info)
{
	delete Info->nextPkg;
	genericState *ret = (genericState *) new ST_Idle();
	ret->setLastEvent(CONNECTION_FAIL);
	return ret;
}

genericState * ST_ReceiveData::on_LastData(EV_LastData * ev, usefulInfo *Info)
{
	Info->fileInterface->saveData(ev->dataPkg->package);	//Escribo el bloque de data en el archivo
	Info->networkSrc->expectedBlockNum++;
	delete Info->nextPkg;	//Elimino el paquete anterior
	Info->nextPkg = new Acknowledge(Info->networkSrc->expectedBlockNum);	//Construyo el Ack a enviar
	Info->networkInterface->sendPackage(Info->nextPkg);	//Envio el ultimo paquete de data
	Info->fileInterface->closeFile();	//Cierro el archivo
	Info->timeoutSrc->startTimer();	//Reinicio el timer
	genericState * ret = (genericState *) new ST_ReceiveLastAck();
	return ret;
}