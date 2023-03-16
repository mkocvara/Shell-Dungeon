#include "GameObjectFactory.h"

static const Vector2D defaultPosition = Vector2D(-1.f, -1.f);
static const float defaultAngle = -1;
static const float defaultScale = -1;

GameObjectFactory::GameObjectFactory()
{
}

GameObjectFactory::~GameObjectFactory()
{
}

Vector2D GameObjectFactory::GetDefaultPositionValue()
{
	static const Vector2D defaultPosition(-1.f,-1.f);
	return defaultPosition;
}

float GameObjectFactory::GetDefaultAngleValue()
{
	return -1.f;
}

float GameObjectFactory::GetDefaultScaleValue()
{
	return -1.f;
}
