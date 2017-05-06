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
		case PUT:
			newState = currentState->on_SendWRQ(ev);
			break;
		case GET:
			newState = currentState->on_SendRRQ(ev);
			break;
		case HELP:
			newState = currentState->on_HelpRequest(ev);
			break;
		case CLEAR:
			newState = currentState->on_ClearTerminal(ev);
			break;
		case QUIT:
			newState = currentState->on_CloseClient(ev);
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
			newState = currentState->on_ReceiveData(ev);
			break;
		case LAST_DATA:
			newState = currentState->on_LastData(ev);
			break;
		case ACK:
			newState = currentState->on_ReceiveAck(ev);
			break;
		case TIMEOUT:
			newState = currentState->on_timeout(ev);
			break;
		case RECEIVE_ERROR:
			newState = currentState->on_ReceiveError(ev);
			break;
		case SEND_ERROR:
			newState = currentState->on_SendError(ev);
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

