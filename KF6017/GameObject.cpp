#include "GameObject.h"
#include "mydrawengine.h"
#include "ServiceManager.h"

GameObject::GameObject(std::weak_ptr<ServiceManager> pServiceManager)
{
	SetActive(false);
	mpServiceManager = pServiceManager;
}


// PUBLIC

GameObject::~GameObject()
{
	Remove();
}

void GameObject::Initialise(Vector2D position, float angle, float scale)
{
	mPosition = position;
	mRotationAngle = angle;
	mRenderScale = scale;

	SetActive(true);
}

ErrorType GameObject::Update(double deltaTime)
{
	if (!IsActive())
		return SUCCESS;

	return Render();
}

Vector2D GameObject::GetForwardVector() const
{
	Vector2D forward(0, 0);
	forward.setBearing(mRotationAngle, 1);
	return forward;
}

void GameObject::SetActive(bool newActive)
{
	mState = newActive ? GameObjectState::active : GameObjectState::inactive;
}

void GameObject::Remove()
{
	mState = GameObjectState::removed;
}

bool GameObject::IsActive() const
{
	return mState == GameObjectState::active;
}

bool GameObject::IsRemoved() const
{
	return mState == GameObjectState::removed;
}

ObjectType GameObject::GetObjectType() const
{
	return mObjectType;
}

void GameObject::HandleEvent(const Event& rEvent)
{
}

void GameObject::SetPosition(const Vector2D position)
{
	mPosition = position;
}

void GameObject::SetPosition(const float x, const float y)
{
	mPosition.XValue = x;
	mPosition.YValue = y;
}


// PROTECTED

void GameObject::SetRenderSprite(const wchar_t* imagePath)
{
	if (!imagePath)
	{
		ErrorLogger::Writeln(L"GameObject::SetRenderSprite(); Provided image path is null.");
		return;
	}

	std::shared_ptr<MyDrawEngine> pDrawEngine = mpServiceManager.lock()->GetDrawEngine().lock();

	if (!pDrawEngine)
	{
		ErrorLogger::Writeln(L"GameObject::SetRenderSprite(); Draw engine is null.");
		return;
	}

	mRenderSprite = pDrawEngine->LoadPicture(imagePath);
}

ErrorType GameObject::Render() 
{
	if (mRenderSprite == -1) 
	{
		ErrorLogger::Writeln(L"GameObject::Render(); renderSprite has not been set.");
		return FAILURE;
	}

	std::shared_ptr<MyDrawEngine> pDrawEngine = mpServiceManager.lock()->GetDrawEngine().lock();
	return pDrawEngine->DrawAt(mPosition, mRenderSprite, mRenderScale, mRotationAngle);
}


// PRIVATE