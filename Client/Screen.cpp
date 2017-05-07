#include "Screen.h"
#include <fstream>
#include <sstream>

Screen::Screen()
{
	terminalWindow = initscr();
	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	wbkgd(terminalWindow, COLOR_PAIR(1));
	attron(A_BOLD);
	scrollok(terminalWindow, TRUE);
}

void Screen::initTerminal()
{
	resetTerminal();
}

void Screen::resetTerminal()
{
	clear();
	move(0, 0);
	color_set(2, NULL);
	printw("Client>");
	color_set(1, NULL);
	refresh();
}

void Screen::setCommandLine()
{
	color_set(2, NULL);
	printw("\nClient>");
	color_set(1, NULL);
	refresh();
}

void Screen::outputHelp()
{
	std::fstream helpFile;
	helpFile.open(HELP_FILE);
	std::string line;
	clear();
	if (helpFile.is_open())
	{
		color_set(2, NULL);
		printw("INSTRUCCIONES DE USO:");
		move(terminalWindow->_cury + 2, 0);
		color_set(1, NULL);
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

void Screen::fileErrorMsg(std::string fileName)
{
	printw("\nNo se pudo encontrar el archivo '"); printw(fileName.c_str()); printw("'\n");
	refresh();
	setCommandLine();
}

void Screen::outputInvalid(std::string command)
{
	printw("\nComando '"); printw(command.c_str()); printw("' no encontrado\n");
	printw("Para ver la pagina de ayuda ingrese HELP\n");
	refresh();
	setCommandLine();
}