#ifndef PACKAGES_H
#define PACKAGES_H
#include <string>
#include <vector>

typedef char MYBYTE;

typedef enum { RRQ_OP = 1, WRQ_OP, DATA_OP, ACK_OP, ERROR_OP }opCodes;
typedef enum { NOT_DEFINED = 1, FILE_NOT_FOUND, FILE_ALREADY_EXISTS }errorCodes;

class genericPackage
{
public:
	std::vector<char> package;
	virtual void setPackage() = 0;
	opCodes code;
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

private:
	std::string mode;
	std::string fileName;
};

class Data :public genericPackage
{
public:
	Data(std::vector<char> _data, unsigned int _blockNumber);
	void setPackage();

private:
	std::vector<char> data;
	unsigned int packageLength;
	unsigned int blockNumber;
};

class Acknowledge :public genericPackage
{
public:
	Acknowledge(unsigned int _blockNumber) :blockNumber(_blockNumber) { code = ACK_OP; }
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
