#include "GameObject.h"

// PUBLIC

GameObject::GameObject()
{
	SetActive(false);
}

GameObject::~GameObject()
{
	SetActive(false);
}

void GameObject::Initialise(Vector2D position, float angle)
{
	this->position = position;
	this->directionAngle = angle;

	SetActive(true);
}

void GameObject::Update()
{
	if (!active)
		return;

	Render();
}

Vector2D GameObject::GetForwardVector() const
{
	Vector2D forward(0, 0);
	forward.setBearing(directionAngle, 1);
	return forward;
}

void GameObject::SetActive(bool newActive)
{
	active = newActive;
}

bool GameObject::IsActive() const
{
	return active;
}


// PROTECTED



// PRIVATE