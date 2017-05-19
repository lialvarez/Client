#include "Screen.h"

#include <fstream>
#include <sstream>

Screen::Screen() :lastLine(0)
{
	terminalWindow = initscr();
	nodelay(terminalWindow, TRUE);
	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	wbkgd(terminalWindow, COLOR_PAIR(1));
	attron(A_BOLD);
	scrollok(terminalWindow, TRUE);
}

void Screen::initTerminal()
{
	clear();
	move(terminalWindow->_maxy - 3, 0);
	for (int i = 0; i < terminalWindow->_maxx; i++)
	{
		printw("-");
		move(terminalWindow->_cury, i);
	}
	setCommandLine();
	lastLine = 0;
}

void Screen::setCommandLine()
{
	move(terminalWindow->_maxy - 2, 0);
	clrtoeol();
	color_set(2, NULL);
	printw("Client>");
	inputPos = terminalWindow->_curx;
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
		putClear("INSTRUCCIONES DE USO");
		//printw("INSTRUCCIONES DE USO:");
		//move(terminalWindow->_cury + 2, 0);
		color_set(1, NULL);
		while (std::getline(helpFile, line))
		{
			putNext(line);
			//printw(line.c_str());
			//move(terminalWindow->_cury + 1, 0);
		}
		refresh();
	}
	else
	{
		putNext("Error al abrir archivo Help.txt");
		//printw("Error al abrir archivo Help.txt");
	}
	//setCommandLine();
}

void Screen::putClear(std::string inputText)
{
	initTerminal();
	move(0, 0);
	printw(inputText.c_str());
	lastLine = terminalWindow->_cury;
	move(terminalWindow->_maxy - 2, 7);
	refresh();
}

void Screen::putNext(std::string inputText)
{
	if (lastLine == terminalWindow->_maxy - 5)
	{
		shiftLinesUp();
		move(lastLine, 0);
	}
	else
	{
		move(lastLine + 1, 0);
	}

	printw(inputText.c_str());
	lastLine = terminalWindow->_cury;
	move(terminalWindow->_maxy - 2, inputPos);
	refresh();
}


void Screen::fileErrorMsg(std::string fileName)
{
	std::string aux = "No se pudo encontrar el archivo '" + fileName + "'";
	putNext(aux);
	/*printw("\nNo se pudo encontrar el archivo '"); printw(fileName.c_str()); printw("'\n");*/
	refresh();
	setCommandLine();
}

void Screen::outputInvalid(std::string command)
{
	std::string aux = "Comando '" + command + "' no encontrado";
	putNext(aux);
	putNext("Para ver la pagina de ayuda ingrese HELP\n");
	//printw("\nComando '"); printw(command.c_str()); printw("' no encontrado\n");
	//printw("Para ver la pagina de ayuda ingrese HELP\n");
	//refresh();
	setCommandLine();
}

void Screen::fileSendEnd(std::string fileName)
{
	std::string aux = "Archivo '" + fileName + "' enviado existosamente";
	putNext(aux);
	//printw("\nArchivo '"); printw(fileName.c_str()); printw("' enviado exitosamente\n");
	//refresh();
	//setCommandLine();
}

void Screen::fileREceiveEnd(std::string fileName)
{
	std::string aux = "Archivo '" + fileName + "' recibido exitosamente";
	putNext(aux);
	//printw("\nArchivo '"); printw(fileName.c_str()); printw("' recibido exitosamente\n");
	//refresh();
	//setCommandLine();
}

void Screen::errorMsg(errorCodes code, std::string errorMsg)
{
	std::string aux;
	switch (code)
	{
	case NOT_DEFINED:
		putNext("Eerror: Error indefinido");
		aux = "Mensaje de error: " + errorMsg;
		putNext(aux);
		//printw("\nError: Not defined error\nError message:"); printw(errorMsg.c_str());
		//refresh();
		break;
	case FILE_NOT_FOUND:
		putNext("Error: No se encontro el archivo solicitado");
		//printw("\nError: The requested file was not found\n");
		//refresh();
		break;
	case FILE_ALREADY_EXISTS:
		putNext("Error: El archivo ya existe");
		//printw("\nError: Selected file already exists\n");
		//refresh();
		break;
	default:
		break;
	}
}

void Screen::shiftLinesUp()
{
	char *aux = new char[terminalWindow->_maxx];
	for (int i = 0; i < lastLine; i++)
	{
		mvinstr(i + 1, 0, aux);
		clrtoeol();
		mvprintw(i, 0, aux);
		refresh();
	}
}