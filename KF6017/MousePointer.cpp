/*
* MousePointer.cpp
* 19002175
*
* Service for tracking and displaying the mouse pointer. Dispatches click events on click.
*/

#include "MousePointer.h"
#include <algorithm>

#include "ServiceManager.h"
#include "myinputs.h"
#include "mydrawengine.h"
#include "EventManager.h"


// PUBLIC

MousePointer::MousePointer(std::shared_ptr<ServiceManager> pServiceManager)
	: mpServiceManager(pServiceManager)
{
	SetRenderSprite(mDefaultRenderSpritePath);
}

MousePointer::~MousePointer()
{
}

void MousePointer::Update(double deltaTime)
{
	const std::shared_ptr<ServiceManager> pServiceManagerLocked = mpServiceManager.lock();
	const std::shared_ptr<MyInputs> pInputs = pServiceManagerLocked->GetInputs().lock();

	pInputs->SampleMouse();

	if (!mPointerEnabled)
		return;

	const std::shared_ptr<MyDrawEngine> pDrawEngine = pServiceManagerLocked->GetDrawEngine().lock();

	const Rectangle2D viewport = pDrawEngine->GetViewport();
	
	const int screenWidth = (int)viewport.GetWidth();
	const int screenHeight = (int)viewport.GetHeight();

	const int maxX = screenWidth / 2;
	const int minX = -screenWidth / 2;
	const int maxY = screenHeight / 2;
	const int minY = -screenHeight / 2;

	mScreenPosition.XValue = (float)std::clamp((int)(mScreenPosition.XValue + pInputs->GetMouseDX() * mPointerSpeed), minX, maxX);
	mScreenPosition.YValue = (float)std::clamp((int)(mScreenPosition.YValue - pInputs->GetMouseDY() * mPointerSpeed), minY, maxY);

	if (pInputs->IfMouseNewLeftDown())
		Click();
}

void MousePointer::Render()
{
	if (mPointerEnabled && mPointerVisible)
	{
		const std::shared_ptr<MyDrawEngine> pDrawEngine = mpServiceManager.lock()->GetDrawEngine().lock();
		const Rectangle2D viewport = pDrawEngine->GetViewport();
		mWorldPosition = viewport.GetCentre() + mScreenPosition;
		pDrawEngine->DrawAt(mWorldPosition, GetPointerSprite(), mScale, mRotationAngle);
	}
}

void MousePointer::Click()
{
	const std::shared_ptr<ServiceManager> pServiceManagerLocked = mpServiceManager.lock();
	const std::shared_ptr<EventManager> pEventManager = pServiceManagerLocked->GetEventManager().lock();

	Event e(EventType::mouseClick, mWorldPosition);
	pEventManager->DispatchEvent(e);
}

void MousePointer::SetPointerEnabled(bool newEnabled)
{
	mPointerEnabled = newEnabled;
}

void MousePointer::SetPointerVisible(bool newVisible)
{
	mPointerVisible = newVisible;
}

void MousePointer::SetPointerSpeed(float newSpeed)
{
	mPointerSpeed = newSpeed;
}

Vector2D MousePointer::GetPosition() const
{
	return mWorldPosition;
}

PictureIndex MousePointer::GetPointerSprite() const
{
	return mRenderSprite;
}


// PROTECTED

void MousePointer::SetRenderSprite(const wchar_t* imagePath)
{
	const std::shared_ptr<MyDrawEngine> pDrawEngine = mpServiceManager.lock()->GetDrawEngine().lock();
	mRenderSprite = pDrawEngine->LoadPicture(imagePath);
}