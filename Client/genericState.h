#ifndef GENERICSTATE_H
#define GENERICSTATE_H

#include "genericEvent.h"

class genericState
{
public:

	//OJO ESTOS SON LOS ESTADOS DE LA FSM DEL TP ANTERIOR!!!

	// Estas funciones responden a los eventos recibidos en el estado actual. Por default, se toma que los eventos recibidos son eventos inesperados, lo que sse indica devolviendo nullptr. Al ser virtuales, las clases de los estados especificos que heredan a genericState pueden redefinir que respuesta tomar ante los eventos que no son inesperados para ese estado en especifico.
	virtual genericState* on_SendWRQ(genericEvent* ev) { return nullptr; }
	virtual genericState* on_SendRRQ(genericEvent* ev) { return nullptr; }
	virtual genericState* on_ReceiveWRQAck(genericEvent* ev) { return nullptr; }
	virtual genericState* on_SendData(genericEvent* ev) { return nullptr; }
	virtual genericState* on_ReceiveData(genericEvent* ev) { return nullptr; }
	virtual genericState* on_SendAck(genericEvent* ev) { return nullptr; }
	virtual genericState* on_ReceiveAck(genericEvent* ev) { return nullptr; }
	virtual genericState* on_SendLastData(genericEvent* ev) { return nullptr; }
	virtual genericState* on_ReceiveLastData(genericEvent* ev) { return nullptr; }
	virtual genericState* on_timeout(genericEvent* ev) { return nullptr; }
	virtual genericState* on_SendError(genericEvent* ev) { return nullptr; }
	virtual genericState* on_ReceiveError(genericEvent* ev) { return nullptr; }
	virtual genericState* on_CloseClient(genericEvent* ev) { return nullptr; }
	virtual genericState* on_HelpRequest(genericEvent *ev) { return nullptr; }
	virtual genericState* on_ClearTerminal(genericEvent* ev) { return nullptr; }
	virtual genericState* on_InvalidCommand(genericEvent* ev) { return nullptr; }
	virtual genericState* on_FileError(genericEvent* ev) { return nullptr; }
	virtual genericState* on_LastData(genericEvent* ev) { return nullptr; }//ES VALIDO TANTO PARA SEND COMO RECEIVE??
	virtual genericState* on_EmptyCommand(genericEvent *ev) { return nullptr; }

	void setLastEvent(eventCode lastEvent) { this->lastEvent = lastEvent; }   //setter
	eventCode getLastEvent() { return lastEvent; }    //getter

protected:
	eventCode lastEvent;
};

#endif // !GENERICSTATE_H


