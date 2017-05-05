#ifndef USEFULINFO_H
#define USEFULINFO_H

#include "eventSources.h"

class usefulInfo
{
public:
	usefulInfo(std::string serverAddres) { this->serverAddress = serverAddres; }

	NetworkEventSource* network;	//TODO: ponerle getters quizas? quedaria mas prolijo o es al pedo??
	UserEventSource* user;
	TimeoutEventSource* timeout;
	SoftwareEventSource* software;

	genericEvent* eventBuffer;	//TODO: implementar un buffer posta

private:
	std::string serverAddress;
};

#endif // !USEFULINFO_H