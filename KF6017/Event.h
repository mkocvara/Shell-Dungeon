/*
* Event.h
* 19002175
*
* Event struct with a simple overloaded constructor.
* Represents an event that can be dispached via the EventManager.
*/

#pragma once

#include "EventType.h"
#include "vector2D.h"
#include <memory>

class GameObject;

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

	Event(EventType type, Vector2D location)
	{
		this->type = type;
		this->location = location;
		this->pSourceObject = nullptr;
	}
};