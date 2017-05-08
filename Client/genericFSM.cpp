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

void genericFSM::dispatch(genericEvent *ev)
{
	genericState *newState = nullptr;
	{
		switch (ev->getEventType())	
		{
		case NO_EV:
			newState = currentState->on_NoEv(ev);
			break;
		case PUT:
			newState = currentState->on_Put(ev);
			break;
		case GET:
			newState = currentState->on_Get(ev);
			break;
		case QUIT:
			newState = currentState->on_CloseClient(ev);
			break;
		case HELP:
			newState = currentState->on_Help(ev);
			break;
		case CLEAR:
			newState = currentState->on_ClearTerminal(ev);
			break;
		case INVALID:
			newState = currentState->on_InvalidCommand(ev);
			break;
		case EMPTY_COMMAND:
			newState = currentState->on_EmptyCommand(ev);
			break;
		case FILE_ERROR:
			newState = currentState->on_FileError(ev);
			break;
		case DATA:
			newState = currentState->on_Data(ev);
			break;
		case ACK:
			newState = currentState->on_Ack(ev);
			break;
		case WRQ:
			newState = currentState->on_WRQ(ev);
			break;	
		case RRQ:
			newState = currentState->on_RRQ(ev);
			break;
		case ERRO:
			newState = currentState->on_Error(ev);
			break;
		case LAST_DATA:
			newState = currentState->on_LastData(ev);
			break;
		case TIMEOUT:
			newState = currentState->on_Timeout(ev);
			break;
		case CONNECTION_FAIL:
			newState = currentState->on_ConnectionFailed(ev);
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

