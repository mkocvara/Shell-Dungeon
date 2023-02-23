#pragma once
#include "vector2D.h"
#include "errortype.h"

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
	virtual ErrorType Update(float deltaTime);
	virtual void SetRenderSprite(const wchar_t* imagePath);
	Vector2D GetForwardVector() const;
	virtual void SetActive(bool newActive);
	virtual void Remove();
	bool IsActive() const;
	bool IsRemoved() const;

protected:
	GameObject(); // make it essentially abstract
	Vector2D position;
	float rotationAngle;	

	float renderScale;

	virtual ErrorType Render();

private: 
	GameObjectState state;
	int renderSprite = -1;
};

