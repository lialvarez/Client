#ifndef STATES_H
#define STATES_H

#include "genericState.h"

class ST_Idle : public genericState
{
public:

	genericState* on_Get(genericEvent *ev);
	genericState* on_Put(genericEvent *ev);
	//Chequear estos dos:
	genericState* on_SendError(genericEvent *ev);
	genericState* on_ReceiveError(genericEvent *ev);

	genericState* on_FileError(genericEvent *ev);
	genericState* on_InvalidCommand(genericEvent *ev);
	genericState* on_CloseClient(genericEvent* ev);
	genericState* on_HelpRequest(genericEvent *ev);
	genericState* on_ClearTerminal(genericEvent *ev);
	genericState* on_EmptyCommand(genericEvent *ev);

};

class ST_ReceiveFirstAck :public genericState
{
public:

	genericState* on_Ack(genericEvent* ev);
	genericState* on_Error(genericEvent* ev);
	genericState* on_Timeout(genericEvent* ev);
	genericState* on_ConnectionFailed(genericEvent* ev);

private:
};

class ST_ReceiveAck : public genericState
{
public:

	genericState* on_Ack(genericEvent* ev);
	genericState* on_Error(genericEvent* ev);
	genericState* on_Timeout(genericEvent* ev);
	genericState* on_ConnectionFailed(genericEvent* ev);
	genericState* on_LastData(genericEvent* ev);

private:
};

class ST_ReceiveLastAck : public genericState
{
public:

	genericState* on_Ack(genericEvent* ev);
	genericState* on_Error(genericEvent* ev);
	genericState* on_Timeout(genericEvent* ev);
	genericState* on_ConnectionFailed(genericEvent* ev);

private:

};

class ST_ReceiveFirstData : public genericState
{
public:

	genericState* on_Data(genericEvent* ev);
	genericState* on_Error(genericEvent* ev);
	genericState* on_Timeout(genericEvent* ev);
	genericState* on_ConnectionFailed(genericEvent* ev);
private:
};

class ST_ReceiveData : public genericState
{
public:

	genericState* on_Data(genericEvent* ev);
	genericState* on_Error(genericEvent* ev);
	genericState* on_Timeout(genericEvent* ev);
	genericState* on_ConnectionFailed(genericEvent* ev);
	genericState* on_LastData(genericEvent* ev);

private:
};

#endif // !STATES_H



