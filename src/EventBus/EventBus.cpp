#include "EventBus.h"
#include "../Logger/Logger.h"

EventBus::EventBus()
{
	Logger::Log("EventBus constructor called!");
}

EventBus::~EventBus()
{
	Logger::Log("EventBus destructor called!");
}

void EventBus::Reset()
{
	Subscribers.clear();
}
