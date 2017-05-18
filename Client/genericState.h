#ifndef GENERICSTATE_H
#define GENERICSTATE_H

#include "usefulInfo.h"
#include "genericEvent.h"

class genericState
{
public:
	// Estas funciones responden a los eventos recibidos en el estado actual. Por default, se toma que los eventos recibidos son eventos inesperados, lo que sse indica devolviendo nullptr. Al ser virtuales, las clases de los estados especificos que heredan a genericState pueden redefinir que respuesta tomar ante los eventos que no son inesperados para ese estado en especifico.
	virtual genericState* on_NoEv(genericEvent* ev) { return nullptr; }
	virtual genericState* on_Put(genericEvent* ev, usefulInfo *Info) { return nullptr; }
	virtual genericState* on_Get(genericEvent* ev, usefulInfo *Info) { return nullptr; }
	virtual genericState* on_CloseClient(genericEvent* ev) {
		setLastEvent(QUIT);
		return nullptr;
	}
	virtual genericState* on_HelpRequest(genericEvent* ev, usefulInfo *Info) { return nullptr; }
	virtual genericState* on_ClearTerminal(genericEvent *ev, usefulInfo *Info) { return nullptr; }
	virtual genericState* on_EmptyCommand(genericEvent* ev, usefulInfo *Info) { return nullptr; }
	virtual genericState* on_FileError(genericEvent* ev, usefulInfo *Info) { return nullptr; }
	virtual genericState* on_InvalidCommand(genericEvent* ev, usefulInfo *Info) { return nullptr; }
	virtual genericState* on_Data(genericEvent* ev, usefulInfo *Info) { return nullptr; }
	virtual genericState* on_Ack(genericEvent* ev, usefulInfo *Info) { return nullptr; }
	virtual genericState* on_Error(genericEvent* ev, usefulInfo *Info) { return nullptr; }
	virtual genericState* on_WRQ(genericEvent* ev, usefulInfo *Info) { return nullptr; }
	virtual genericState* on_RRQ(genericEvent *ev, usefulInfo *Info) { return nullptr; }
	virtual genericState* on_ConnectionFailed(genericEvent* ev, usefulInfo *Info) { return nullptr; }
	virtual genericState* on_LastData(genericEvent* ev, usefulInfo *Info) { return nullptr; }
	virtual genericState* on_Timeout(genericEvent* ev, usefulInfo *Info) { return nullptr; }


	void setLastEvent(eventCode lastEvent) { this->lastEvent = lastEvent; }   //setter
	eventCode getLastEvent() { return lastEvent; }    //getter de last event
	std::string getSelectedFile() { return fileToTransfer; }
	void setFileToTransfer(std::string selectedFile) { this->fileToTransfer = selectedFile; }

protected:
	eventCode lastEvent;
	std::string fileToTransfer;
};

#endif // !GENERICSTATE_H


