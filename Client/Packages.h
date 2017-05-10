#ifndef PACKAGES_H
#define PACKAGES_H
#include <string>
typedef char MYBYTE;
typedef enum{RRQ_OP = 1, WRQ_OP, DATA_OP, ACK_OP, ERROR_OP}opCode;

class genericPackage
{
public:
	MYBYTE *package;
	virtual void setPackage() {}
	opCode code;
};

class ReadRequest :public genericPackage
{
public:
	ReadRequest(std::string _fileName, std::string _mode) :fileName(_fileName), mode(_mode) { code = RRQ_OP; }
	void setPackage();
	std::string mode;
	std::string fileName;

};

class WriteRequest :public genericPackage
{
public:
	WriteRequest(std::string _fileName, std::string _mode) :fileName(_fileName), mode(_mode) { code = WRQ_OP; }
	void setPackage();
	std::string mode;
	std::string fileName;
};

class Data :public genericPackage
{
public:
	Data(std::string _data, unsigned int _blockNumber = 1) :data(_data), blockNumber(_blockNumber) { code = DATA_OP; }
	std::string data;
	unsigned int blockNumber;
	void setPackage();
};

class Acknowledgment :public genericPackage
{
public:
	Acknowledgment(unsigned int _blockNumber) :blockNumber(_blockNumber) { code = ACK_OP; }
	unsigned int blockNumber;
	void setPackage();
};

class Error :public genericPackage
{
public:
	Error(unsigned int _errorCode, std::string _errorMsg) :errorCode(_errorCode), errorMsg(_errorMsg) { code = ERROR_OP; }
	unsigned int errorCode;
	std::string errorMsg;
	void setPackage();
};

#endif // !PACKAGES_H
