/////////////////////////////////////////////////////////////////////////////////////////////	
//
// Instituto Tecnologico de Buenos Aires						18/05/2017
//
// Algoritmos y Estructuras de Datos - Trabajo Práctico 7
//
//
// Alvarez, Lisandro						 ////////////////////////////////////////////////
// Stewart, María Luz						//	   Networking + Event Driven Programming
// Muller, Malena						   //////////////////////////////////////////////////
//
// CLIENT
//
// Este programa comienza conectandose a un servidor. El usuario puede elegir que hacer con
// el servidor. Ya sea enviarle un archivo o pedirle al servidor que le envie un archivo.
// Se implemento el protocolo TFTP, cuyas especificaciones se encuentran en la documentacion
// de RFC1350. https://tools.ietf.org/html/rfc1350
//
// Ingresar "help" para información sobre la interaccion del usuario con el programa.
// Tener en cuenta que el ingreso de un path de ubicacion de un archivo es case sensitive.
//
/////////////////////////////////////////////////////////////////////////////////////////////

#include "parseCmdLine.h"
#include "usefulInfo.h"
#include "eventGenerator.h"
#include "genericFSM.h"
#include "eventSources.h"
#include "Screen.h"
#include "Networking.h"
#include <string>

typedef struct
{
	std::string serverAddress;
	unsigned int argCount;
}userData_t;


int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cout << "Por favor ingrese la direccion IP del servidor a traves de la linea de comando.\n Ejemplo: 'Client.exe 192.168.0.50'" << std::endl;
		std::cout << "Presione una tecla para salir." << std::endl;
		getchar();
		return 0;
	}
	std::string IPAddress = argv[1];
	Screen Terminal;
	FileSystem fileSystem;
	Networking Network(IPAddress);
	NetworkEventSource networkSource(&Network);
	UserEventSource userSource(&Terminal);
	TimeoutEventSource timeoutSource;
	SoftwareEventSource Software;
	usefulInfo Info(IPAddress, &userSource, &timeoutSource, &networkSource, &fileSystem, &Software);	//Se crea la instancia de usefulInfo
	eventGenerator evGen(&Info);	//creo la instancia del generador de eventos
	genericEvent *ev;
	genericFSM FSM;

	

	unsigned int connectAttempt = 0;
	do
	{
		Network.startConnection();

		do //Si hay conexion, entro en la FSM
		{
			evGen.generateEvent();	//Chequea todas las fuentes de estados
			ev = evGen.getNextEvent();	//Carga el evento en ev

			if (ev != nullptr)
			{
				FSM.dispatch(ev, &Info);
			}

		} while (FSM.getCurrentState()->getLastEvent() != QUIT);
		evGen.clearBuffer();

	} while (FSM.getCurrentState()->getLastEvent() != QUIT);
	
}
