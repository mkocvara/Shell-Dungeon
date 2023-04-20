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

