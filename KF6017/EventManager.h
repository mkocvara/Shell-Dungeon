#pragma once
#include "Event.h"
#include <list>
#include <memory>

class ObjectManager;

class EventManager
{
public:
	EventManager(std::shared_ptr<ObjectManager> pObjectManager);
	~EventManager();

	void DispatchEvent(const Event& rEvent) const;

private:
	std::weak_ptr<ObjectManager> mpObjectManager;
};

