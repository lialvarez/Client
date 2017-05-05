#include "eventGenerator.h"

eventGenerator::eventGenerator(usefulInfo* _I):buffer(16)
{
	I = _I;
}

void eventGenerator::generateEvent()
{
	//Buffer FILO

	if (I->software->isThereEvent())
	{
		buffer.push_back(I->software->insertEvent());
	}

	if (I->network->isThereEvent())
	{
		buffer.push_back(I->network->insertEvent());
	}

	if (I->timeout->isThereEvent())
	{
		buffer.push_back(I->timeout->insertEvent());
	}

	if (I->user->isThereEvent())
	{
		buffer.push_back(I->user->insertEvent());
	}		
}
genericEvent * eventGenerator::getNextEvent()
{
	genericEvent * ret;
	if (buffer.size() != 0)
	{
		ret = buffer.front();	//carga en ret el primer elemento del buffer
		buffer.pop_front();	//elimina el ultimo elemento del buffer
	}
	else
	{
		ret = nullptr;	//Si no hay eventos en el buffer devuelve nullptr
	}
	return ret;
}
