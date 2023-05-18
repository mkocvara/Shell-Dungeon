// myinputs.h
// Shell engine version 2020
// Chris Rook
// Modified 08/11/2018
//		Start() now returns an ErrorType, to be consistent with MyDrawEngine

// Modified by 19002175
//		Changed from singleton to normal instanced when implementing ServiceManager.
//		Renamed variables to a style consistent with other work.

#pragma once
#pragma comment(lib, "dinput8.lib")

#include <windows.h>			// Windows headers all-in-one
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include "errortype.h"
#include <memory>


// Class to manage input from keyboard, mouse and joystick
class MyInputs
{
private:
	int mMouseDX;						   // The x-movement of the mouse in the last sample
	int mMouseDY;						   // The y-movement of the mouse in the last sample
	int mMouseDZ;

	bool mMouseLeft;					   // Whether or not left mouse button down in last sample
	bool mMouseRight;					// Whether or not right mouse button down in last sample
	bool mMouseMiddle;					// Whether or not middle mouse button down in last sample

	bool mLastMouseLeft;				// Whether or not left mouse button down in previous sample
	bool mLastMouseRight;				// Whether or not right mouse button down in previous sample
	bool mLastMouseMiddle;				// Whether or not middle mouse button down in previous sample

	DIMOUSESTATE mMousestate;			// DirectINput object to store state of mouse
	static const int KEYMAPSIZE=256;
	char mKeystates[KEYMAPSIZE];			// Array to hold the keyboard state. (Usually 102 keys, but 256 allows
										            //		for future expansion
	char mOldKeystates[KEYMAPSIZE];		// Keystate on previous keyboard sample

	DIJOYSTATE mJoystickState;			      // DirectInput object to store state of joystick
	static LPDIRECTINPUT8 sLpdi;				   // DirectInput interface
	static LPDIRECTINPUTDEVICE8 sLpdiJoystick;	// The dirextInput joystick
	static LPDIRECTINPUTDEVICE8 sLpdiKeyboard;	// The directInput keyboard
	static LPDIRECTINPUTDEVICE8 sLpdiMouse;		// The directInput mouse
	GUID mJoystickGUID;					         // The GUID of the first joystick
	const int mMaxJoystickAxis ;

	static LPDIRECTINPUTEFFECT sLpdiEffectList[3];	// The list of pointers to available effects. Will be NULL if not available.
	static const int PULL=0;
	static const int SHAKE=1;
	static const int CENTRE=2;

	// Releases all directInput objects
	void Release();

	// Callback function to enumerate the force feedback effects.
	static BOOL CALLBACK DIEnumEffectsCallback(LPCDIEFFECTINFO pdei, LPVOID pvRef);

	// Callback function to enumerate the joystick. Currently, I just use the first one found
	static BOOL CALLBACK EnumerateJoystick(LPCDIDEVICEINSTANCE lpddi, LPVOID pDeviceFoundGUID);


public:
	// Constructor. Creates the MyInputs object and connects to a mouse, joystick and keyboard
	// via direct input.
	// The attributes mbKeyboardAvailable , mbMouseAvailable, mbJoystickAvailable will be set
	// to true if each corresponding object can be connected. Otherwise each is false.
	MyInputs(HINSTANCE hinst, HWND hwnd);

	// Destructor. Calls Release().
	~MyInputs();

	// ***** Mouse functions ********************************************************

	// Gets the x-movement of the mouse found on the last sample
	// this is the number of pixels the mouse has moved horizontally.
	// a movement to the right is positive.
	// a movement to the left is negative.
	int GetMouseDX();

	// Gets the y-movement of the mouse found on the last sample
	// This is the number of pixels the mouse has moved vertically.
	// a movement downwards is positive.
	// a movement upwards is negative.
	int GetMouseDY();

	// Gets the Z-movement (wheel) of the mouse found on the last sample
	int GetMouseDZ();

	// Returns true if the left mouse button was down on the last sample.
	// False otherwise
	bool IfMouseLeftDown();

	// Returns true if the right mouse button was down on the last sample.
	// False otherwise
	bool IfMouseRightDown();

	// Returns true if the middle mouse button was down on the last sample.
	// False otherwise
	bool IfMouseMiddleDown();

	// Returns true if the left mouse button was down on the last sample and
	// was not down on the previous sample
	// False otherwise
	bool IfMouseNewLeftDown();

	// Returns true if the right mouse button was down on the last sample and
	// was not down on the previous sample
	// False otherwise
	bool IfMouseNewRightDown();

	// Returns true if the middle mouse button was down on the last sample and
	// was not down on the previous sample
	// False otherwise
	bool IfMouseNewMiddleDown();

	// Returns true if the left mouse button was up on the last sample and
	// was not up on the previous sample
	// False otherwise
	bool IfMouseNewLeftUp();

	// Returns true if the right mouse button was up on the last sample and
	// was not up on the previous sample
	// False otherwise
	bool IfMouseNewRightUp();

	// Returns true if the middle mouse button was up on the last sample and
	// was not up on the previous sample
	// False otherwise
	bool IfMouseNewMiddleUp();

	// Samples the mouse. Records the state of the buttons and the movement made
	// since the previous sample.
	// you should call this each frame, if you plan to use the mouse.
	void SampleMouse();

	// Returns true if the joystick is available and has been initialised.
	bool MouseIsAvailable();

	// ***** Keyboard methods *******************************************************

	// Samples the keyboard. Records the up/down state of all the keys at the time of sampling
	void SampleKeyboard();

	// Returns a pointer to an array describing the state of all keys the at the time
	// of the last keyboard sampling.
	//      The integer values for the array position of each key are stored in a series of constants.
	//		DIK_0		Main keyboard zero
	//		DIK_1		Main keyboard 1
	//			etc for numbers
	//		DIK_MINUS	Minus key
	//		DIK_EQUALS	Equals key
	//		DIK_A		A key
	//		DIK_B		B key
	//		DIK_C		C key
	//			etc for letters
	//		DIK_LSHIFT	Left shift
	//		DIK_RSHIFT	Right shift
	//		DIK_LCONTROL	Left ctrl
	//		DIK_RCONTROL	Right ctrl
	//		DIK_LMENU		Left ALT
	//		DIK_SPACE
	//		DIK_F1		F1 key
	//		DIK_F2		F2 key
	//		DIK_NUMPAD0	Number pad zero
	//		DIK_NUMPAD1	Number pad one
	//		DIK_LEFT	Left arrow
	//		DIK_RIGHT	Right arrow
	//		DIK_UP		Up arrow
	//		DIK_DOWN	Down arrow
	//		.... YOU GET THE IDEA
	char* GetKeyboardState();

	// A method to check if a particular key was pressed at the last sample of the keyboard.
	// Returns:
	//  true if the specified key was pressed at the time the keyboard was last sampled.
	//  false otherwise
	// Parameters:
	//  key: an integer to indicate the key to check.
	//      The integer values for each key are stored in a series of constants.
	//		DIK_0		Main keyboard zero
	//		DIK_1		Main keyboard 1
	//			etc for numbers
	//		DIK_MINUS	Minus key
	//		DIK_EQUALS	Equals key
	//		DIK_A		A key
	//		DIK_B		B key
	//		DIK_C		C key
	//			etc for letters
	//		DIK_LSHIFT	Left shift
	//		DIK_RSHIFT	Right shift
	//		DIK_LCONTROL	Left ctrl
	//		DIK_RCONTROL	Right ctrl
	//		DIK_LMENU		Left ALT
	//		DIK_SPACE
	//		DIK_F1		F1 key
	//		DIK_F2		F2 key
	//		DIK_NUMPAD0	Number pad zero
	//		DIK_NUMPAD1	Number pad one
	//		DIK_LEFT	Left arrow
	//		DIK_RIGHT	Right arrow
	//		DIK_UP		Up arrow
	//		DIK_DOWN	Down arrow
	//		.... YOU GET THE IDEA
	bool KeyPressed(unsigned char key);
	bool NewKeyPressed(unsigned char key);

	// Returns true if the keyboard is available and has been initialised.
	bool KeyboardIsAvailable();

	// ***** Joystick methods *****************************************************

	// Samples the joystick. Records the position of all the axes and buttons
	void SampleJoystick();

	// Returns true if the POV hat is centred (or nonexistent)
	bool IfPOVCentred();

	// Returns the angle of the pov from 0-360.
	// If POV hat is centred, number could be either 655 or 0.
	// Use IfPOVCentred() to check if POV is centred.
	int GetJoystickPOV();

	// Returns the joystick x-axis found on the last sample (from -100 to +100)
	int GetJoystickX();

	// Returns the joystick y-axis found on the last sample (from -100 to +100)
	int GetJoystickY();

	// Returns the joystick z-axis found on the last sample (from -100 to +100)
	int GetJoystickTwist();

	// Returns the joystick throttle-axis (the slider) found on the last sample (from 0 to +100)
	int GetJoystickThrottle();

	// RETURNS true if the button indicated was pressed on the last sample.
	// buttonNumber is from 0 to 31 to indicate which button is being checked.
	// The trigger is usually zero.
	bool IfJoystickButton(int buttonNumber);

	// Returns true if the joystick is available and has been initialised.
	bool JoystickIsAvailable();


	// ***** Force feedback methods ***********************************************

	// Pulls the force feedback joystick in the specified direction for the specified
	// duration with the specified magnitude.
	// Parameters:
	//   direction - the direction of the pull in degrees. 0 = north, 90=east and so on
	//	duraton - how long to pull for, in seconds
	//  magnitude - the strength of the pull as a percentage of full strength
	// Returns:
	//  SUCCESS if the force was applied
	//  FAILURE if the force was not applied
	ErrorType PullJoystick(int direction, double duration, int magnitude=100);

	// Turns on the joystick centering effect for X-axis, y-axis or both
	// Note that joysticks may be configured to have an automatic centering effect.
	// See control panel.
	// The centering effect has no time limit. It can be turned off with
	//  "CentreJoystickOff()" or adjusted by making a new call to this method.
	// Parameters:
	//	Xmagnitude: the strength of the centering effect for the X axis in the range
	//			0-100. Use zero to turn this axis off. 100 gives a very strong
	//			centering effect. 30 is about right for most games.
	//	Ymagnitude: the same for the Y-axis.
	// Returns: SUCCESS if centering was applied.
	//			FAILURE if centering failed, for example if force feedback is not supported.
	ErrorType CentreJoystickOn(int Xmagnitude=50, int Ymagnitude=50);

	// Makes the joystick shake if force feedback is available.
	// Parameters:
	//	duration: the duration of the shake in seconds. (including attack and fade)
	//	magnitude: the strength of the shake effect in %. 0 is no effect. 100 is full effect
	//	frequency: the frequency of the shake effect in Hz. Values above 50 are pretty pointless
	//	attack: the time for the shake effect to build to full strength, in seconds.
	//	fade: the time for the shake effect to fall off, in seconds.
	// Returns: SUCCESS if shake was applied.
	//			FAILURE if shake failed, for example if force feedback is not supported.
	ErrorType PlayShake(double duration=2.0, int magnitude=100, double frequency=15.0, double attack=0.0, double fade =0.0);

	// Turns off centering effect on the joystick. This can also be done by calling
	// "CentreJoystickOn(0,0)", but this may reduce processor load
	ErrorType CentreJoystickOff();
};