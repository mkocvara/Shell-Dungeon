#include "Attack.h"
#include "Weapon.h"
#include "ObjectType.h"

#include "ServiceManager.h"
#include "mydrawengine.h"

#include "Creature.h"


// PUBLIC

Attack::Attack(const std::weak_ptr<ServiceManager> pServiceManager)
	: Super(pServiceManager)
{
}

Attack::~Attack()
{
}

void Attack::Initialise(Vector2D position, float angle, float scale, PictureIndex sprite, std::shared_ptr<Weapon> pSourceWeapon, std::shared_ptr<Creature> pAttacker, float moveSpeed)
{
	Super::Initialise(position, angle, scale);

	mObjectType = ObjectType::attack;

	SetMovementSpeed(moveSpeed);
	SetTimeToFullSpeed(0.f);
	SetTimeToStop(0.f);

	mRenderSprite = sprite;
	mpSourceWeapon = pSourceWeapon;
	mpAttacker = pAttacker; 

	const std::shared_ptr<MyDrawEngine> pDrawEngine = mpServiceManager.lock()->GetDrawEngine().lock();
	int spriteHeight, spriteWidth;
	pDrawEngine->GetDimensions(mRenderSprite, spriteHeight, spriteWidth);
	mpBoundingShape = std::make_shared<AngledRectangle2D>(position, (float)spriteHeight, (float)spriteWidth);
	mpBoundingShape->SetAngle(mRotationAngle);
}

ErrorType Attack::Update(double deltaTime)
{
	if (mTimer >= mDuration)
	{
		Remove();
		return SUCCESS;
	}

	mpBoundingShape->SetCentre(mPosition);

	mTimer += deltaTime;

	return Super::Update(deltaTime);
}

std::weak_ptr<IShape2D> Attack::GetShape() const
{
	return mpBoundingShape;
}

void Attack::HandleCollision(const std::shared_ptr<GameObject> pOtherObject)
{
}

int Attack::GetDamage() const
{
	return mpSourceWeapon->GetBaseAttackDamage() + mpAttacker->GetBonusDamage();
}

std::shared_ptr<GameObject> Attack::GetAttacker() const
{
	return mpAttacker;
}

void Attack::AddHitEntity(const std::weak_ptr<GameObject> pObject)
{
	mEntitiesHit.push_back(pObject);
}

bool Attack::HasEntityBeenHit(const std::weak_ptr<GameObject> pObject) const
{
	bool found = false;

	for (const std::weak_ptr<GameObject> pHitEntity : mEntitiesHit)
	{
		// compares control blocks of weak_ptrs, if they are the same
		if (!(pHitEntity.owner_before(pObject) || pObject.owner_before(pHitEntity)))
		{
			found = true;
			break;
		}
	}

	return found;
}
