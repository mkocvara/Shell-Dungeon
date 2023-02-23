#pragma once
#include "GameObject.h"
//#include "mydrawengine.h"

class Spaceship : public GameObject
{
public:
	Spaceship();
	virtual ~Spaceship();

	typedef GameObject Super;

	virtual void Initialise(Vector2D position = Vector2D(0, 0), float angle = 0, float scale = 1) override;
	virtual ErrorType Update(float deltaTime) override;

protected:
	Vector2D velocity;
	
	const float rotationSpeed = 2.f; 
	const float acceleration = 5.f;
	const float friction = -0.5f;

	virtual void Move(float deltaTime);

private:
	const wchar_t* renderSpritePath = L"Assets/basic.bmp";
};

