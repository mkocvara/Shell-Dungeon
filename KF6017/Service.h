/*
* Service.h
* 19002175
*
* Base class for services. Enables keeping a list of services for event dispatching purposes.
*/

#pragma once

struct Event;

class Service
{
public:
	~Service();

	virtual void HandleEvent(const Event& rEvent);

protected:
	Service(); // abstract
};

