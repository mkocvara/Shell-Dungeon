#include "Spaceship.h"
//#include "winerror.h"
#include "myinputs.h"
#include "errorlogger.h"

#include "ServiceManager.h"
#include "ObjectManager.h"
#include "GameObjectFactory.h"


// PUBLIC

Spaceship::Spaceship(std::weak_ptr<ServiceManager> serviceManager) : Super(serviceManager, -0.5)
{
}

Spaceship::~Spaceship()
{
}

void Spaceship::Initialise(Vector2D position, float angle, float scale)
{
	SetRenderSprite(renderSpritePath);
	Super::Initialise(position, angle, scale);
	return;
}

ErrorType Spaceship::Update(double deltaTime)
{
	if (!IsActive())
		return SUCCESS;

	HandleInputs(deltaTime);

	if (FAILED(Super::Update(deltaTime)))
	{
		return FAILURE;
	}

	return SUCCESS;
}


// PROTECTED

void Spaceship::HandleInputs(double deltaTime)
{
	std::shared_ptr<MyInputs> pInputs = serviceManager.lock()->GetInputs().lock();
	pInputs->SampleKeyboard();
	pInputs->SampleMouse();

	// MOVEMENT
	// Rotation
	short rotDir = 0;
	if (pInputs->KeyPressed(DIK_LEFT) || pInputs->KeyPressed(DIK_A))
		rotDir = -1;
	else if (pInputs->KeyPressed(DIK_RIGHT) || pInputs->KeyPressed(DIK_D))
		rotDir = +1;

	rotationAngle += rotDir * rotationSpeed * static_cast<float>(deltaTime);

	// Movement
	if (pInputs->KeyPressed(DIK_UP) || pInputs->KeyPressed(DIK_W))
	{
		// accelerate
		Vector2D a;
		a.setBearing(rotationAngle, acceleration);
		velocity += a;
	}

	// FIRING
	if (pInputs->IfMouseNewLeftDown())
	{
		Shoot();
	}
}

void Spaceship::Shoot()
{
	std::shared_ptr<ObjectManager> pObjectManager = serviceManager.lock()->GetObjectManager().lock();
	if (!pObjectManager)
		return;

	std::shared_ptr<GameObjectFactory> pObjectFactory = serviceManager.lock()->GetObjectFactory().lock();
	if (!pObjectFactory)
		return;

	std::unique_ptr<GameObject> obj = pObjectFactory->Create(ObjectType::bullet, serviceManager);
	obj->Initialise(position, rotationAngle, 1.f);
	pObjectManager->AddObject(obj);
}
