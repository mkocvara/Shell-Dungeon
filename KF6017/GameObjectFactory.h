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
	GameObjectFactory();
	virtual ~GameObjectFactory();
	virtual std::weak_ptr<GameObject> Create(ObjectType type, std::weak_ptr<ServiceManager> pServiceManager, bool initialise = true, Vector2D initPosition = GetDefaultPositionValue(), float initAngle = GetDefaultAngleValue(), float initScale = GetDefaultScaleValue()) = 0;
	
protected:
	static Vector2D GetDefaultPositionValue();
	static float GetDefaultAngleValue();
	static float GetDefaultScaleValue();
};