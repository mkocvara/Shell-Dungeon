#include "EventManager.h"
#include "ObjectManager.h"
#include "GameObject.h"

EventManager::EventManager(std::shared_ptr<ObjectManager> pObjectManager)
{
	mpObjectManager = pObjectManager;
}

EventManager::~EventManager()
{
}


// PUBLIC

void EventManager::DispatchEvent(const Event& rEvent) const
{
	std::shared_ptr<ObjectManager> pObjectManagerLocked = mpObjectManager.lock();
	std::list<std::shared_ptr<GameObject>> objectListShared = pObjectManagerLocked->GetAllObjects();
	for (const std::shared_ptr<GameObject>& rpGameObject : objectListShared)
	{
		if (!rpGameObject)
			continue;

		rpGameObject->HandleEvent(rEvent);
	}
}