/*
* DungeonObjectFactory.h
* 19002175
*
* Game-specific service for creating objects. Has weak dependencies on all game objects, so that others don't have to.
*/

#pragma once
#include "GameObjectFactory.h"

class DungeonObjectFactory final : public GameObjectFactory
{
public:
	DungeonObjectFactory(std::weak_ptr<ServiceManager> pServiceManager);
	virtual ~DungeonObjectFactory();

	typedef GameObjectFactory Super;

	
	// Creates an object of specified type and adds it to the object manager. 
	// Initialises the object by default, but this is optional (second parameter).
	// Returns a weak_ptr to the created object.
	virtual std::weak_ptr<GameObject> Create(ObjectType type, bool initialise, Vector2D initPosition, float initAngle, float initScale) override;

private:
	void InitialiseGameObject(std::shared_ptr<GameObject>& rpObj, Vector2D initPosition, float initAngle, float initScale);
};

