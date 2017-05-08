#ifndef USEFULINFO_H
#define USEFULINFO_H

#include <string>
#include "eventSources.h"

class usefulInfo
{
public:
	//Hay que agrgarle los punteros a las otras fuentes al constructor
	//y pasar el constructor a un .cpp
	usefulInfo(std::string _serverAddres, UserEventSource *UsEvSrc, TimeoutEventSource *TOEvSrc, NetworkEventSource *NETEvSrc) 
		:userSrc(UsEvSrc), timeoutSrc(TOEvSrc), networkSrc(NETEvSrc), serverAddress(_serverAddres) {}

	NetworkEventSource* networkSrc;	//TODO: ponerle getters quizas? quedaria mas prolijo o es al pedo??
	UserEventSource* userSrc;
	TimeoutEventSource* timeoutSrc;
	SoftwareEventSource* softwareSrc;

	genericEvent* eventBuffer;	//TODO: implementar un buffer posta

	void closeFile() { fclose(filePointer); }
	void setFilePointer(std::string fileName) { filePointer = fopen(fileName.c_str(), "rb"); }
	FILE *getFilePointer() { return filePointer; }
	unsigned int blockNumber;
private:
	FILE *filePointer;
	std::string serverAddress;

};

#endif // !USEFULINFO_H