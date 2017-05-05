#ifndef SCREEN_H
#define SCREEN_H

#include "curses.h"

#define ENTER			10
#define BACKSPACE		8 
#define ASCII_START		31
#define ASCII_END		127

#define HELP_FILE		"resources/Help.txt"

void initTerminal();
void outputHelp();
void setCommandLine();
void resetTerminal();
void fileErrorMsg(std::string fileName);

WINDOW * terminalWindow;

#endif // !SCREEN_H
