#pragma once
#include "GameObject.h"
//#include "mydrawengine.h"

class Spaceship : public GameObject
{
public:
	Spaceship();
	~Spaceship();

	typedef GameObject Super;

	virtual void Initialise(Vector2D position = Vector2D(0, 0), float angle = 0) override;
	virtual ErrorType Update() override;

protected:
	Vector2D velocity;

	virtual void Move();

private:
	const wchar_t* renderSpritePath = L"Assets/basic.bmp";
};

