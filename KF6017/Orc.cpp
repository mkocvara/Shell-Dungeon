#include "Orc.h"
#include "ObjectType.h"

#include "ServiceManager.h"
#include "mydrawengine.h"


// PUBLIC

Orc::Orc(const std::weak_ptr<ServiceManager> pServiceManager)
	: Super(pServiceManager)
{
	mObjectType = ObjectType::orc;
}

Orc::~Orc()
{
}

void Orc::Initialise(Vector2D position, float angle, float scale)
{
	scale *= mBaseSpriteScale;

	SetRenderSprite(mRenderSpritePath);

	SetMaxHealth(mMaxHP);
	Heal(mMaxHP);
	SetShowHealthBar(true);

	const std::shared_ptr<MyDrawEngine> pDrawEngine = mpServiceManager.lock()->GetDrawEngine().lock();
	int spriteHeight, spriteWidth;
	pDrawEngine->GetDimensions(mRenderSprite, spriteHeight, spriteWidth);
	mpBoundingShape = std::make_shared<Rectangle2D>();
	mpBoundingShape->PlaceAt(position, (float)spriteHeight * scale, (float)spriteWidth * scale);
	
	Super::Initialise(position, angle, scale);
}

ErrorType Orc::Update(double deltaTime)
{
	mpBoundingShape->SetCentre(mPosition);

	return Super::Update(deltaTime);
}

std::weak_ptr<IShape2D> Orc::GetShape() const
{
	return mpBoundingShape;
}

void Orc::Attack()
{
	// TODO
	Super::Attack();
}
