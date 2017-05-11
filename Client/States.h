#ifndef STATES_H
#define STATES_H

#include "Events.h"
#include "eventSources.h"
#include "genericState.h"

class ST_Idle : public genericState
{
public:
	genericState* on_Get(EV_Get *ev, usefulInfo *Info);
	genericState* on_Put(EV_Put *ev, usefulInfo *Info);
	
	//Chequear estos dos:
	//genericState* on_SendError(genericEvent *ev);
	//genericState* on_ReceiveError(genericEvent *ev);

	genericState* on_FileError(EV_FileError *ev, usefulInfo *Info);
	genericState* on_InvalidCommand(EV_InvalidCommand *ev, usefulInfo *Info);
	genericState* on_CloseClient(EV_Quit* ev);
	genericState* on_HelpRequest(EV_Help *ev, usefulInfo *Info);
	genericState* on_ClearTerminal(EV_Clear *ev, usefulInfo *Info);
	genericState* on_EmptyCommand(EV_EmptyCommand *ev, usefulInfo *Info);

};

class ST_ReceiveFirstAck :public genericState
{
public:
	genericState* on_Ack(EV_Ack* ev, usefulInfo *Info);
	genericState* on_Error(EV_Error* ev, usefulInfo *Info);
	genericState* on_Timeout(EV_Timeout* ev, usefulInfo *Info);
	genericState* on_ConnectionFailed(EV_ConnectionFailed* ev, usefulInfo *Info);

	std::string getSelectedFile() { return fileToTransfer; }
	void setFileToTransfer(std::string selectedFile) { this->fileToTransfer = selectedFile; }

private:
	std::string fileToTransfer;
};

class ST_ReceiveAck : public genericState
{
public:

	genericState* on_Ack(EV_Ack* ev, usefulInfo *Info);
	genericState* on_Error(EV_Error* ev);
	genericState* on_Timeout(EV_Timeout* ev, usefulInfo *Info);
	genericState* on_ConnectionFailed(EV_ConnectionFailed* ev);
	genericState* on_LastData(genericEvent* ev, usefulInfo *Info);

private:
};

class ST_ReceiveLastAck : public genericState
{
public:

	genericState* on_Ack(EV_Ack* ev, usefulInfo *Info);
	genericState* on_Error(EV_Error* ev);
	genericState* on_Timeout(EV_Timeout* ev, usefulInfo *Info);
	genericState* on_ConnectionFailed(EV_ConnectionFailed* ev);

private:

};

class ST_ReceiveFirstData : public genericState
{
public:

	genericState* on_Data(EV_Data* ev);
	genericState* on_Error(EV_Error* ev);
	genericState* on_Timeout(EV_Timeout* ev);
	genericState* on_ConnectionFailed(EV_ConnectionFailed* ev);

	std::string getSelectedFile() { return fileToTransfer; }
	void setFileToTransfer(std::string selectedFile) { this->fileToTransfer = selectedFile; }
private:
	std::string fileToTransfer;
};

class ST_ReceiveData : public genericState
{
public:

	genericState* on_Data(EV_Data* ev);
	genericState* on_Error(EV_Error* ev);
	genericState* on_Timeout(EV_Timeout* ev);
	genericState* on_ConnectionFailed(EV_ConnectionFailed* ev);
	genericState* on_LastData(EV_LastData* ev);

private:
};

#endif // !STATES_H



