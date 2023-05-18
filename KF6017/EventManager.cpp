/*
* EventManager.cpp
* 19002175
*
* Service class for dispatching events (messages).
*/

#include "EventManager.h"
#include "ServiceManager.h"
#include "ObjectManager.h"
#include "GameObject.h"

EventManager::EventManager(std::shared_ptr<ServiceManager> pServiceManager)
	: Super(), mpServiceManager(pServiceManager)
{
}

EventManager::~EventManager()
{
}


// PUBLIC

void EventManager::DispatchEvent(const Event& rEvent) const
{
	std::shared_ptr<ServiceManager> pServiceManagerLocked = mpServiceManager.lock();
	std::shared_ptr<ObjectManager> pObjectManager = pServiceManagerLocked->GetObjectManager().lock();

	std::list<std::shared_ptr<GameObject>> objects = pObjectManager->GetAllObjects();
	std::list<std::shared_ptr<Service>> services = pServiceManagerLocked->GetAllServices();
	
	for (const std::shared_ptr<GameObject>& rpGameObject : objects)
	{
		if (!rpGameObject)
			continue;

		rpGameObject->HandleEvent(rEvent);
	}

	for (const std::shared_ptr<Service>& rpService : services)
	{
		if (!rpService)
			continue;

		rpService->HandleEvent(rEvent);
	}
}