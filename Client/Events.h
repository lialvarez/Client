#ifndef EVENTS_H
#define EVENTS_H

#include "genericEvent.h"

class EV_EmptyCommand
{
public:
	EV_EmptyCommand();
	~EV_EmptyCommand();

private:

};

class EV_SendWRQ :public genericEvent
{
public:
	EV_SendWRQ();
	~EV_SendWRQ();

private:

};

class EV_SendRRQ :public genericEvent
{
public:
	EV_SendRRQ();
	~EV_SendRRQ();

private:

};

class EV_SendData
{
public:
	EV_SendData();
	~EV_SendData();

private:
	unsigned int sentPckCounter;	//lleva la cuenta de paquetes enviados

};


class EV_SendLastData
{
public:
	EV_SendLastData();
	~EV_SendLastData();

private:


};

class EV_CloseClient :public genericEvent
{
public:
	EV_CloseClient();
	~EV_CloseClient();

private:

};

class EV_HelpRequest :public genericEvent
{
public:
	EV_HelpRequest();
	~EV_HelpRequest();

private:

};

class EV_ClearScreen :public genericEvent
{
public:
	EV_ClearScreen();
	~EV_ClearScreen();

private:

};

class EV_ReceiveData :public genericEvent
{
public:
	EV_ReceiveData();
	~EV_ReceiveData();

private:
	unsigned int 
};

class EV_ReceiveLastData :public genericEvent
{
public:
	EV_ReceiveLastData();
	~EV_ReceiveLastData();

private:

};


class EV_ReceiveAck :public genericEvent
{
public:
	EV_ReceiveAck();
	~EV_ReceiveAck();

private:

};

class EV_ReceiveError :public genericEvent
{
public:
	EV_ReceiveError();
	~EV_ReceiveError();

private:

};

class EV_InvalidCommand :public genericEvent
{
public:
	EV_InvalidCommand();
	~EV_InvalidCommand();

private:

};

class EV_ConnectionFailed :public genericEvent
{
public:
	EV_ConnectionFailed();
	~EV_ConnectionFailed();

private:

};

class EV_Timeout :public genericEvent
{
public:
	EV_Timeout();
	~EV_Timeout();

private:

};

class EV_FileError :public genericEvent
{
public:
	EV_FileError();
	~EV_FileError();

private:

};


#endif // !EVENTS_H

