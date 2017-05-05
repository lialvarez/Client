#include "genericEventSource.h"
#include "eventSources.h"
#include "Events.h"

genericEvent * UserEventSource::insertEvent()
{
	genericEvent * ret;
	switch (evCode)
	{
	case NO_EV:
		ret = (genericEvent *) new EV_NoEvent;
		break;
	case PUT:
		ret = (genericEvent *) new EV_SendWRQ;
		break;
	case GET:
		ret = (genericEvent *) new EV_SendRRQ;
		break;
	case HELP:
		ret = (genericEvent *) new EV_HelpRequest;
		break;
	default:
		break;
	}
}