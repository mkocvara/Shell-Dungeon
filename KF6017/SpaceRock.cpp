#include "SpaceRock.h"
#include "errorlogger.h"
#include "RandUtil.h"
#include "ServiceManager.h"
#include "mydrawengine.h"
#include "EventManager.h"


// PUBLIC

SpaceRock::SpaceRock(std::weak_ptr<ServiceManager> pServiceManager) : Super(pServiceManager)
{
	mObjectType = ObjectType::spacerock;
	SetZIndex(2);
}

SpaceRock::~SpaceRock()
{
}

void SpaceRock::Initialise(Vector2D position, float angle, float scale)
{
	int randSprite = RandUtil::randRangeInt(0, mRenderSpritePaths.size()-1);	
	float randAngle = (float)RandUtil::randRangeDouble(0, 360);
	float randScale = (float)RandUtil::randRangeDouble(0.6f, 1.f);
	
	mMoveDirection = Vector2D((float)RandUtil::randRangeDouble(-1.f, 1.f), (float)RandUtil::randRangeDouble(-1.f, 1.f));
	SetMovementSpeed((float)RandUtil::randRangeDouble(0.1f, 1.2f));
	SetTimeToFullSpeed(0.f);
	SetTimeToStop(0.f);	

	std::shared_ptr<MyDrawEngine> pDrawEngine = mpServiceManager.lock()->GetDrawEngine().lock();
	Vector2D randPos = pDrawEngine->mpCamera->ReverseTransform(Vector2D(
		(float)RandUtil::randRangeDouble(0.f, pDrawEngine->GetScreenWidth()),
		(float)RandUtil::randRangeDouble(0.f, pDrawEngine->GetScreenHeight())
	));

	SetRenderSprite(mRenderSpritePaths[randSprite]);

	int spriteHeight, spriteWidth;
	pDrawEngine->GetDimensions(mRenderSprite, spriteHeight, spriteWidth);
	spriteHeight = (int)(randScale * spriteHeight);
	spriteWidth = (int)(randScale * spriteWidth);
	const float diameter = (float)(spriteHeight < spriteWidth ? spriteHeight : spriteWidth);
	mpBoundingShape = std::make_shared<Circle2D>(position, diameter/2);

	Super::Initialise(randPos, randAngle, randScale);
	return;
}

ErrorType SpaceRock::Update(double deltaTime)
{
	if (!IsActive())
		return SUCCESS;

	mpBoundingShape->PlaceAt(mPosition);

	if (FAILED(Super::Update(deltaTime)))
	{
		return FAILURE;
	}

	return SUCCESS;
}

std::weak_ptr<IShape2D> SpaceRock::GetShape() const
{
	return mpBoundingShape;
}

void SpaceRock::HandleCollision(std::shared_ptr<GameObject> pOtherObject)
{
	if (pOtherObject->GetObjectType() == ObjectType::bullet)
	{
		std::shared_ptr<EventManager> pEventManager = mpServiceManager.lock()->GetEventManager().lock();

		Event e(EventType::asteroidDestroyed, mPosition, this);
		pEventManager->DispatchEvent(e);

		Remove();
	}
}
