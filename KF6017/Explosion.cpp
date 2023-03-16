#include "Explosion.h"
#include <winerror.h>
#include <cmath>

Explosion::Explosion(std::weak_ptr<ServiceManager> serviceManager) : Super(serviceManager)
{
}

Explosion::~Explosion()
{
}


// PUBLIC

void Explosion::Initialise(Vector2D position, float angle, float scale)
{
	elapsedTime = 0.f;
	SetRenderSprite(renderSpritePaths[0]);

	Super::Initialise(position, angle, scale);
	return;
}

ErrorType Explosion::Update(double deltaTime)
{
	if (!IsActive())
		return SUCCESS;

	elapsedTime += deltaTime;
	if (elapsedTime >= animationLength)
	{
		Remove();
		return SUCCESS;
	}

	int spriteIndex = std::floor(std::lerp(0, renderSpritePaths.size(), elapsedTime/animationLength));
	SetRenderSprite(renderSpritePaths[spriteIndex]);

	if (FAILED(Super::Update(deltaTime)))
	{
		return FAILURE;
	}

	return SUCCESS;
}
