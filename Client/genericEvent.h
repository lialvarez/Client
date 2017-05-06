#ifndef GENERICEVENT_H
#define GENERICEVENT_H

#include <string>

typedef enum { NO_EV, PUT, GET, QUIT, HELP, CLEAR, EMPTY_COMMAND, INVALID, FILE_ERROR, ERROR, DATA, ACK, WRQ, RRQ, LAST_DATA, TIMEOUT, CONNECTION_FAIL } eventCode;

class genericEvent
{
public:
	genericEvent() { ev = NO_EV; }//por default, se setea el tipo de evento en NO_EV. Si otra clase hereda genericEvent, puede cambiar el constructor para setear el evento como el que corresponda
	eventCode getEventType() { return ev; }	//getter del tipo de evento

protected:
	std::string selectedFile;
	eventCode ev;
};


#endif // !GENERICEVENT_H