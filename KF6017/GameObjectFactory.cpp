#include "GameObjectFactory.h"


// PUBLIC

GameObjectFactory::GameObjectFactory()
	: Super()
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
