#pragma once
#include "Service.h"
#include "vector2D.h"
#include <memory>

class ServiceManager;

class MousePointer : public Service
{
	typedef Service Super;
	typedef int PictureIndex;

public:
	MousePointer(std::shared_ptr<ServiceManager> pServiceManager);
	~MousePointer();

	void Update(double deltaTime);
	void Click();

	void SetPointerEnabled(bool newEnabled);
	void SetPointerVisible(bool newVisibile);
	void SetPointerSpeed(float newSpeed);

	Vector2D GetPosition() const;
	PictureIndex GetPointerSprite() const;

protected:
	virtual void SetRenderSprite(const wchar_t* imagePath);

private:
	Vector2D mScreenPosition; // = (0, 0)
	Vector2D mWorldPosition; // = (0, 0)
	float mScale = 1.f;
	float mRotationAngle = 0.f;

	bool mPointerEnabled = true;
	bool mPointerVisible = true;
	float mPointerSpeed = 1.f;

	PictureIndex mRenderSprite = -1;

	std::weak_ptr<ServiceManager> mpServiceManager;

	const wchar_t* mDefaultRenderSpritePath = L"Assets/Engine/crosshair.png";
};

