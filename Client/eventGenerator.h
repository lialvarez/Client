#ifndef EVENTGENERATOR_H
#define EVENTGENERATOR_H

#include "usefulInfo.h"
#include "eventSources.h"
#include "genericEvent.h"
#include <vector>

class eventGenerator
{
public:
	eventGenerator(usefulInfo* _I);
	~eventGenerator();
	void generateEvent();
	genericEvent* getNextEvent();

protected:

	std::vector<genericEvent *> buffer;
	usefulInfo* I;
};

#endif // !EVENTGENERATOR_H



