#include "Bullet.h"
#include <winerror.h>

#include "ServiceManager.h"
#include "mydrawengine.h"


// PUBLIC

Bullet::Bullet(std::weak_ptr<ServiceManager> pServiceManager) : Super(pServiceManager)
{
	mObjectType = ObjectType::bullet;
}

Bullet::~Bullet()
{
}

void Bullet::Initialise(Vector2D position, float angle, float scale)
{
	mVelocity.setBearing(angle, mSpeed);
	SetRenderSprite(mRenderSpritePath);

	std::shared_ptr<MyDrawEngine> pDrawEngine = mpServiceManager.lock()->GetDrawEngine().lock();
	int spriteHeight, spriteWidth;
	pDrawEngine->GetDimensions(mRenderSprite, spriteHeight, spriteWidth);
	mpBoundingShape = std::make_shared<AngledRectangle2D>(position, (float)spriteHeight, (float)spriteWidth);
	mpBoundingShape->SetAngle(mRotationAngle);

	Super::Initialise(position, angle, scale);
	return;
}

ErrorType Bullet::Update(double deltaTime)
{
	if (FAILED(Super::Update(deltaTime)))
		return FAILURE;

	if (IsOffScreen())
		Remove();

	mpBoundingShape->SetCentre(mPosition);
	mpBoundingShape->SetAngle(mRotationAngle);

	return SUCCESS;
}

std::weak_ptr<IShape2D> Bullet::GetShape() const
{
	return mpBoundingShape;
}

void Bullet::HandleCollision(std::shared_ptr<GameObject> pOtherObject)
{
	if (pOtherObject->GetObjectType() == ObjectType::spacerock)
	{
		Remove();
	}
}


// PROTECTED

void Bullet::Move(double deltaTime)
{
	mPosition += mVelocity;
}

bool Bullet::IsOffScreen()
{
	std::shared_ptr<MyDrawEngine> pDrawEngine = mpServiceManager.lock()->GetDrawEngine().lock();
	if (!pDrawEngine)
		return true;

	return !pDrawEngine->IsOnCamera(mPosition);
}
