#ifndef GENERICFSM_H
#define GENERICFSM

#include "genericState.h"
#include "genericEvent.h"

class genericFSM
{
public:
	genericFSM();	//Crea un estado al que apunta currentState
	~genericFSM();	//Destruye el estado al que apunta currentState

	void dispatch(genericEvent * ev);	//llama a la funcion del estado al que apunta current state que repsonde al evento recibido
	genericState * getCurrentState();

protected:
	genericState * currentState;	//se usa para apuntar a los estados, que son objetos de una clase que hereda la clase genericState
};

#endif // !GENERICFSM_H

