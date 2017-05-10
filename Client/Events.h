#ifndef EVENTS_H
#define EVENTS_H

#include "Networking.h"	//Para el tipo de dato _BYTE
#include "genericEvent.h"

//USER EVENTS

class EV_Put :public genericEvent
{
public:
	EV_Put(std::string selectedFile) { ev = PUT; this->selectedFile = selectedFile; }
private:
};

class EV_Get : public genericEvent
{
public:
	EV_Get(std::string selectedFile) { ev = GET; this->selectedFile = selectedFile; };
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
	EV_FileError(std::string slectedFile) { ev = FILE_ERROR; this->selectedFile = selectedFile; }
private:
};

class EV_InvalidCommand : public genericEvent
{
public:
	EV_InvalidCommand(std::string command) :invalidCommand(command) { ev = INVALID; };
	std::string getInvalidCommand() { return invalidCommand; };
private:
	std::string invalidCommand;
};

//NETWORK EVENTS

class EV_Data : public genericEvent
{
public:
	EV_Data(std::string _data): data(_data) { ev = DATA; };
	//TODO: funciones de cargar el buffer y de obetener la info
private:
	std::string data;
	unsigned int blockNumber;	//Guardar el numero de bloque aca.
	_BYTE* dataBuffer;
};

class EV_Ack : public genericEvent
{
public:
	EV_Ack(unsigned int _blockNumber) :blockNumber(_blockNumber) { ev = ACK; };
private:
	unsigned int blockNumber;
};

class EV_Error : public genericEvent
{
public:
	EV_Error(unsigned int _errorCode, std::string _errorMsg) :errorCode(_errorCode), errorMsg(_errorMsg) { ev = ERRO; };

private:
	unsigned int errorCode;
	std::string errorMsg;
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
	EV_ConnectionFailed() { ev = CONNECTION_FAIL; };

private:

};

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

