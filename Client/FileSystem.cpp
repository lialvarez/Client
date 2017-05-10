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
	char *aux;
	fileStream.read(aux, 512);
	std::string ret(aux);
	return ret;
}

void FileSystem::closeFile()
{
	fileStream.close();
}