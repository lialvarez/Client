#ifndef SCREEN_H
#define SCREEN_H

#include "curses.h"
#include <string>

#define ENTER			10
#define BACKSPACE		8 
#define ASCII_START		31
#define ASCII_END		127

#define HELP_FILE		"resources/Help.txt"

class Screen
{
public:
	Screen();
	
	void initTerminal();
	void newLine();
	void outputHelp();
	void outputInvalid(std::string command);
	void setCommandLine();
	void resetTerminal();
	void fileErrorMsg(std::string fileName);
	void fileSendEnd(std::string fileName);
	void fileREceiveEnd(std::string fileName);

	WINDOW * terminalWindow;
private:
	

};
#endif // !SCREEN_H
