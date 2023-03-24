#include "Explosion.h"
#include <winerror.h>
#include <cmath>

Explosion::Explosion(std::weak_ptr<ServiceManager> pServiceManager) : Super(pServiceManager)
{
}

Explosion::~Explosion()
{
}


// PUBLIC

void Explosion::Initialise(Vector2D position, float angle, float scale)
{
	mElapsedTime = 0.f;
	SetRenderSprite(mRenderSpritePaths[0]);

	Super::Initialise(position, angle, scale);
	return;
}

ErrorType Explosion::Update(double deltaTime)
{
	if (!IsActive())
		return SUCCESS;

	mElapsedTime += deltaTime;
	if (mElapsedTime >= mAnimationLength)
	{
		Remove();
		return SUCCESS;
	}

	int spriteIndex = (int)std::floor(std::lerp(0, mRenderSpritePaths.size(), mElapsedTime/mAnimationLength));
	SetRenderSprite(mRenderSpritePaths[spriteIndex]);

	if (FAILED(Super::Update(deltaTime)))
	{
		return FAILURE;
	}

	return SUCCESS;
}
