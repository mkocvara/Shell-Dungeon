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

	std::shared_ptr<MyDrawEngine> pDrawEngine = serviceManager.lock()->GetDrawEngine().lock();
	int spriteHeight, spriteWidth;
	pDrawEngine->GetDimensions(renderSprite, spriteHeight, spriteWidth);
	boundingShape = std::make_shared<AngledRectangle2D>(position, (float)spriteHeight, (float)spriteWidth);
	boundingShape->SetAngle(rotationAngle);

	Super::Initialise(position, angle, scale);
	return;
}

ErrorType Bullet::Update(double deltaTime)
{
	if (FAILED(Super::Update(deltaTime)))
		return FAILURE;

	if (IsOffScreen())
		Remove();

	boundingShape->SetCentre(position);
	boundingShape->SetAngle(rotationAngle);

	return SUCCESS;
}

std::weak_ptr<IShape2D> Bullet::GetShape() const
{
	return boundingShape;
}

void Bullet::HandleCollision(std::shared_ptr<GameObject> otherObject)
{
	// TODO NOT IMPLEMENTED
	ErrorLogger::Writeln(L"Bullet collided!");
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
