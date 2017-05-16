#ifndef EVENTS_H
#define EVENTS_H

#include "Packages.h"
#include "Networking.h"	//Para el tipo de dato _BYTE
#include "genericEvent.h"

//USER EVENTS

class EV_Put :public genericEvent
{
public:
	EV_Put() { ev = PUT; }
private:
};

class EV_Get : public genericEvent
{
public:
	EV_Get() { ev = GET; }
private:
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

class EV_EmptyCommand : public genericEvent
{
public:
	EV_EmptyCommand() { ev = EMPTY_COMMAND; }
private:

};

class EV_FileError : public genericEvent
{
public:
	EV_FileError() { ev = FILE_ERROR; }
private:
};

class EV_InvalidCommand : public genericEvent
{
public:
	EV_InvalidCommand() { ev = INVALID; };
private:

};

//NETWORK EVENTS

class EV_Data : public genericEvent
{
public:
	EV_Data() { ev = DATA; };
private:
};

class EV_Ack : public genericEvent
{
public:
	EV_Ack() { ev = ACK; };
private:
};

class EV_Error : public genericEvent
{
public:
	EV_Error() { ev = ERRO; };
private:
};

//SOFTWARE EVENTS

class EV_LastData : public genericEvent
{
public:
	EV_LastData() { ev = LAST_DATA; };
private:

};

//TIMEOUT EVENT

class EV_Timeout : public genericEvent
{
public:
	EV_Timeout() { ev = TIMEOUT; };

private:

};

#endif // !EVENTS_H

