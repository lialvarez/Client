#ifndef NETWORKING_H
#define NETWORKING_H

#include <string>

typedef enum { RRQ_OP, WRQ_OP, DATA_OP, ACK_OP, ERROR_OP }opCodes;

class Networking
{
public:
	Networking(std::string _serverAddres):serverAddress(_serverAddres){}

private:

	std::string serverAddress;
};
#endif // !NETWORKING_H
