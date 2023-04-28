#pragma once
#include "GameObjectFactory.h"

class DungeonObjectFactory final : public GameObjectFactory
{
public:
	DungeonObjectFactory();
	virtual ~DungeonObjectFactory();

	typedef GameObjectFactory Super;

	/// <summary>
	/// Creates an object of specified type and adds it to the object manager in the passed service manager.
	/// </summary>
	/// <param name="type">Type of object to create</param>
	/// <param name="serviceManager">Service manager instance for the game</param>
	/// <returns>A weak_ptr to the created object.</returns>
	virtual std::weak_ptr<GameObject> Create(ObjectType type, std::weak_ptr<ServiceManager> pServiceManager, bool initialise, Vector2D initPosition, float initAngle, float initScale) override;

private:
	void InitialiseGameObject(std::shared_ptr<GameObject>& rpObj, Vector2D initPosition, float initAngle, float initScale);
};

