#pragma once
#include "vector2D.h"
#include "errortype.h"

class GameObject
{
public:
	GameObject();
	~GameObject();

	virtual void Initialise(Vector2D position = Vector2D(0, 0), float angle = 0);
	virtual void Update();
	Vector2D GetForwardVector() const;
	virtual void SetActive(bool newActive);
	bool IsActive() const;

protected:
	Vector2D position;
	float directionAngle;	

	virtual void Render() =0;

private:
	bool active;
};

