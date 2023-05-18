// GameTimer.cpp
// Shell engine version 2020
// Chris Rook
// Last modified 06/09/2019
// Added experimental throttle component to gradually restore frame rate after a dip
//	Intention is to smooth frame rate osscilation

// Modified by 19002175
//		Renamed variables to a style consistent with other work.

#include "GameTimer.h"

GameTimer::GameTimer()
{
	if(QueryPerformanceFrequency(&mFrequency)==0)		// Find the timer frequency
	{
		mFrequency.QuadPart=0;									// Set to zero if function failed

	}

	mGameRate = 1.0;                   // Can adjust this to make the game faster/slower
                                       // Trivial to add functions that let the programmer adjust this.
                                       // I just haven't done it.
	mMinimumFrameTime=0;
	mThrottledFrameTime = mMinimumFrameTime;
	mFrameTime = 0;
}

// Use to set the frameTime. Call this once each frame. 
// The function will also delay until the minumum frame time
// has elapsed since the last call to mark.
// Unless the function "setMinimumFrameTime()" has been called
// the minimum time will be zero, in which case, the mark() function will
// not delay.
// Note that
// the frame time is based on the interval between the last two
// calls to mark(). Until this function has been called twice, it
// will be unreliable.
void GameTimer::mark()
{

	if (mFrequency.QuadPart<=0)		// If frequency is zero (if QueryPerformanceCounter failed)
	{
		mLast.QuadPart=0;			// Set everything to zero. Something is badly wrong.
		mFrameTime=0;
	}
	else
	{
		LARGE_INTEGER now;
		mFrameTime=0.0;						      // Set to zero, ready for loop
		while(mFrameTime<= mThrottledFrameTime)	// This is a loop that causes a delay until minimum frame time has elapsed
                                             // Using "sleep" is just not accurate enough
		{

			QueryPerformanceCounter(&now);		// Get current time

			mFrameTime=(now.QuadPart - mLast.QuadPart)/(double)mFrequency.QuadPart;
											// Find time elapsed since last mark, and divide by
											// frequency to convert to seconds
		}

		mFrameTime = mFrameTime * mGameRate;
		mLast=now;						// Update mark time with current time

		// If the throttle is faster than the game can keep up with
		if (mThrottledFrameTime < mFrameTime)
		{
			// Set the throttle to the current speed
			mThrottledFrameTime = mFrameTime;
		}

		// Gradually ease off the throttling
		mThrottledFrameTime *= 0.5;

		// Don't let throttle go faster than requested speed
		if (mThrottledFrameTime < mMinimumFrameTime)
		{
			mThrottledFrameTime = mMinimumFrameTime;
		}

		// And never slower that 10 fps
		if (mThrottledFrameTime > 0.1)
		{
			mThrottledFrameTime = 0.1;
		}
	}
}

void GameTimer::begin()
{
	mark();
	mark();
}

// Sets the minimum frame time (in seconds). The mark() function will delay until
// the minimum time has elapsed since the last call to mark().
// If not set, the minimum time is zero.
void GameTimer::setMinimumFrameTime(double minTime)
{
	if(minTime>0.0)      // Can't have a negative minimum time
	{
		mMinimumFrameTime = minTime;
	}
	else 
		mMinimumFrameTime=0.0;

	mThrottledFrameTime = mMinimumFrameTime;
}
