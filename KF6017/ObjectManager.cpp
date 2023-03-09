#include "ObjectManager.h"
#include "GameObject.h"
#include <winerror.h>

#include "ServiceManager.h"
#include "MyDrawEngine.h"

// PUBLIC

ObjectManager::ObjectManager()
{
}

ObjectManager::~ObjectManager()
{
	gameObjects.clear();
}

void ObjectManager::AddObject(GameObject* newObject)
{
	std::unique_ptr<GameObject> pObj(newObject);
	AddObject(pObj);
}

void ObjectManager::AddObject(std::unique_ptr<GameObject>& newObject)
{
	gameObjects.push_back(std::move(newObject));
}

ErrorType ObjectManager::UpdateAll(double deltaTime)
{
	for (const std::unique_ptr<GameObject>& gameObject : gameObjects)
	{
		if (!gameObject)
			continue;

		if (FAILED(gameObject->Update(deltaTime)))
			return FAILURE;
	}

	RemoveDeletedObjects(); // Perhaps run this only every so often instead?

	return SUCCESS;
}

void ObjectManager::Clear()
{
	for (std::unique_ptr<GameObject>& gameObject : gameObjects) // Do I need to do this?
	{
		if (!gameObject)
			continue;

		gameObject.reset();
	}

	gameObjects.clear();
}

int ObjectManager::GetNumberOfObjects() const
{
	return gameObjects.size();
}

void ObjectManager::RemoveDeletedObjects()
{
	//std::list<std::unique_ptr<GameObject>>::iterator it;
	//for (it = mGameObjects.begin(); it != mGameObjects.end(); ++it) 
	for (std::unique_ptr<GameObject>& gameObject : gameObjects)
	{
		if (gameObject.get()->IsRemoved())
		{
			gameObject.reset();
		}
	}

	gameObjects.remove(nullptr);
}