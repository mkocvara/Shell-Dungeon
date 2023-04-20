#pragma once
#include "Service.h"
#include <memory>
#include <list>
#include <map>
#include "errortype.h"

class GameObject;
class ICollidableObject;

class ObjectManager : public Service
{
	typedef Service Super;

public:
	ObjectManager();
	~ObjectManager();

	ErrorType UpdateAll(double deltaTime);
	void AddObject(std::shared_ptr<GameObject>& rpNewObject);
	void AddCollidableObject(std::shared_ptr<GameObject>& rpAsGameObject, std::shared_ptr<ICollidableObject>& rpAsCollidableObject);
	void Clear(); // removes all objects from the game
	int GetNumberOfObjects() const;

	// Mind that this returns a copy of the list with new shared ptrs, sharing ownership!
	std::list<std::shared_ptr<GameObject>> GetAllObjects() const;

private:
	void CheckCollisions();
	void RemoveDeletedObjects();
	std::list<std::shared_ptr<GameObject>> mGameObjectsList;
	std::map<std::shared_ptr<GameObject>, std::shared_ptr<ICollidableObject>> mObjectsCollidableMap;
};

