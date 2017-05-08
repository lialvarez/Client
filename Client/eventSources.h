#ifndef EVENTSOURCES_H
#define EVENTSOURCES_H

#include <string>
#include <vector>
#include <fstream>
#include <boost\asio.hpp>
#include "Screen.h"
#include "Networking.h"
#include "genericEventSource.h"
#include <boost\asio\deadline_timer.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>


class NetworkEventSource : public genericEventSource
{
public:
	NetworkEventSource(Networking *_networkInterface) :networkInterface(_networkInterface){}
	bool isThereEvent();
	void setServerIP(std::string _serverIP);
	std::string getServerIP();
private:
	Networking *networkInterface;
	std::string serverIP;
};

class UserEventSource : public genericEventSource
{
public:
	UserEventSource(Screen *Terminal);
	bool isThereEvent();
	genericEvent* insertEvent();
	std::string getFileToTransfer();	//getter del nombre del archivo a transferir
	std::string getCommand();	//getter del commando ingresado
	Screen *terminal;
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

	TimeoutEventSource() :timer(ioForTimer, boost::posix_time::minutes(1)), timeout(false) {} //constructor, setea el timer
	bool isThereEvent();
	void startTimer();
	void stopTimer();
	genericEvent* insertEvent();
	
private:
	boost::asio::io_service ioForTimer;	
	boost::asio::deadline_timer timer;
	bool timeout;
	void setTimeout(const boost::system::error_code& /*e*/);
	void handler(const boost::system::error_code&, boost::asio::deadline_timer* t); //prototipo del handler
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