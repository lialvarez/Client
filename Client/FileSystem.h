#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <fstream>

#define SAVE_DIR	"Download/"

class FileSystem
{
public:
	FileSystem();
	~FileSystem();

	bool saveData(std::string data);
	std::string readData();
	
private:
	
	FILE *filePointer;

};

#endif // !FILESYSTEM_H
