#ifndef GENERICEVENTSOURCE_H
#define GENERICEVENTSOURCE_H

#include "genericEvent.h"

class genericEventSource
{
public:
	virtual bool isThereEvent() = 0;	//forzar a hacer una funcion distinta de busqueda de eventos para cada fuente
	virtual genericEvent* insertEvent() = 0;

protected:
	eventCode evCode;
	genericEvent* event;
};

#endif // !GENERICEVENTSOURCE_H