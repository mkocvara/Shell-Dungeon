#include "ObjectManager.h"
#include "GameObject.h"
#include "ICollidableObject.h"
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

void ObjectManager::AddObject(std::shared_ptr<GameObject>& newObject)
{
	gameObjects.push_back(newObject);
}

void ObjectManager::AddCollidableObject(std::shared_ptr<GameObject>& asGameObject, std::shared_ptr<ICollidableObject>& asCollidableObject)
{
	AddObject(asGameObject);
	collidableObjectsLookup.emplace(asGameObject, asCollidableObject); 
}

ErrorType ObjectManager::UpdateAll(double deltaTime)
{
	for (const std::shared_ptr<GameObject>& gameObject : gameObjects)
	{
		if (!gameObject)
			continue;

		if (FAILED(gameObject->Update(deltaTime)))
			return FAILURE;
	}

	CheckCollisions();
	RemoveDeletedObjects(); // TODO Perhaps run this only every so often instead?

	return SUCCESS;
}

void ObjectManager::Clear()
{
	gameObjects.clear();
	collidableObjectsLookup.clear();
}

int ObjectManager::GetNumberOfObjects() const
{
	return gameObjects.size();
}

void ObjectManager::CheckCollisions()
{
	std::list<std::shared_ptr<GameObject>>::iterator it1;
	std::list<std::shared_ptr<GameObject>>::iterator it2;
	for (it1 = gameObjects.begin(); it1 != gameObjects.end(); it1++)
	{
		for (it2 = std::next(it1); it2 != gameObjects.end(); it2++)
		{
			std::shared_ptr<GameObject> go1 = *it1;
			std::shared_ptr<GameObject> go2 = *it2;

			// Check that both objects exist and are active
			if (!go1 || !go2 || !go1->IsActive() || !go2->IsActive())
				continue;

			// Check that both objects are collidable.
			if (!collidableObjectsLookup[go1] || !collidableObjectsLookup[go2])
				continue;

			std::shared_ptr<ICollidableObject> co1 = collidableObjectsLookup[go1];
			std::shared_ptr<ICollidableObject> co2 = collidableObjectsLookup[go2];
			
			std::shared_ptr<IShape2D> shape1 = co1->GetShape().lock();
			std::shared_ptr<IShape2D> shape2 = co2->GetShape().lock();

			if (shape1->Intersects(*(shape2.get())))
			{
				co1->HandleCollision(go2);
				co2->HandleCollision(go1);
			}
		}
	}
}

void ObjectManager::RemoveDeletedObjects()
{
	for (std::shared_ptr<GameObject>& gameObject : gameObjects)
	{
		if (gameObject.get()->IsRemoved())
		{
			collidableObjectsLookup.erase(gameObject);
			gameObject.reset();
		}
	}

	gameObjects.remove(nullptr);
}