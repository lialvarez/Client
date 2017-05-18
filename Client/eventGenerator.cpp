#include "eventGenerator.h"

void eventGenerator::generateEvent()
{
	if (I->userSrc->isThereEvent())
	{
		buffer.push_back(I->userSrc->insertEvent());
	}

	if (I->softwareSrc->isThereEvent())
	{
		buffer.push_back(I->softwareSrc->insertEvent());
	}

	if (I->networkSrc->isThereEvent())
	{
		buffer.push_back(I->networkSrc->insertEvent());
	}

	if (I->timeoutSrc->isThereEvent())
	{
		buffer.push_back(I->timeoutSrc->insertEvent());
	}

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