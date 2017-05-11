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
	eventGenerator(usefulInfo* _I) :buffer(16), I(_I){}
	void generateEvent();
	genericEvent* getNextEvent();
	void clearBuffer();

protected:

	boost::circular_buffer<genericEvent *> buffer;
	usefulInfo* I;
};

#endif // !EVENTGENERATOR_H



