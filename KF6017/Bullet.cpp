#include "Bullet.h"
#include <winerror.h>

#include "ServiceManager.h"
#include "mydrawengine.h"


// PUBLIC

Bullet::Bullet(std::weak_ptr<ServiceManager> serviceManager) : Super(serviceManager)
{
}

Bullet::~Bullet()
{
}

void Bullet::Initialise(Vector2D position, float angle, float scale)
{
	velocity.setBearing(angle, speed);
	SetRenderSprite(renderSpritePath);
	Super::Initialise(position, angle, scale);
	return;
}

ErrorType Bullet::Update(double deltaTime)
{
	if (FAILED(Super::Update(deltaTime)))
		return FAILURE;

	if (IsOffScreen())
		Remove();

	return SUCCESS;
}


// PROTECTED

void Bullet::Move(double deltaTime)
{
	position += velocity;
}

bool Bullet::IsOffScreen()
{
	std::shared_ptr<MyDrawEngine> pDrawEngine = serviceManager.lock()->GetDrawEngine().lock();
	if (!pDrawEngine)
		return true;

	return !pDrawEngine->IsOnCamera(position);
}
