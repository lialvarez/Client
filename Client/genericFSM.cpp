#include "genericFSM.h"
#include "States.h"

genericFSM::genericFSM()
{
	currentState = (genericState *) new ST_Idle();
}

genericFSM::~genericFSM()
{
	delete currentState;
}

genericState * genericFSM::getCurrentState()
{
	return currentState;
}

void genericFSM::dispatch(genericEvent *ev, usefulInfo *Info)
{
	genericState *newState = nullptr;
	{
		switch (ev->getEventType())	
		{
		case NO_EV:
			newState = currentState->on_NoEv(ev);
			break;
		case PUT:
			newState = currentState->on_Put(ev, Info);
			break;
		case GET:
			newState = currentState->on_Get(ev, Info);
			break;
		case QUIT:
			newState = currentState->on_CloseClient(ev);
			break;
		case HELP:
			newState = currentState->on_HelpRequest(ev, Info);
			break;
		case CLEAR:
			newState = currentState->on_ClearTerminal(ev, Info);
			break;
		case INVALID:
			newState = currentState->on_InvalidCommand(ev, Info);
			break;
		case EMPTY_COMMAND:
			newState = currentState->on_EmptyCommand(ev, Info);
			break;
		case FILE_ERROR:
			newState = currentState->on_FileError(ev, Info);
			break;
		case DATA:
			newState = currentState->on_Data(ev, Info);
			break;
		case ACK:
			newState = currentState->on_Ack(ev, Info);
			break;
		case WRQ:
			newState = currentState->on_WRQ(ev, Info);
			break;	
		case RRQ:
			newState = currentState->on_RRQ(ev, Info);
			break;
		case ERRO:
			newState = currentState->on_Error(ev, Info);
			break;
		case LAST_DATA:
			newState = currentState->on_LastData(ev, Info);
			Info->fileInterface->lastData = false;
			break;
		case TIMEOUT:
			newState = currentState->on_Timeout(ev, Info);
			break;
		case CONNECTION_FAIL:
			newState = currentState->on_ConnectionFailed(ev, Info);
			break;
		default:
			break;
		}
		if (newState != nullptr)
		{
			delete currentState;
			currentState = newState;
		}
	}
}

