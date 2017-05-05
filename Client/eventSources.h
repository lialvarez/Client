#ifndef EVENTSOURCES_H
#define EVENTSOURCES_H

#include <string>
#include <vector>
#include <fstream>
#include <boost\asio.hpp>
#include "genericEventSource.h"


class NetworkEventSource : public genericEventSource
{
public:
	NetworkEventSource();
	~NetworkEventSource();
	bool isThereEvent();
	void setServerIP(std::string _serverIP);
	std::string getServerIP();
private:
	std::string serverIP;
};

class UserEventSource : public genericEventSource
{
public:

	UserEventSource();
	bool isThereEvent();
	std::string getFileToTransfer();
	genericEvent* insertEvent();
private:

	std::string fileToTransfer;

	std::vector<std::string> words;	//Vector para separar los argumentos ingresados
	std::vector<char> buffer;
	std::string command;

	int inputChar;
	char ** args;

};

class TimeoutEventSource : public genericEventSource
{
public:

	TimeoutEventSource() :timer(io, boost::posix_time::minutes(1)), timeout(false) {}
	bool isThereEvent();
	void startTimer();
	void stopTimer();
	genericEvent* insertEvent();
private:
	boost::asio::io_service io;	
	boost::asio::deadline_timer timer;
	bool timeout;
	void setTimeout();
};

class SoftwareEventSource : public genericEventSource
{
public:
	SoftwareEventSource();
	~SoftwareEventSource();
	bool isThereEvent();
	//loadSoftwareEvent(ev); //TODO: agregar una funcion para cargar eventos de software
};

#endif // !EVENTSOURCES_H