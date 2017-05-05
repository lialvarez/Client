#ifndef GENERICEVENT_H
#define GENERICEVENT_H

#include <string>

typedef enum { NO_EV, PUT, GET, QUIT, HELP, CLEAR, EMPTY_COMMAND, INVALID, FILE_ERROR, DATA, ACK, WRQ, RRQ, SEND_ERROR ,RECEIVE_ERROR, LAST_DATA, TIMEOUT } eventCode;

class genericEvent
{
public:
	genericEvent();	//por default, se setea el tipo de evento en NO_EV. Si otra clase hereda genericEvent, puede cambiar el constructor para setear el evento como el que corresponda
	eventCode getEventType();	//getter del tipo de evento

	std::string getSelectedFile();	//getter del nombre del archivo
	void setSelectedFile(std::string selectedFile);	//setter del nombre del archivo

protected:
	std::string selectedFile;
	eventCode ev;
};


#endif // !GENERICEVENT_H