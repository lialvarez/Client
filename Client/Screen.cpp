#include "Screen.h"
#include <fstream>
#include <sstream>

void initTerminal()
{
	terminalWindow = initscr();
	resetTerminal();
}

void resetTerminal()
{
	clear();
	move(0, 0);
	printw("Client>");
	refresh();
}

void setCommandLine()
{
	move(terminalWindow->_cury + 2, 0);
	printw("Client>");
	refresh();
}

void outputHelp()
{
	std::fstream helpFile;
	helpFile.open(HELP_FILE);
	std::string line;
	clear();
	if (helpFile.is_open())
	{
		while (std::getline(helpFile, line))
		{
			printw(line.c_str());
			move(terminalWindow->_cury + 1, 0);
		}
	}
	else
	{
		printw("Error al abrir archivo Help.txt");
	}
	setCommandLine();
}

void fileErrorMsg(std::string fileName)
{
	move(terminalWindow->_cury + 1, 0);
	printw("No se pudo encontrar el siguiente archivo:");
	printw(fileName.c_str());
	setCommandLine();
}