/*
* GameObjectFactory.h
* 19002175
*
* Abstract class for a service for creating objects.
* Intended to be inherited from by a game-specific class.
*/

#pragma once
#include "Service.h"
#include <memory>
#include "ObjectType.h"
#include "vector2D.h"

class GameObject;
class ServiceManager;

class GameObjectFactory : public Service
{
	typedef Service Super;

public:
	GameObjectFactory(std::weak_ptr<ServiceManager> pServiceManager);
	virtual ~GameObjectFactory();
	virtual std::weak_ptr<GameObject> Create(ObjectType type, bool initialise = true, Vector2D initPosition = GetDefaultPositionValue(), float initAngle = GetDefaultAngleValue(), float initScale = GetDefaultScaleValue()) = 0;
	
protected:
	std::weak_ptr<ServiceManager> mpServiceManager;

	static Vector2D GetDefaultPositionValue();
	static float GetDefaultAngleValue();
	static float GetDefaultScaleValue();
};