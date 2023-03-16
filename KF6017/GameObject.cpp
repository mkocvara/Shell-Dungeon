#include "GameObject.h"
#include "mydrawengine.h"
#include "ServiceManager.h"

GameObject::GameObject(std::weak_ptr<ServiceManager> serviceManager)
{
	SetActive(false);
	this->serviceManager = serviceManager;
}

// PUBLIC

GameObject::~GameObject()
{
	Remove();
}

void GameObject::Initialise(Vector2D position, float angle, float scale)
{
	this->position = position;
	this->rotationAngle = angle;
	this->renderScale = scale;

	SetActive(true);
}

ErrorType GameObject::Update(double deltaTime)
{
	if (!IsActive())
		return SUCCESS;

	return Render();
}

void GameObject::SetRenderSprite(const wchar_t* imagePath)
{
	if (!imagePath)
	{
		ErrorLogger::Writeln(L"GameObject::SetRenderSprite(); Provided image path is null.");
		return;
	}

	std::shared_ptr<MyDrawEngine> pDrawEngine = serviceManager.lock()->GetDrawEngine().lock();

	if (!pDrawEngine)
	{
		ErrorLogger::Writeln(L"GameObject::SetRenderSprite(); Draw engine is null.");
		return;
	}

	renderSprite = pDrawEngine->LoadPicture(imagePath);
}

Vector2D GameObject::GetForwardVector() const
{
	Vector2D forward(0, 0);
	forward.setBearing(rotationAngle, 1);
	return forward;
}

void GameObject::SetActive(bool newActive)
{
	state = newActive ? GameObjectState::active : GameObjectState::inactive;
}

void GameObject::Remove()
{
	state = GameObjectState::removed;
}

bool GameObject::IsActive() const
{
	return state == GameObjectState::active;
}

bool GameObject::IsRemoved() const
{
	return state == GameObjectState::removed;
}

ObjectType GameObject::GetObjectType() const
{
	return objectType;
}


// PROTECTED
ErrorType GameObject::Render() 
{
	if (renderSprite == -1) 
	{
		ErrorLogger::Writeln(L"GameObject::Render(); renderSprite has not been set.");
		return FAILURE;
	}

	std::shared_ptr<MyDrawEngine> pDrawEngine = serviceManager.lock()->GetDrawEngine().lock();
	return pDrawEngine->DrawAt(position, renderSprite, renderScale, rotationAngle);
}


// PRIVATE