#pragma once
#include "Service.h"
#include "Event.h"
#include <list>
#include <memory>

class ServiceManager;

class EventManager : public Service
{
	typedef Service Super;

public:
	EventManager(std::shared_ptr<ServiceManager> pServiceManager);
	~EventManager();

	void DispatchEvent(const Event& rEvent) const;

private:
	std::weak_ptr<ServiceManager> mpServiceManager;
};

