#include "FileSystem.h"

void FileSystem::openFile(std::string fileName, openMode mode)
{
	if (mode == READ)
	{
		fileStream.open(fileName, std::fstream::in | std::fstream::binary);
	}
	else if(mode == WRITE)
	{
		fileStream.open(fileName, std::fstream::out | std::fstream::binary);
	}
}

bool FileSystem::saveData(std::string data)
{
	fileStream.write(data.c_str(), data.length());
}

std::string FileSystem::readData()
{
//	std::string ret;
//	dataSize = 4 + fileStream.readsome(ret.c_str, 512);
//	return ret;

	char* aux = nullptr;
	fileStream.read(aux, 512);
	std::string ret(aux);
	return ret;
}

void FileSystem::closeFile()
{
	if (fileStream.is_open())
	{
		fileStream.close();
	}
}