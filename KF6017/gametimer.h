// GameTimer.h
// Shell engine version 2023
// Chris Rook
// Last modified 06/09/2019
// Added experimental throttle component to gradually restore frame rate after a dip
//	Intention is to smooth frame rate osscilation


#pragma once

#include <windows.h>

// This class will allow you to measure the frame time.
// Can also be used to measure any other short intervals.
class GameTimer
{
private:
	LARGE_INTEGER freq;		// Stores the frequency of the performance counter ("ticks" per second)
	LARGE_INTEGER last;		// Stores the time of the last mark time (in ticks)
	double mdMinimumFrameTime;	// The minumim frame time that mark() will allow
	double mdThrottledFrameTime;	// The minimum frame time currently allowed. This will gradually reset
									// to the minimum frame time to smooth frame rate changes.
				

public:
	double mdFrameTime;		// The duration between the last two uses of the mark() function in seconds
	double mdGameRate;		// A number to set the "speed" of your game. Keep at 1.0 for normal rates
							      // Set to lower values for slow-time. Set to higher values for speedy effects.
							      // This number is in effect a modifier to your FrameTime. mdFrameTime will
							      // be multiplied by mdGameRate when you call "mark()".
							      // If you set mdGameRate to 0.5, mdFrameTime will record the duration
							      // of each frame as half of its actual duration. As a result your
							      // physics engine will make everything move at half speed.

	// Constructor
	GameTimer();

		// Sets the minimum frame time (in seconds). The mark() function will delay until
		// the minimum time has elapsed since the last call to mark().
		// If not set, the minimum time is zero.
	void setMinimumFrameTime(double minTime);
							
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
	void mark();			
	
	// Starts the timer. Should be called before using mark().
	void begin();
};
