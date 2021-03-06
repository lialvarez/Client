#include "States.h"
#include "Screen.h"
#include <fstream>
#include <sstream>

//ST_Idle

genericState *ST_Idle::on_Put(genericEvent *ev, usefulInfo *Info)
{
	std::string aux = "WRQ enviado. Archivo solicitado: '" + Info->userSrc->getFileToTransfer() + "'";
	Info->userInterface->putNext(aux);
	Info->networkSrc->expectedBlockNum = 0;	//Setea el block number que deberia tener el proximo ACK
	Info->fileInterface->openFile(Info->userSrc->getOrigFileToTransfer(), READ);	//Abre el archivo en el modo indicado
	Info->nextPkg = new WriteRequest(Info->userSrc->getFileToTransfer(), "octet");	//Copia el ultimo paquete enviado
	Info->networkInterface->sendPackage(Info->nextPkg);	//Funcion que envia el WRQ con el nombre del archivo
	Info->timeoutSrc->startTimer();	//Inicia el timer
	genericState *ret = (genericState *) new ST_ReceiveFirstAck();
	return ret;
}

genericState *ST_Idle::on_Get(genericEvent *ev, usefulInfo *Info)
{
	std::string aux = "RRQ enviado. Archivo solicitado: '" + Info->userSrc->getFileToTransfer() + "'";
	Info->userInterface->putNext(aux);
	Info->nextPkg = new ReadRequest(Info->userSrc->getFileToTransfer(), "octet");
	Info->networkSrc->networkInterface->sendPackage(Info->nextPkg);	//Funcion que envia el RRQ con el nombre del archivo
	Info->networkSrc->expectedBlockNum = 1;
	Info->timeoutSrc->startTimer();	//Inicia el timer
	genericState *ret = (genericState *) new ST_ReceiveFirstData;
	return ret;
}

genericState *ST_Idle::on_HelpRequest(genericEvent *ev, usefulInfo *Info)
{
	Info->userInterface->outputHelp();	//Rutina que muestra el mensaje de ayuda en pantalla
	genericState *ret = (genericState*) new ST_Idle;
	return ret;
}

genericState *ST_Idle::on_CloseClient(genericEvent* ev, usefulInfo *Info)
{
	delete Info->nextPkg;
	Info->fileInterface->closeFile();
	setLastEvent(QUIT);
	return nullptr;
}

genericState *ST_Idle::on_ClearTerminal(genericEvent *ev, usefulInfo *Info)
{
	Info->userInterface->initTerminal();	//Rutina de accion que borra el terminal
	genericState *ret = (genericState*) new ST_Idle;
	return ret;
}

genericState *ST_Idle::on_FileError(genericEvent *ev, usefulInfo *Info)
{
	Info->userInterface->fileErrorMsg(Info->userSrc->getFileToTransfer());	//Rutina de accion que indica que se ingreso archivo invalido
	genericState *ret = (genericState*) new ST_Idle();
	return ret;
}

genericState *ST_Idle::on_InvalidCommand(genericEvent *ev, usefulInfo *Info)
{
	Info->userInterface->outputInvalid(Info->userSrc->getCommand());	//Rutina que indica por pantalla que se ingreso comando invalido
	genericState * ret = (genericState *) new ST_Idle();
	return ret;
}

genericState *ST_Idle::on_EmptyCommand(genericEvent *ev, usefulInfo *Info)
{
	genericState *ret = (genericState *) new ST_Idle();
	Info->userInterface->setCommandLine();
	return ret;
}


//ST_ReceiveFirstAck
genericState * ST_ReceiveFirstAck::on_Ack(genericEvent * ev, usefulInfo *Info)
{
	std::string auxText = "Ackowledge #" + std::to_string(Info->networkSrc->blockNumber) + " recivido.";
	Info->userInterface->putNext(auxText);

	delete Info->nextPkg;
	Info->nextPkg =(Data *) new Data(Info->fileInterface->readData(), ++Info->networkSrc->expectedBlockNum);	//Armo el paquete a enviar
	Info->networkInterface->sendPackage(Info->nextPkg);
	
	auxText = "Data block #" + std::to_string(Info->networkSrc->expectedBlockNum) + " enviado";
	Info->userInterface->putNext(auxText);

	Info->timeoutSrc->startTimer();		//Resetear Timer
	return (genericState*) new ST_ReceiveAck;
}

genericState * ST_ReceiveFirstAck::on_Error(genericEvent * ev, usefulInfo *Info)
{
	Info->fileInterface->closeFile();	//Cerrar el archivo
	delete Info->nextPkg;	//Elimino el ultimo paquete enviado
	Info->timeoutSrc->stopTimer();	//Detener el timer
	Info->userInterface->errorMsg(Info->networkSrc->errorCode, Info->networkSrc->errorMsg);
	Info->userInterface->setCommandLine();
	genericState *ret = (genericState *) new ST_Idle;
	ret->setLastEvent(ERRO);	//Setear el ultimo evento en Error para resetear la FSM
	return ret;
}

genericState * ST_ReceiveFirstAck::on_Timeout(genericEvent * ev, usefulInfo *Info)
{
	Info->userInterface->putNext("Timeout, reenviado WRQ");
	Info->networkInterface->sendPackage(Info->nextPkg);
	Info->timeoutSrc->startTimer();
	return nullptr;
}

genericState *ST_ReceiveFirstAck::on_CloseClient(genericEvent* ev, usefulInfo *Info)
{
	delete Info->nextPkg;
	Info->fileInterface->closeFile();
	setLastEvent(QUIT);
	return nullptr;
}

//ST_ReceiveAck
genericState * ST_ReceiveAck::on_Ack(genericEvent * ev, usefulInfo *Info)
{
	std::string auxText = "Ackowledge #" + std::to_string(Info->networkSrc->blockNumber) + " recibido.";
	Info->userInterface->putNext(auxText);

	delete Info->nextPkg;
	Info->nextPkg = new Data(Info->fileInterface->readData(), ++Info->networkSrc->expectedBlockNum);
	Info->networkSrc->networkInterface->sendPackage(Info->nextPkg);	//Se envia el siguiente bloque

	auxText = "Data block #" + std::to_string(Info->networkSrc->expectedBlockNum) + " enviado";
	Info->userInterface->putNext(auxText);

	Info->timeoutSrc->startTimer();	//Inicia el timer
	return nullptr;
}

genericState * ST_ReceiveAck::on_Error(genericEvent * ev, usefulInfo *Info)
{
	delete Info->nextPkg;
	Info->timeoutSrc->stopTimer();
	genericState *ret = (genericState*) new ST_Idle;
	ret->setLastEvent(ERRO);
	return ret;
}

genericState * ST_ReceiveAck::on_Timeout(genericEvent * ev, usefulInfo *Info)
{	
	Info->userInterface->putNext("Timeout, reenviado data package");
	Info->networkInterface->sendPackage(Info->nextPkg);	
	Info->timeoutSrc->startTimer();	//Reinicia el timer
	return nullptr;
}

genericState * ST_ReceiveAck::on_LastData(genericEvent * ev, usefulInfo *Info)
{
	Info->userInterface->putNext("Last data package enviado");
	Info->timeoutSrc->startTimer();
	genericState *ret = (genericState*) new ST_ReceiveLastAck;
	return ret;
}

genericState * ST_ReceiveAck::on_CloseClient(genericEvent* ev, usefulInfo *Info)
{
	delete Info->nextPkg;
	Info->fileInterface->closeFile();
	setLastEvent(QUIT);
	return nullptr;
}

//ST_ReceiveLastAck
genericState * ST_ReceiveLastAck::on_Ack(genericEvent * ev, usefulInfo *Info)
{	

	std::string auxText = "Ackowledge #" + std::to_string(Info->networkSrc->blockNumber) + " recibido.";
	Info->userInterface->putNext(auxText);
	Info->userInterface->putNext("Transmision completada");

	Info->fileInterface->closeFile();
	delete Info->nextPkg;	//Como se finalizo el envio, se cierra el archivo.

	Info->timeoutSrc->stopTimer();
	genericState *ret = (genericState*) new ST_Idle;
	return ret;
}

genericState * ST_ReceiveLastAck::on_Error(genericEvent * ev, usefulInfo *Info)
{
	Info->fileInterface->closeFile();
	delete Info->nextPkg;
	Info->timeoutSrc->stopTimer();
	genericState *ret = (genericState*) new ST_Idle;
	ret->setLastEvent(ERRO);
	return ret;;
}

genericState * ST_ReceiveLastAck::on_Timeout(genericEvent * ev, usefulInfo *Info)
{
	Info->userInterface->putNext("Timeout, reenviado data package");
	Info->networkInterface->sendPackage(Info->nextPkg);	
	Info->timeoutSrc->startTimer();	//Reinicia el timer
	return nullptr;
}

genericState * ST_ReceiveLastAck::on_CloseClient(genericEvent* ev, usefulInfo *Info)
{
	delete Info->nextPkg;
	Info->fileInterface->closeFile();
	setLastEvent(QUIT);
	return nullptr;
}

//ST_ReceiveFirstData
genericState * ST_ReceiveFirstData::on_Data(genericEvent * ev, usefulInfo *Info)
{
	std::string auxText = "Data block #" + std::to_string(Info->networkSrc->blockNumber) + " recibido.";
	Info->userInterface->putNext(auxText);
	Info->fileInterface->openFile(Info->userSrc->getFileToTransfer().c_str(), WRITE);	//abro el archivo.
	Info->fileInterface->saveData(Info->networkSrc->data);	//Escribo el bloque de data en el archivo
	Info->nextPkg = new Acknowledge(Info->networkSrc->expectedBlockNum++);	//Construyo el Ack a enviar.
	Info->networkInterface->sendPackage(Info->nextPkg);	//Envio el Ack
	Info->timeoutSrc->startTimer();	//Reseteo timer.
	genericState * ret = (genericState *) new ST_ReceiveData();
	return ret;
}

genericState * ST_ReceiveFirstData::on_Error(genericEvent * ev, usefulInfo *Info)
{
	Info->userInterface->errorMsg(Info->networkSrc->errorCode, Info->networkSrc->errorMsg);
	Info->fileInterface->closeFile();
	delete Info->nextPkg;
	Info->timeoutSrc->stopTimer();
	Info->userInterface->setCommandLine();
	genericState *ret = (genericState*) new ST_Idle;
	ret->setLastEvent(ERRO);
	return ret;
}

genericState * ST_ReceiveFirstData::on_Timeout(genericEvent * ev, usefulInfo *Info)
{
	Info->userInterface->putNext("Timeout, reenviado acknowledge");
	Info->networkInterface->sendPackage(Info->nextPkg);
	Info->timeoutSrc->startTimer();	//Reinicia el timer
	return nullptr;
}

genericState * ST_ReceiveFirstData::on_LastData(genericEvent *ev, usefulInfo *Info)
{
	std::string auxText = "Data block #" + std::to_string(Info->networkSrc->blockNumber) + " recibido.";
	Info->userInterface->putNext(auxText);
	Info->userInterface->putNext("Last data package recibido");

	Info->fileInterface->openFile(Info->userSrc->getFileToTransfer().c_str(), WRITE);	//abro el archivo.
	Info->fileInterface->saveData(Info->networkSrc->data);	//Escribo el bloque de data en el archivo
	delete Info->nextPkg;	//Elimino el paquete anterior
	Info->nextPkg = new Acknowledge(Info->networkSrc->expectedBlockNum);	//Construyo el Ack a enviar
	Info->networkInterface->sendPackage(Info->nextPkg);	//Envio el ultimo paquete de data
	Info->fileInterface->closeFile();	//Cierro el archivo
	Info->timeoutSrc->startTimer();	//Reinicio el timer

	Info->userInterface->fileSendEnd(Info->userSrc->getFileToTransfer());

	genericState * ret = (genericState *) new ST_Idle;
	return ret;
}

genericState * ST_ReceiveFirstData::on_CloseClient(genericEvent* ev, usefulInfo *Info)
{
	delete Info->nextPkg;
	Info->fileInterface->closeFile();
	setLastEvent(QUIT);
	return nullptr;
}
//ST_ReceiveData
genericState * ST_ReceiveData::on_Data(genericEvent * ev, usefulInfo *Info)
{
	std::string auxText = "Data block #" + std::to_string(Info->networkSrc->blockNumber) + " recibido.";
	Info->userInterface->putNext(auxText);

	Info->fileInterface->saveData(Info->networkSrc->data);	//Escribo el bloque de data en el archivo
	delete Info->nextPkg;	//Elimino el paquete anterior
	Info->nextPkg = new Acknowledge(Info->networkSrc->expectedBlockNum++);	//Construyo el Ack a enviar
	Info->networkInterface->sendPackage(Info->nextPkg);
	Info->timeoutSrc->startTimer();
	return nullptr;
}

genericState * ST_ReceiveData::on_Error(genericEvent * ev, usefulInfo *Info)
{
	Info->fileInterface->closeFile();
	delete Info->nextPkg;
	Info->timeoutSrc->stopTimer();
	genericState *ret = (genericState*) new ST_Idle;
	ret->setLastEvent(ERRO);
	return ret;
}

genericState * ST_ReceiveData::on_Timeout(genericEvent * ev, usefulInfo *Info)
{
	Info->userInterface->putNext("Timeout, reenviado acknowledge");
	Info->networkInterface->sendPackage(Info->nextPkg);
	Info->timeoutSrc->startTimer();	//Reinicia el timer
	return nullptr;
}

genericState * ST_ReceiveData::on_LastData(genericEvent * ev, usefulInfo *Info)
{
	std::string auxText = "Data block #" + std::to_string(Info->networkSrc->blockNumber) + " recibido.";
	Info->userInterface->putNext(auxText);
	Info->userInterface->putNext("Last data package recibido");

	Info->fileInterface->saveData(Info->networkSrc->data);	//Escribo el bloque de data en el archivo
	delete Info->nextPkg;	//Elimino el paquete anterior
	Info->nextPkg = new Acknowledge(Info->networkSrc->expectedBlockNum);	//Construyo el Ack a enviar
	Info->networkInterface->sendPackage(Info->nextPkg);	//Envio el ultimo paquete de data
	Info->fileInterface->closeFile();	//Cierro el archivo
	Info->timeoutSrc->startTimer();	//Reinicio el timer

	Info->userInterface->fileSendEnd(Info->userSrc->getFileToTransfer());

	genericState * ret = (genericState *) new ST_Idle;
	return ret;
}

genericState * ST_ReceiveData::on_CloseClient(genericEvent* ev, usefulInfo *Info)
{
	delete Info->nextPkg;
	Info->fileInterface->closeFile();
	setLastEvent(QUIT);
	return nullptr;
}