#include "eventGenerator.h"


eventGenerator::eventGenerator(usefulInfo* _I)
{
	I = _I;
}
eventGenerator::~eventGenerator() {};

void eventGenerator::generateEvent()
{
	//Buffer FILO

	if (I->user->isThereEvent())
	{
		buffer.push_back(I->user->insertEvent());
		I->eventBuffer/*->insert(I->user->getEvent())*/;
	}

	if (I->timeout->isThereEvent())
	{
		buffer.push_back(I->timeout->insertEvent());
		I->eventBuffer/*->insert(I->timeout->getEvent())*/;
	}

	if (I->network->isThereEvent())
	{
		buffer.push_back(I->network->insertEvent());
		I->eventBuffer/*->insert(I->network->getEvent())*/;
	}

	if (I->software->isThereEvent())
	{
		buffer.push_back(I->software->insertEvent());
		I->eventBuffer/*->insert(I->software->getEvent())*/;	//TODO: todavia no esta implementado el buffer ni sus funciones
	}
		
}

genericEvent * eventGenerator::getNextEvent()
{
//	return I->eventBuffer->getNextElement();	//TODO: todavia no esta implementado el buffer ni sus funciones
	genericEvent * ret;
	if (buffer.size() != 0)
	{
		ret = buffer.back();	//carga en ret el ultimo elemento del buffer
		buffer.pop_back();	//elimina el ultimo elemento del buffer
	}
	else
	{
		ret = nullptr;
	}
	return ret;
}
