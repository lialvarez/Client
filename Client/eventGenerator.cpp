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
	}

	if (I->timeout->isThereEvent())
	{
		buffer.push_back(I->timeout->insertEvent());
	}

	if (I->network->isThereEvent())
	{
		buffer.push_back(I->network->insertEvent());
	}

	if (I->software->isThereEvent())
	{
		buffer.push_back(I->software->insertEvent());
	}
		
}
genericEvent * eventGenerator::getNextEvent()
{
	genericEvent * ret;
	if (buffer.size() != 0)
	{
		ret = buffer.back();	//carga en ret el ultimo elemento del buffer
		buffer.pop_back();	//elimina el ultimo elemento del buffer
	}
	else
	{
		ret = nullptr;	//Si no hay eventos en el buffer devuelve nullptr
	}
	return ret;
}
