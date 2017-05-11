#include "eventGenerator.h"

void eventGenerator::generateEvent()
{

	//if (I->software->isThereEvent())
	//{
	//	buffer.push_back(I->software->insertEvent());
	//}

	//if (I->network->isThereEvent())
	//{
	//	buffer.push_back(I->network->insertEvent());
	//}

	//if (I->timeout->isThereEvent())
	//{
	//	buffer.push_back(I->timeout->insertEvent());
	//}

	if (I->userSrc->isThereEvent())
	{
		buffer.push_back(I->userSrc->insertEvent());
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

void eventGenerator::clearBuffer()
{
	buffer.clear();
}