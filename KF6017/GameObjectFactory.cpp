/*
* GameObjectFactory.cpp
* 19002175
*
* Abstract class for a service for creating objects.
* Intended to be inherited from by a game-specific class.
*/

#include "GameObjectFactory.h"


// PUBLIC

GameObjectFactory::GameObjectFactory(std::weak_ptr<ServiceManager> mpServiceManager)
	: Super(), mpServiceManager(mpServiceManager)
{
}

GameObjectFactory::~GameObjectFactory()
{
}


// PROTECTED

Vector2D GameObjectFactory::GetDefaultPositionValue()
{
	static const Vector2D sDefaultPosition(-1.f,-1.f);
	return sDefaultPosition;
}

float GameObjectFactory::GetDefaultAngleValue()
{
	return -1.f;
}

float GameObjectFactory::GetDefaultScaleValue()
{
	return -1.f;
}
