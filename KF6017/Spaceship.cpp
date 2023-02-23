#include "Spaceship.h"
#include "winerror.h"

// PUBLIC

Spaceship::Spaceship() : Super()
{
}

void Spaceship::Initialise(Vector2D position, float angle)
{
	Super::Initialise(position, angle);

	SetRenderSprite(renderSpritePath);

	return;
}

ErrorType Spaceship::Update()
{
	if (!IsActive())
		return;

	if (FAILED(Super::Update()))
	{
		return FAILURE;
	}

	Move();

	return SUCCESS;
}


// PROTECTED

void Spaceship::Move()
{
	position += velocity;
}
