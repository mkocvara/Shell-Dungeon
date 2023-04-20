#pragma once
#include "vector2D.h"
#include <memory>

class GameObject;

// Events used in the messaging/event system
enum class EventType 
{
	levelStart,
	asteroidDestroyed,
	bulletFired
};

struct Event 
{
	EventType type;
	Vector2D location;
	GameObject* pSourceObject; // only observes, should never be saved by event handlers

	Event(EventType type, Vector2D location, GameObject* pSourceObject)
	{
		this->type = type;
		this->location = location;
		this->pSourceObject = pSourceObject;
	}
};