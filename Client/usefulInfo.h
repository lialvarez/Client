#ifndef USEFULINFO_H
#define USEFULINFO_H

#include "eventSources.h"

class usefulInfo
{
public:
	//Hay que agrgarle los punteros a las otras fuentes al constructor
	//y pasar el constructor a un .cpp
	usefulInfo(std::string _serverAddress, UserEventSource *UsEvSrc, TimeoutEventSource *TOEvSrc) :user(UsEvSrc), timeout(TOEvSrc), serverAddress(_serverAddress) {}

	NetworkEventSource* networkSrc;	//TODO: ponerle getters quizas? quedaria mas prolijo o es al pedo??
	UserEventSource* userSrc;
	TimeoutEventSource* timeoutSrc;
	SoftwareEventSource* softwareSrc;

	genericEvent* eventBuffer;	//TODO: implementar un buffer posta

private:
	std::string serverAddress;

};

#endif // !USEFULINFO_H