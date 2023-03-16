#include "SpaceRock.h"
#include "errorlogger.h"
#include "RandUtil.h"
#include "ServiceManager.h"
#include "mydrawengine.h"

// PUBLIC

SpaceRock::SpaceRock(std::weak_ptr<ServiceManager> serviceManager) : Super(serviceManager)
{
	objectType = ObjectType::spacerock;
}

SpaceRock::~SpaceRock()
{
}

void SpaceRock::Initialise(Vector2D position, float angle, float scale)
{
	int randSprite = RandUtil::randRangeInt(0, _renderSpritePaths.size()-1);	
	float randAngle = RandUtil::randRangeDouble(0, 360);
	float randScale = RandUtil::randRangeDouble(0.6f, 1.f);
	Vector2D moveDir(RandUtil::randRangeDouble(0.f, 1.f), RandUtil::randRangeDouble(0.f, 1.f));
	float randSpeed = RandUtil::randRangeDouble(15.f, 25.f);
	velocity = moveDir * randSpeed;

	std::shared_ptr<MyDrawEngine> pDrawEngine = serviceManager.lock()->GetDrawEngine().lock();
	Vector2D randPos = pDrawEngine->camera->ReverseTransform(Vector2D(
		RandUtil::randRangeDouble(0.f, pDrawEngine->GetScreenWidth()), 
		RandUtil::randRangeDouble(0.f, pDrawEngine->GetScreenHeight())
	));

	SetRenderSprite(_renderSpritePaths[randSprite]);

	int spriteHeight, spriteWidth;
	pDrawEngine->GetDimensions(renderSprite, spriteHeight, spriteWidth);
	spriteHeight *= randScale;
	spriteWidth *= randScale;
	const float diameter = (float)(spriteHeight < spriteWidth ? spriteHeight : spriteWidth);
	boundingShape = std::make_shared<Circle2D>(position, diameter/2);

	Super::Initialise(randPos, randAngle, randScale);
	return;
}

ErrorType SpaceRock::Update(double deltaTime)
{
	if (!IsActive())
		return SUCCESS;

	boundingShape->PlaceAt(position);

	if (FAILED(Super::Update(deltaTime)))
	{
		return FAILURE;
	}

	return SUCCESS;
}

std::weak_ptr<IShape2D> SpaceRock::GetShape() const
{
	return boundingShape;
}

void SpaceRock::HandleCollision(std::shared_ptr<GameObject> otherObject)
{
	if (otherObject->GetObjectType() == ObjectType::bullet)
	{
		Remove();
	}
}
