#include "GameObject.h"
#include "mydrawengine.h"

// PUBLIC

GameObject::GameObject()
{
	SetActive(false);
}

GameObject::~GameObject()
{
	SetActive(false);
}

void GameObject::Initialise(Vector2D position, float angle)
{
	this->position = position;
	this->directionAngle = angle;

	SetActive(true);
}

ErrorType GameObject::Update()
{
	if (!IsActive())
		return SUCCESS;

	return Render();
}

void GameObject::SetRenderSprite(const wchar_t* imagePath)
{
	MyDrawEngine* pDrawEngine = MyDrawEngine::GetInstance();

	if (!pDrawEngine)
	{
		ErrorLogger::Writeln(L"GameObject::SetRenderSprite(); MyDrawEngine instance is null");
		return;
	}

	renderSprite = pDrawEngine->LoadPicture(imagePath);
}

Vector2D GameObject::GetForwardVector() const
{
	Vector2D forward(0, 0);
	forward.setBearing(directionAngle, 1);
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

	if (!pDrawEngine)
	{
		ErrorLogger::Writeln(L"GameObject::Render(); MyDrawEngine instance is null.");
		return FAILURE;
	}

	return pDrawEngine->DrawAt(position, renderSprite);
}


// PRIVATE