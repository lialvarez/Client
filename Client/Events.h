#ifndef EVENTS_H
#define EVENTS_H

#include "genericEvent.h"

typedef char BYTE;

//USER EVENTS

class EV_Put :public genericEvent
{
public:
	EV_Put() { ev = PUT; };
	~EV_Put();

private:

};

class EV_Get : public genericEvent
{
public:
	EV_Get() { ev = GET; };
	~EV_Get();

private:

};

class EV_Quit : public genericEvent
{
public:
	EV_Quit() { ev = QUIT; };
	~EV_Quit();

private:

};

class EV_Help : public genericEvent
{
public:
	EV_Help() { ev = HELP; };
	~EV_Help();

private:

};

class EV_Clear : public genericEvent
{
public:
	EV_Clear() { ev = CLEAR; };
	~EV_Clear();

private:

};

//NETWORK EVENTS

class EV_Data : public genericEvent
{
public:
	EV_Data() { ev = DATA; };
	~EV_Data();

	//TODO: funciones de cargar el buffer y de obetener la info

private:
	BYTE* dataBuffer;
};

class EV_Ack : public genericEvent
{
public:
	EV_Ack() { ev = ACK; };
	~EV_Ack();

private:

};

class EV_Error : public genericEvent
{
public:
	EV_Error() { ev = ERROR; };
	~EV_Error();

private:

};

class EV_WRQ : public genericEvent
{
public:
	EV_WRQ() { ev = WRQ; };
	~EV_WRQ();

private:

}; 

class EV_RRQ : public genericEvent
{
public:
	EV_RRQ() { ev = RRQ; };
	~EV_RRQ();

private:

};

//SOFTWARE EVENTS

class EV_InvalidCommand : public genericEvent
{
public:
	EV_InvalidCommand() { ev = INVALID; };
	~EV_InvalidCommand();

private:

};

class EV_ConnectionFailed : public genericEvent
{
public:
	EV_ConnectionFailed();
	~EV_ConnectionFailed();

private:

};

class EV_FileError : public genericEvent
{
public:
	EV_FileError() { ev = FILE_ERROR; };
	~EV_FileError();

private:

};

class EV_LastData : public genericEvent
{
public:
	EV_LastData() { ev = LAST_DATA; };
	~EV_LastData();

private:

};

//TIMEOUT EVENT

class EV_Timeout : public genericEvent
{
public:
	EV_Timeout() { ev = TIMEOUT; };
	~EV_Timeout();

private:

};

#endif // !EVENTS_H

