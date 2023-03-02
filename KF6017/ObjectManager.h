#pragma once
#include <memory>
#include <list>
#include "errortype.h"

class GameObject;

class ObjectManager
{
public:
	ObjectManager();
	~ObjectManager();

	ErrorType UpdateAll(double deltaTime);
	void AddObject(GameObject* newObject);
	void AddObject(std::unique_ptr<GameObject>& newObject);

private:
	std::list<std::unique_ptr<GameObject>> mGameObjects;
};

