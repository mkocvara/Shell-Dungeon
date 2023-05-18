/*
* GameObject.h
* 19002175
* 
* Base abstract superclass for all objects within the game world
*/

#pragma once
#include "vector2D.h"
#include "errortype.h"
#include <memory>
#include "ObjectType.h"

struct Event;
class ServiceManager;

typedef int PictureIndex;

class GameObject : public std::enable_shared_from_this<GameObject>
{
public:
	virtual ~GameObject();

	virtual void Initialise(Vector2D position = Vector2D(0, 0), float angle = 0, float scale = 1);
	virtual ErrorType Update(double deltaTime);
	Vector2D GetForwardVector() const;
	virtual void SetActive(bool newActive);
	virtual void Remove();
	bool IsActive() const;
	bool IsRemoved() const;
	virtual ObjectType GetObjectType() const;
	virtual void HandleEvent(const Event& rEvent);
	Vector2D GetPosition() const;
	void SetPosition(const Vector2D position);
	void SetPosition(const float x, const float y);
	virtual void SetRenderSprite(PictureIndex spriteIndex);
	virtual PictureIndex SetRenderSprite(const wchar_t* imagePath);
	int GetZIndex() const;
	void SetZIndex(int newZIndex);

protected:
	GameObject(std::weak_ptr<ServiceManager> pServiceManager); // make it essentially abstract
	Vector2D mPosition;
	float mRotationAngle;	
	float mRenderScale;

	int mZIndex = 0;

	std::weak_ptr<ServiceManager> mpServiceManager;

	PictureIndex mRenderSprite = -1;
	virtual ErrorType Render();

	ObjectType mObjectType;

private: 
	enum class GameObjectState
	{
		active,
		inactive,
		removed
	};

	GameObjectState mState;
};

