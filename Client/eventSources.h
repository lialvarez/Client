#ifndef EVENTSOURCES_H
#define EVENTSOURCES_H

#include <string>
#include <vector>
#include <fstream>
#include <boost\asio.hpp>
#include "Packages.h"
#include "Screen.h"
#include "Networking.h"
#include "FileSystem.h"
#include "genericEventSource.h"
#include <boost\asio\deadline_timer.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <ctime>

#define ONE_MINUTE		60
#define MAX_TIMEOUTS	5

class NetworkEventSource : public genericEventSource
{
public:
	NetworkEventSource(Networking *_networkInterface);
	bool isThereEvent();
	genericEvent* insertEvent();
	void setServerIP(std::string _serverIP);
	std::string getServerIP();
	Networking *networkInterface;
	unsigned int expectedBlockNum;
	std::vector<char> data;	//Se almacena la data en caso de recibir DATA
	std::string errorMsg;
	errorCodes errorCode;
	unsigned int blockNumber;
private:

	std::string serverIP;
};

class UserEventSource : public genericEventSource
{
public:
	UserEventSource(Screen *Terminal);
	bool isThereEvent();
	genericEvent* insertEvent();
	std::string getFileToTransfer();	//getter del nombre del archivo a transferir
	std::string getOrigFileToTransfer();
	std::string getCommand();	//getter del commando ingresado
	Screen *terminal;
private:
	std::string origFileToTransfer;
	std::string fileToTransfer;
	std::vector<char> buffer;
	std::string command;

	int inputChar;
	char ** args;

};

class TimeoutEventSource : public genericEventSource
{
public:
	TimeoutEventSource();
	bool isThereEvent();
	void startTimer();
	void stopTimer();
	genericEvent* insertEvent();
private:
	clock_t tInicial;
	unsigned int timeoutsCount;
	bool timeout;	//Si está en true se cumplió el tiempo.
	bool timerRunning;
};

class SoftwareEventSource : public genericEventSource
{
public:
	SoftwareEventSource();
	bool isThereEvent();
	bool ev;
	genericEvent* insertEvent();
	FileSystem *fileInterface;
};

#endif // !EVENTSOURCES_H