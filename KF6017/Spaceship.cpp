#include "Spaceship.h"
#include "myinputs.h"
#include "errorlogger.h"
#include "Shapes.h"

#include "ServiceManager.h"
#include "ObjectManager.h"
#include "GameObjectFactory.h"
#include "mydrawengine.h"


// PUBLIC

Spaceship::Spaceship(std::weak_ptr<ServiceManager> serviceManager) :
	Super(serviceManager, -0.5)
{
	objectType = ObjectType::spaceship;
}

Spaceship::~Spaceship()
{
}

void Spaceship::Initialise(Vector2D position, float angle, float scale)
{
	SetRenderSprite(renderSpritePath);

	std::shared_ptr<MyDrawEngine> pDrawEngine = serviceManager.lock()->GetDrawEngine().lock();
	int spriteHeight, spriteWidth;
	pDrawEngine->GetDimensions(renderSprite, spriteHeight, spriteWidth);
	boundingShape = std::make_shared<AngledRectangle2D>(position, (float)spriteHeight, (float)spriteWidth);
	boundingShape->SetAngle(rotationAngle);

	Super::Initialise(position, angle, scale);
	return;
}

ErrorType Spaceship::Update(double deltaTime)
{
	if (!IsActive())
		return SUCCESS;
	
	boundingShape->SetCentre(position);
	boundingShape->SetAngle(rotationAngle);

	HandleInputs(deltaTime);

	if (FAILED(Super::Update(deltaTime)))
	{
		return FAILURE;
	}

	return SUCCESS;
}

std::weak_ptr<IShape2D> Spaceship::GetShape() const
{
	return boundingShape;
}

void Spaceship::HandleCollision(std::shared_ptr<GameObject> otherObject)
{
	// NOT IMPLEMENTED
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

	pObjectFactory->Create(ObjectType::bullet, serviceManager, position, rotationAngle, 1.5f);
}
