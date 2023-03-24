#include "Spaceship.h"
#include "myinputs.h"
#include "errorlogger.h"
#include "Shapes.h"

#include "ServiceManager.h"
#include "GameObjectFactory.h"
#include "mydrawengine.h"
#include "AsteroidsSFXManager.h"


// PUBLIC

Spaceship::Spaceship(std::weak_ptr<ServiceManager> pServiceManager) : Super(pServiceManager, -0.5)
{
	mObjectType = ObjectType::spaceship;
}

Spaceship::~Spaceship()
{
}

void Spaceship::Initialise(Vector2D position, float angle, float scale)
{
	SetRenderSprite(mRenderSpritePath);

	std::shared_ptr<MyDrawEngine> pDrawEngine = mpServiceManager.lock()->GetDrawEngine().lock();
	int spriteHeight, spriteWidth;
	pDrawEngine->GetDimensions(mRenderSprite, spriteHeight, spriteWidth);
	mpBoundingShape = std::make_shared<AngledRectangle2D>(position, (float)spriteHeight, (float)spriteWidth);
	mpBoundingShape->SetAngle(mRotationAngle);

	Super::Initialise(position, angle, scale);
	return;
}

ErrorType Spaceship::Update(double deltaTime)
{
	if (!IsActive())
		return SUCCESS;
	
	mpBoundingShape->SetCentre(mPosition);
	mpBoundingShape->SetAngle(mRotationAngle);

	HandleInputs(deltaTime);

	if (FAILED(Super::Update(deltaTime)))
	{
		return FAILURE;
	}

	return SUCCESS;
}

std::weak_ptr<IShape2D> Spaceship::GetShape() const
{
	return mpBoundingShape;
}

void Spaceship::HandleCollision(std::shared_ptr<GameObject> pOtherObject)
{
	if (pOtherObject->GetObjectType() == ObjectType::spacerock)
	{
		Die();
	}
}


// PROTECTED

void Spaceship::HandleInputs(double deltaTime)
{
	std::shared_ptr<MyInputs> pInputs = mpServiceManager.lock()->GetInputs().lock();
	pInputs->SampleKeyboard();
	pInputs->SampleMouse();

	// MOVEMENT
	// Rotation
	short rotDir = 0;
	if (pInputs->KeyPressed(DIK_LEFT) || pInputs->KeyPressed(DIK_A))
		rotDir = -1;
	else if (pInputs->KeyPressed(DIK_RIGHT) || pInputs->KeyPressed(DIK_D))
		rotDir = +1;

	mRotationAngle += rotDir * mRotationSpeed * static_cast<float>(deltaTime);

	// Movement
	if (pInputs->KeyPressed(DIK_UP) || pInputs->KeyPressed(DIK_W))
	{
		// accelerate
		Vector2D a;
		a.setBearing(mRotationAngle, mAcceleration);
		mVelocity += a;
	}

	// FIRING
	if (pInputs->IfMouseNewLeftDown())
	{
		Shoot();
	}
}

void Spaceship::Shoot()
{
	std::shared_ptr<GameObjectFactory> pObjectFactory = mpServiceManager.lock()->GetObjectFactory().lock();
	if (!pObjectFactory)
	{
		ErrorLogger::Writeln(L"Spaceship failed to retreive object factory.");
		return;
	}

	pObjectFactory->Create(ObjectType::bullet, mpServiceManager, mPosition, mRotationAngle, 1.5f);

	std::shared_ptr<SFXManager> pSFXManager = mpServiceManager.lock()->GetSFXManager().lock();
	if (!pSFXManager)
	{
		ErrorLogger::Writeln(L"Spaceship failed to retreive SFX manager.");
		return;
	}

	bool castSuccess = false;
	if (typeid(*pSFXManager) == typeid(AsteroidsSFXManager))
	{
		std::shared_ptr<AsteroidsSFXManager> pSFXManager = std::static_pointer_cast<AsteroidsSFXManager>(pSFXManager);
		if (pSFXManager)
		{
			pSFXManager->PlayShot();
			castSuccess = true;
		}
	}
	
	if (!castSuccess)
	{
		ErrorLogger::Writeln(L"Spaceship failed to cast SFX manager to game specific manager.");
		return;
	}
}

void Spaceship::Die()
{
	Remove();

	std::shared_ptr<GameObjectFactory> pObjectFactory = mpServiceManager.lock()->GetObjectFactory().lock();
	if (!pObjectFactory)
		return;

	pObjectFactory->Create(ObjectType::explosion, mpServiceManager, mPosition, 0.f, 2.f);
}
