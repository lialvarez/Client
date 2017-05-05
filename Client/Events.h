#ifndef EVENTS_H
#define EVENTS_H

#include "genericEvent.h"

typedef char BYTE;

//USER EVENTS

class EV_Put :public genericEvent
{
public:
	EV_Put() { ev = PUT; };

private:
	std::string fileToTransfer;
};

class EV_Get : public genericEvent
{
public:
	EV_Get() { ev = GET; };
private:
	std::string fileToTransfer;
};

class EV_Quit : public genericEvent
{
public:
	EV_Quit() { ev = QUIT; };
private:

};

class EV_Help : public genericEvent
{
public:
	EV_Help() { ev = HELP; };
private:

};

class EV_Clear : public genericEvent
{
public:
	EV_Clear() { ev = CLEAR; };
private:

};

class EV_InvalidCommand : public genericEvent
{
public:
	EV_InvalidCommand() { ev = INVALID; };
	~EV_InvalidCommand();

private:

};

//NETWORK EVENTS

class EV_Data : public genericEvent
{
public:
	EV_Data() { ev = DATA; };
	//TODO: funciones de cargar el buffer y de obetener la info

private:
	BYTE* dataBuffer;
};

class EV_Ack : public genericEvent
{
public:
	EV_Ack() { ev = ACK; };

private:
	unsigned int blockNumber;
};

class EV_Error : public genericEvent
{
public:
	EV_Error() { ev = ERROR; };

private:

};

class EV_WRQ : public genericEvent
{
public:
	EV_WRQ() { ev = WRQ; };

private:
	std::string fileToTransfer;
}; 

class EV_RRQ : public genericEvent
{
public:
	EV_RRQ() { ev = RRQ; };

private:
	std::string fileToTransfer;

};

//SOFTWARE EVENTS

class EV_ConnectionFailed : public genericEvent
{
public:
	EV_ConnectionFailed();

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

