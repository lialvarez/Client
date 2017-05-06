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
	move(terminalWindow->_cury + 2, 0);
	color_set(2, NULL);
	printw("Client>");
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
	move(terminalWindow->_cury + 1, 0);
	printw("No se pudo encontrar el archivo '"); printw(fileName.c_str()); printw("'");
	move(terminalWindow->_cury + 1, 0);
	setCommandLine();
}

void Screen::newLine()
{
	if (terminalWindow->_cury > getmaxy(terminalWindow) - 2)	//Si estoy en la ultima fila de la terminal limpio la pantalla
	{
		clear();
		move(0, 0);
	}
	else
	{
		move(terminalWindow->_cury + 1, 0);	//Creo una nueva linea
	}
	color_set(2, NULL);
	printw("Client>");
	color_set(1, NULL);
	refresh();
}

void Screen::outputInvalid(std::string command)
{
	if (terminalWindow->_cury > getmaxy(terminalWindow) - 5)
	{
		clear();
		move(0, 0);
	}
	else
	{
		move(terminalWindow->_cury + 1, 0);
	}
	printw("Comando '"); printw(command.c_str()); printw("' no encontrado");
	move(terminalWindow->_cury + 1, 0);
	printw("Para ver la pagina de ayuda ingrese HELP");
	move(terminalWindow->_cury + 1, 0);
	refresh();
	newLine();
}