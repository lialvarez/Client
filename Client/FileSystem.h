#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <vector>
#include <fstream>
#include <istream>
#include <ostream>

#define SAVE_DIR	"Download/"

typedef enum { READ, WRITE }openMode;

class FileSystem
{
public:
	void closeFile();
	void openFile(std::string fileName, openMode mode);
	void saveData(std::vector<char> data);
	std::vector<char> readData();
	bool lastData;
private:
	std::streamsize dataSize;
	std::fstream fileStream;
};


#endif // !FILESYSTEM_H
