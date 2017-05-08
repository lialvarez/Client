#ifndef USEFULINFO_H
#define USEFULINFO_H

#include "eventSources.h"

class usefulInfo
{
public:
	//Hay que agrgarle los punteros a las otras fuentes al constructor
	//y pasar el constructor a un .cpp
	usefulInfo(std::string _serverAddres, UserEventSource *UsEvSrc, TimeoutEventSource *TOEvSrc) :user(UsEvSrc), timeout(TOEvSrc), serverAddress(_serverAddres) {}

	NetworkEventSource* network;	//TODO: ponerle getters quizas? quedaria mas prolijo o es al pedo??
	UserEventSource* user;
	TimeoutEventSource* timeout;
	SoftwareEventSource* software;

	genericEvent* eventBuffer;	//TODO: implementar un buffer posta

private:
	std::string serverAddress;

};

#endif // !USEFULINFO_H