#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <fstream>

#define SAVE_DIR	"Download/"

typedef enum { READ, WRITE }openMode;

class FileSystem
{
public:

	void closeFile();
	void openFile(std::string fileName, openMode mode);
	bool saveData(std::string data);
	std::string readData();
	
private:
	
	std::fstream fileStream;
	std::streamsize dataSize;
};

#endif // !FILESYSTEM_H
