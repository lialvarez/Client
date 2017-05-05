#ifndef EVENTGENERATOR_H
#define EVENTGENERATOR_H


#include "usefulInfo.h"
#include "eventSources.h"
#include "genericEvent.h"
#include <vector>
#include <boost\circular_buffer.hpp>

class eventGenerator
{
public:
	eventGenerator(usefulInfo* _I);
	~eventGenerator();
	void generateEvent();
	genericEvent* getNextEvent();

protected:

	boost::circular_buffer<genericEvent *> buffer(16);	//Buffer circular de 16 posiciones
	usefulInfo* I;
};

#endif // !EVENTGENERATOR_H



