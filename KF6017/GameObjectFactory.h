#pragma once
#include <memory>
#include "objecttypes.h"
#include "vector2D.h"

class GameObject;
class ServiceManager;

class GameObjectFactory
{
public:
	GameObjectFactory();
	virtual ~GameObjectFactory();
	virtual std::weak_ptr<GameObject> Create(ObjectType type, std::weak_ptr<ServiceManager> serviceManager, Vector2D initPosition = GetDefaultPositionValue(), float initAngle = GetDefaultAngleValue(), float initScale = GetDefaultScaleValue()) = 0;
	
protected:
	static Vector2D GetDefaultPositionValue();
	static float GetDefaultAngleValue();
	static float GetDefaultScaleValue();
};