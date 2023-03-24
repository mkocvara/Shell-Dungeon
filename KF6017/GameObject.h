#pragma once
#include "vector2D.h"
#include "errortype.h"
#include <memory>
#include "objecttypes.h"

class ServiceManager;

enum class GameObjectState
{
	active,
	inactive,
	removed
};

class GameObject
{
public:
	virtual ~GameObject();

	virtual void Initialise(Vector2D position = Vector2D(0, 0), float angle = 0, float scale = 1);
	virtual ErrorType Update(double deltaTime);
	virtual void SetRenderSprite(const wchar_t* imagePath);
	Vector2D GetForwardVector() const;
	virtual void SetActive(bool newActive);
	virtual void Remove();
	bool IsActive() const;
	bool IsRemoved() const;
	virtual ObjectType GetObjectType() const;

protected:
	GameObject(std::weak_ptr<ServiceManager> pServiceManager); // make it essentially abstract
	Vector2D mPosition;
	float mRotationAngle;	
	float mRenderScale;

	std::weak_ptr<ServiceManager> mpServiceManager;

	int mRenderSprite = -1;
	virtual ErrorType Render();

	ObjectType mObjectType;

private: 
	GameObjectState mState;
};

