#include "Orc.h"
#include "ObjectType.h"
#include "RandUtil.h"

#include "ServiceManager.h"
#include "mydrawengine.h"
#include "DungeonGameManager.h"

#include "Club.h"


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
	
	SetMovementSpeed(6.f);
	SetTimeToFullSpeed(0.03f);
	SetTimeToStop(0.03f);

	EquipWeapon(std::shared_ptr<Weapon>(new Club(mpServiceManager)));

	Super::Initialise(position, angle, scale);
}

ErrorType Orc::Update(double deltaTime)
{
	mpBoundingShape->SetCentre(mPosition);

	const std::shared_ptr<GameManager> pGameManager = mpServiceManager.lock()->GetGameManager().lock();
	const std::shared_ptr<DungeonGameManager> pDungeonGameManager = std::static_pointer_cast<DungeonGameManager>(pGameManager);
	
	const Vector2D playerPosition = pDungeonGameManager->GetPlayerLocation();
	const Vector2D orcToPlayer = playerPosition - mPosition;
	const std::shared_ptr<Weapon> equippedWeapon = GetEquippedWeapon();

	if (equippedWeapon)
	{
		const float attackRange = GetEquippedWeapon()->GetBaseAttackReach();
		const float shapeOffset = pDungeonGameManager->GetPlayerBounds().lock()->GetWidth() / 2;
		const float attackDistanceVariability = (float)RandUtil::randRangeDouble(-attackRange/4, attackRange/4);

		if (orcToPlayer.magnitude() <= attackRange + shapeOffset + attackDistanceVariability)
		{
			mMoveDirection.set(0, 0);
			mAttackDirection = orcToPlayer;
			Attack();
		}
		else if (!IsAttackOnCooldown())
		{
			mMoveDirection = orcToPlayer;
		}
	}

	return Super::Update(deltaTime);
}

std::weak_ptr<IShape2D> Orc::GetShape() const
{
	return mpBoundingShape;
}

void Orc::Attack()
{
	if (IsAttackOnCooldown())
		return;

	const std::shared_ptr<Weapon> pWeapon = GetEquippedWeapon();
	pWeapon->Attack(shared_from_this(), mAttackDirection.angle());

	Super::Attack();
}