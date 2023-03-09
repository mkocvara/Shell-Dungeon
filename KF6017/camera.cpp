// camera.cpp
// Shell engine version 2020
// Chris Rook
// Last modified 20/09/2018

#pragma once

#include "mydrawengine.h"
#include "camera.h"


// Sets the camera with an intial position at 0,0 with 
// a scale to make the screen have a height of 2000 using world coordinates.
// So the top of the screen is 1000. Bottom of the screen is -1000.
// Left and right will depend on screen aspect
Camera::Camera(MyDrawEngine* engine)
{
	m_drawEngine = engine;
	Reset();
}

// Returns the screen coordinates from the given world coordinates
// using the current camera settings
Vector2D Camera::Transform(Vector2D objectPosition) const
{
	objectPosition.YValue = -objectPosition.YValue;
	objectPosition = m_zoom*(objectPosition-m_worldPosition) + m_screenCentre;
	return objectPosition;
}

// Returns the world coordinates from the given screen coordinates
// using the current camera settings
Vector2D Camera::ReverseTransform(Vector2D screenPosition) const
{
	screenPosition = m_worldPosition + (screenPosition - m_screenCentre) / m_zoom;
	screenPosition.YValue = -screenPosition.YValue;
	return screenPosition;
}

// Returns the product of the size and the current scale
float Camera::Transform(float size) const
{
	return size*m_zoom;
}

// Returns a circle on the screen from the given world rectangle
// using the current camera settings
Circle2D Camera::Transform(Circle2D circle) const
{
	return Circle2D(Transform(circle.GetCentre()), Transform(circle.GetRadius()));
}

// Returns a rectangle on the screen from the given world rectangle
// using the current camera settings
Rectangle2D Camera::Transform(Rectangle2D rectangle) const
{
	Rectangle2D answer;
	answer.PlaceAt(Transform(rectangle.GetBottomLeft()), Transform(rectangle.GetTopRight()));
	return answer;
}


// Returns a segment on the screen from the given world rectangle
// using the current camera settings
Segment2D Camera::Transform(Segment2D segment) const
{
	Segment2D answer;
	answer.PlaceAt(Transform(segment.GetStart()), Transform(segment.GetEnd()));
	return answer;
}

// Sets the camera with an intial position at 0,0 with 
// a scale to make the screen have a height of 2000 using world coordinates.
// So the top of the screen is 1000. Bottom of the screen is -1000.
// Left and right will depend on screen aspect
void Camera::Reset()
{
	int height = 0;
	int width = 0;

	if(m_drawEngine->IsStarted())			// Draw engine is initialised
	{
		height = m_drawEngine->GetScreenHeight();
		width = m_drawEngine->GetScreenWidth();
	}
	else					// Use windows instead
	{
		width=GetSystemMetrics(SM_CXSCREEN);
		height = GetSystemMetrics(SM_CYSCREEN);
	}

	m_screenCentre.set(width/2.0f, height/2.0f);
	m_zoom = height/2000.0f;
}

// Places the centre of the camera to the specified position
void Camera::PlaceAt(Vector2D worldPosition)
{
	m_worldPosition = worldPosition;
}

// Sets the camera zoom
void Camera::SetZoom(float zoom)
{
	m_zoom = zoom;
}