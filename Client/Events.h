#ifndef EVENTS_H
#define EVENTS_H

#include "genericEvent.h"

typedef char _BYTE;

//USER EVENTS

class EV_Put :public genericEvent
{
public:
	EV_Put(Screen *terminal) { ev = PUT; this->terminal = terminal; };
	Screen *terminal;
	std::string getSelectedFile() { return fileToTransfer; }	//getter del nombre del archivo
	void setSelectedFile(std::string selectedFile) { fileToTransfer = selectedFile; }	//setter del nombre del archivo
private:
	std::string fileToTransfer;
};

class EV_Get : public genericEvent
{
public:
	EV_Get(Screen *terminal) { ev = GET; };
	Screen *terminal;
	std::string getSelectedFile() { return fileToTransfer; }	//getter del nombre del archivo
	void setSelectedFile(std::string selectedFile) { fileToTransfer = selectedFile; }	//setter del nombre del archivo
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
	EV_Help(Screen *terminal) { ev = HELP; this->terminal = terminal; };
	Screen *terminal;
private:

};

class EV_Clear : public genericEvent
{
public:
	EV_Clear(Screen *terminal) { ev = CLEAR; this->terminal = terminal; };
	Screen *terminal;
private:

};

class EV_EmptyCommand : public genericEvent
{
public:
	EV_EmptyCommand(Screen *terminal) { ev = EMPTY_COMMAND; this->terminal = terminal; }
	Screen *terminal;
private:

};

class EV_FileError : public genericEvent
{
public:
	EV_FileError(Screen *terminal) { ev = FILE_ERROR; this->terminal = terminal; };
	Screen *terminal;
	std::string getFileNotFound() { return fileNotFound; }
	void setFileNotFound(std::string fileNotFound) { this->fileNotFound = fileNotFound; }
private:
	std::string fileNotFound;
};

class EV_InvalidCommand : public genericEvent
{
public:
	EV_InvalidCommand(Screen *terminal) { ev = INVALID; this->terminal = terminal; };
	Screen *terminal;
	std::string getInvalidCommand() { return invalidCommand; };
	void setInvalidCommand(std::string invalidCommand) { this->invalidCommand = invalidCommand; }
private:
	std::string invalidCommand;
};

//NETWORK EVENTS

class EV_Data : public genericEvent
{
public:
	EV_Data() { ev = DATA; };
	//TODO: funciones de cargar el buffer y de obetener la info

private:
	unsigned int blockNumber;	//Guardar el numero de bloque aca.
	_BYTE* dataBuffer;
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

