#ifndef USEFULINFO_H
#define USEFULINFO_H

#include <string>
#include "eventSources.h"
#include "FileSystem.h"

class usefulInfo
{
public:

	usefulInfo(std::string _serverAddres, UserEventSource *UsEvSrc, TimeoutEventSource *TOEvSrc, NetworkEventSource *NETEvSrc, FileSystem *FileSys, SoftwareEventSource *SoftSrc) 
		:userSrc(UsEvSrc), timeoutSrc(TOEvSrc), networkSrc(NETEvSrc), serverAddress(_serverAddres), fileInterface(FileSys), softwareSrc(SoftSrc) {
		networkInterface = networkSrc->networkInterface;
		userInterface = userSrc->terminal;
		softwareSrc->fileInterface = fileInterface;

	}

	NetworkEventSource* networkSrc;	//TODO: ponerle getters quizas? quedaria mas prolijo o es al pedo??
	UserEventSource* userSrc;
	TimeoutEventSource* timeoutSrc;
	SoftwareEventSource* softwareSrc;

	Networking *networkInterface;
	Screen  *userInterface;
	FileSystem *fileInterface;
	
	genericPackage *nextPkg;
private:
	std::string serverAddress;

};

#endif // !USEFULINFO_H