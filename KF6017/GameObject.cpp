#include "GameObject.h"
#include "mydrawengine.h"


GameObject::GameObject()
{
	SetActive(false);
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

ErrorType GameObject::Update(float deltaTime)
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

	MyDrawEngine* pDrawEngine = MyDrawEngine::GetInstance();
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


// PROTECTED
ErrorType GameObject::Render() 
{
	if (renderSprite == -1) 
	{
		ErrorLogger::Writeln(L"GameObject::Render(); renderSprite has not been set.");
		return FAILURE;
	}

	MyDrawEngine* pDrawEngine = MyDrawEngine::GetInstance();
	return pDrawEngine->DrawAt(position, renderSprite, renderScale, rotationAngle);
}


// PRIVATE