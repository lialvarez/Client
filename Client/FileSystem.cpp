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
	if (fileStream.fail())
	{

	}
}

void FileSystem::saveData(std::vector<char> data)
{
	fileStream.write(&data[0], data.size());
}

std::vector<char> FileSystem::readData()
{	
	std::vector<char> ret;
	fileStream.read(&ret[0], 512);
	return ret;
}

void FileSystem::closeFile()
{
	if (fileStream.is_open())
	{
		fileStream.close();
	}
}