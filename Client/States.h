#ifndef STATES_H
#define STATES_H

#include "genericState.h"

class ST_Idle:public genericState
{
public:
	//genericState* on_Get(genericEvent *ev);
	//genericState* on_Put(genericEvent *ev);
	//genericState* on_SendError(genericEvent *ev);
	//genericState* on_ReceiveError(genericEvent *ev);
	genericState* on_FileError(genericEvent *ev);
	genericState* on_InvalidCommand(genericEvent *ev);
	genericState* on_CloseClient(genericEvent* ev);
	genericState* on_HelpRequest(genericEvent *ev);
	genericState* on_ClearTerminal(genericEvent *ev);
	genericState* on_EmptyCommand(genericEvent *ev);
	genericState* on_Quit(genericEvent *ev);

};

class ST_ReceiveFirstAck :public genericState
{
public:
	genericState* on_Timeout(genericEvent* ev);
	genericState* on_ReceiveAck(genericEvent* ev);
	genericState* on_ReceiveError(genericEvent* ev);


private:

};

class ST_ReceiveAck :public genericState
{
public:
	ST_ReceiveAck();
	~ST_ReceiveAck();

private:

};

class ST_SendingLastData :public genericState
{
public:
	ST_SendingLastData();
	~ST_SendingLastData();

private:

};

class ST_ReceiveLastAck :public genericState
{
public:
	ST_ReceiveLastAck();
	~ST_ReceiveLastAck();

private:

};

class ST_ReceiveFirstData :public genericState
{
public:
	ST_ReceiveFirstData();
	~ST_ReceiveFirstData();

private:

};

class ST_ReceiveData :public genericState
{
public:
	ST_ReceiveData();
	~ST_ReceiveData();

private:

};


#endif // !STATES_H



