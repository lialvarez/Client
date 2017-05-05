#include "genericEvent.h"

genericEvent::genericEvent()
{
	ev = NO_EV;
}

eventCode genericEvent::getEventType()
{
	return ev;
}

void genericEvent::setSelectedFile(std::string selectedFile)
{
	this->selectedFile = selectedFile;
}

std::string genericEvent::getSelectedFile()
{
	return selectedFile;
}