// myinputs.cpp
// Shell engine version 2020
// Chris Rook
// Last modified 20/09/2018

#include "myinputs.h"
#include "errorlogger.h"

// *************************************************************************************
// Implementation of the global EnumerateJoystick function 

LPDIRECTINPUTDEVICE8 MyInputs::sLpdiJoystick=nullptr;	// The dirextInput joystick
LPDIRECTINPUTDEVICE8 MyInputs::sLpdiKeyboard=nullptr;	// The directInput keyboard
LPDIRECTINPUTDEVICE8 MyInputs::sLpdiMouse=nullptr;		// The directInput mouse	
LPDIRECTINPUT8 MyInputs::sLpdi=nullptr;	
LPDIRECTINPUTEFFECT MyInputs::sLpdiEffectList[3];

BOOL CALLBACK MyInputs::EnumerateJoystick(LPCDIDEVICEINSTANCE lpddi, LPVOID pDeviceFoundGUID)
// Nothing to see here, folks. Move along. Move along.
{
	HRESULT err;

    // Obtain an interface to the enumerated joystick.
    err = sLpdi->CreateDevice(lpddi->guidInstance,  
                                &sLpdiJoystick, NULL);

   if(FAILED(err)) 
   {
	   ErrorLogger::Write(L"Failed to create interface for joystick\n");
	   ErrorLogger::Writeln(ERRORSTRING(err));
        return DIENUM_CONTINUE;
   }

    return DIENUM_STOP;
}

// *************************************************************************************
// Implementation of the MyInputs member functions

MyInputs::MyInputs(HINSTANCE hinst, HWND hwnd)
:mMaxJoystickAxis(100)
{
	sLpdi=nullptr;

	sLpdiKeyboard=nullptr;
	sLpdiMouse=nullptr;
	sLpdiJoystick=nullptr;

	mMouseDX=0;
	mMouseDY=0;
	mMouseLeft=false;
	mMouseRight = false;
	mMouseMiddle = false;

	sLpdiEffectList[PULL]=nullptr;
	sLpdiEffectList[SHAKE]=nullptr;
	sLpdiEffectList[CENTRE]=nullptr;

	
	HRESULT err =DirectInput8Create(hinst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&sLpdi, NULL);
	if (FAILED(err))
	{
		// Failed to get DInput.
		ErrorLogger::Writeln(L"Failed to create direct input.\n");
		ErrorLogger::Writeln(ERRORSTRING(err));

		return;
	}

	// Direct input is set up - handle the keyboard next ********************
	err=sLpdi->CreateDevice(GUID_SysKeyboard, &sLpdiKeyboard, NULL);
	if (FAILED(err))
	{
		// Failed to get a keyboard!
		ErrorLogger::Writeln(L"Failed to find a keyboard\n");
		ErrorLogger::Writeln(ERRORSTRING(err));
	}

	// Setting format for the device to be a default keyboard
	if(sLpdiKeyboard)
	{
		err=sLpdiKeyboard->SetDataFormat(&c_dfDIKeyboard);
		if (FAILED(err))
		{
			// Failed to set format
			ErrorLogger::Writeln(L"Failed to set keyboard format\n");
			ErrorLogger::Writeln(ERRORSTRING(err));
			sLpdiKeyboard->Release();
			sLpdiKeyboard=nullptr;
		}
	}

	// Setting cooperation level to BACKGROUND (gets input whether on top or not)
	//   and EXCLUSIVE (let other programs use the keyboard, too)
	if(sLpdiKeyboard)
	{
		err=sLpdiKeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
		if (FAILED(err))
		{
			// Failed to set cooperative level
			ErrorLogger::Writeln(L"Failed to set keyboard cooperative level");
			ErrorLogger::Writeln(ERRORSTRING(err));
			sLpdiKeyboard->Release();
			sLpdiKeyboard=nullptr;
		}
	}
	// Acquire keyboard
	if(sLpdiKeyboard)
	{
		err=sLpdiKeyboard->Acquire();
		if (FAILED(err))
		{
			// Failed to set cooperative level
			ErrorLogger::Writeln(L"Failed to acquire keyboard on init!");
			ErrorLogger::Writeln(ERRORSTRING(err));

			// Commenting out - Don't release the keyboard, just try to reacquire next time around
			//sLpdiKeyboard->Release();
			//sLpdiKeyboard=NULL;
		}
	}

	// Set all of the keyboard array to zero
	memset(mKeystates, 0, KEYMAPSIZE);
	memset(mOldKeystates, 0, KEYMAPSIZE);

	// Keyboard ready. Next is the mouse **************************************
	err=sLpdi->CreateDevice(GUID_SysMouse, &sLpdiMouse, NULL);
	if (FAILED(err))
	{
		// Failed to get a mouse
		ErrorLogger::Writeln(L"Failed to find a mouse");
		ErrorLogger::Writeln(ERRORSTRING(err));
	}

	// Setting cooperation level to FOREGROUND (only gets input when on top)
	//   and EXCLUSIVE (let other programs use the mouse, too)
	if(sLpdiMouse)
	{
		err=sLpdiMouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND|DISCL_EXCLUSIVE);
		if (FAILED(err))
		{
			// Failed to set cooperative level
			ErrorLogger::Writeln(L"Failed to set mouse cooperative level");
			ErrorLogger::Writeln(ERRORSTRING(err));
			sLpdiMouse->Release();
			sLpdiMouse=nullptr;
		}
	}

	// Setting format for the device to be a default mouse
	if(sLpdiMouse)
	{
		err=sLpdiMouse->SetDataFormat(&c_dfDIMouse);
		if (FAILED(err))
		{
			// Failed to set format
			ErrorLogger::Writeln(L"Failed to set mouse format");
			ErrorLogger::Writeln(ERRORSTRING(err));
			sLpdiMouse->Release();
			sLpdiMouse=nullptr;
		}
	}

	// Aquire mouse
	if(sLpdiMouse)
	{
		err=sLpdiMouse->Acquire();
		if (FAILED(err))
		{
			// Failed to set cooperative level
			ErrorLogger::Writeln(L"Failed to acquire mouse");
			ErrorLogger::Writeln(ERRORSTRING(err));
			sLpdiMouse->Release();
			sLpdiMouse=nullptr;

		}
	}

	// Mouse is armed and hungry. Next try the joystick

	// First need to enumerate the joysticks and just claim the first one connected
	// (a bit simple - alternative is to give users a choice from a list, but too much hassle.)

	// Enumerate the attached joystick devices. This will repeatedly call my "EnumerateJoysticks()
	//  function, and will put the joystick address in sLpdiJoystick.

	err=sLpdi->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumerateJoystick, NULL, DIEDFL_ATTACHEDONLY);
	if (FAILED(err))
	{
		ErrorLogger::Writeln(L"Failed to enumerate joystick devices.");
		ErrorLogger::Writeln(ERRORSTRING(err));
	}

	// Set the cooperative level - want exclusive access

	if(sLpdiJoystick)
	{
		err=sLpdiJoystick->SetCooperativeLevel(hwnd, DISCL_BACKGROUND | DISCL_EXCLUSIVE);
		if(FAILED(err))
		{
			// Could not set cooperative level
			sLpdiJoystick->Release();
			sLpdiJoystick=nullptr;
			ErrorLogger::Writeln(L"Could not set joystick cooperative level\n");
			ErrorLogger::Writeln(ERRORSTRING(err));
		}
	}

	// Set the data format for the joystick

	if(sLpdiJoystick)
	{
		err=sLpdiJoystick->SetDataFormat(&c_dfDIJoystick);
		if(FAILED(err))
		{
			// Could not set data format
			sLpdiJoystick->Release();
			sLpdiJoystick=nullptr;
			ErrorLogger::Writeln(L"Could not set joystick data format");
			ErrorLogger::Writeln(ERRORSTRING(err));
		}
	}

	if(sLpdiJoystick)
	{
		// Set joystick ranges - start with x axis
    // No error reporting here. Any problems are probably just because it does not have the specified axis

		DIPROPRANGE joystickAxisRange;		// A struct to store the details of the range you want to set

		joystickAxisRange.lMax = mMaxJoystickAxis;				// Set max range to the constant
		joystickAxisRange.lMin = -mMaxJoystickAxis;				// Same for min range
		joystickAxisRange.diph.dwSize=sizeof(DIPROPRANGE);			// Tell it its own size
		joystickAxisRange.diph.dwHeaderSize=sizeof(DIPROPHEADER);	// Tell it the size of its header
		joystickAxisRange.diph.dwObj = DIJOFS_X;					// Setting the x-axis
		joystickAxisRange.diph.dwHow = DIPH_BYOFFSET;				// Use offset (as always) for getting data

		sLpdiJoystick->SetProperty(DIPROP_RANGE, &joystickAxisRange.diph);

		// Y-axis next
		joystickAxisRange.lMax = mMaxJoystickAxis;				// Set max range to the constant
		joystickAxisRange.lMin = -mMaxJoystickAxis;				// Same for min range
		joystickAxisRange.diph.dwSize=sizeof(DIPROPRANGE);			// Tell it its own size
		joystickAxisRange.diph.dwHeaderSize=sizeof(DIPROPHEADER);	// Tell it the size of its header
		joystickAxisRange.diph.dwObj = DIJOFS_Y;					// Setting the y-axis
		joystickAxisRange.diph.dwHow = DIPH_BYOFFSET;				// Use offset (as always) for getting data

		sLpdiJoystick->SetProperty(DIPROP_RANGE, &joystickAxisRange.diph);

		// twist-axis
		joystickAxisRange.lMax = mMaxJoystickAxis;				// Set max range to the constant
		joystickAxisRange.lMin = -mMaxJoystickAxis;				// Same for min range
		joystickAxisRange.diph.dwSize=sizeof(DIPROPRANGE);			// Tell it its own size
		joystickAxisRange.diph.dwHeaderSize=sizeof(DIPROPHEADER);	// Tell it the size of its header
		joystickAxisRange.diph.dwObj = DIJOFS_RZ;					// Setting the z-axis
		joystickAxisRange.diph.dwHow = DIPH_BYOFFSET;				// Use offset (as always) for getting data
		
		sLpdiJoystick->SetProperty(DIPROP_RANGE, &joystickAxisRange.diph);

		// Finally, throttle-axis
		joystickAxisRange.lMax = mMaxJoystickAxis;				// Set max range to the constant
		joystickAxisRange.lMin = 0;				// Same for min range
		joystickAxisRange.diph.dwSize=sizeof(DIPROPRANGE);			// Tell it its own size
		joystickAxisRange.diph.dwHeaderSize=sizeof(DIPROPHEADER);	// Tell it the size of its header
		joystickAxisRange.diph.dwObj = DIJOFS_SLIDER(0);					// Setting the z-axis
		joystickAxisRange.diph.dwHow = DIPH_BYOFFSET;				// Use offset (as always)
		
		err=sLpdiJoystick->SetProperty(DIPROP_RANGE, &joystickAxisRange.diph);
	}

   // If, after all of that, we don't have a pointer to the joystick,
   // then it probably is simply not there
	if(sLpdiJoystick)
	{
		// Finally, acquire the joystick
		if(FAILED(sLpdiJoystick->Acquire()))
		{
            // If that failed, release everything
			sLpdiJoystick->Release();
			sLpdiJoystick=nullptr;
			ErrorLogger::Writeln(L"Failed to aquire joystick");
			ErrorLogger::Writeln(ERRORSTRING(err));
		}
	}

	// Enumerate the effects
	if(sLpdiJoystick)
		err = sLpdiJoystick->EnumEffects(&(this->DIEnumEffectsCallback), sLpdiJoystick, DIEFT_ALL);
}


MyInputs::~MyInputs()
{
	// Need to stop all effects
	// -> Stop does not seem to work
	PullJoystick(0, .1, 0);
	if (sLpdiEffectList[SHAKE])
	{
		sLpdiEffectList[SHAKE]->Stop();
		sLpdiEffectList[SHAKE]->Release();
	}
	if (sLpdiEffectList[PULL])
	{
		sLpdiEffectList[PULL]->Stop();
		sLpdiEffectList[PULL]->Release();
	}
	if (sLpdiEffectList[CENTRE])
	{
		sLpdiEffectList[CENTRE]->Stop();
		sLpdiEffectList[CENTRE]->Release();
	}

	Release();
}

void MyInputs::Release()
{
	// Release all objects
	if (sLpdiMouse)
	{
		sLpdiMouse->Release();
		sLpdi=nullptr;
	}
	if (sLpdiKeyboard)
	{
		sLpdiKeyboard->Release();
		sLpdiKeyboard=nullptr;
	}

	if (sLpdiJoystick)
	{
		sLpdiJoystick->Release();
		sLpdiJoystick=nullptr;
	}

	if (sLpdi) 
	{
		sLpdi->Release();
		sLpdi=nullptr;
	}
	mJoystickGUID.Data1=NULL;		// Set joystick guid back to null
}




void MyInputs::SampleMouse()
{
	if(!sLpdiMouse) 
	{
		ErrorLogger::Writeln(L"No mouse - cannot sample.");	
		return;
	}

	// Get state of mouse from DInput
	HRESULT err=sLpdiMouse->GetDeviceState(sizeof(mMousestate), (LPVOID) &mMousestate);
	if(FAILED(err))
	{	
		ErrorLogger::Writeln(L"Failed to get mouse state.");
		ErrorLogger::Writeln(ERRORSTRING(err));
		if (err==DIERR_INPUTLOST || err==DIERR_NOTACQUIRED)
		{
			ErrorLogger::Write(L"Attempting to reacquire....");
			sLpdiMouse->Acquire();
			err=sLpdiMouse->GetDeviceState(sizeof(mMousestate), (LPVOID) &mMousestate);
			if(FAILED(err))
				ErrorLogger::Writeln(L"Failed");
			else
				ErrorLogger::Writeln(L"Success");
		}
	}

	// Record mouse movements in member variables
	mMouseDX=mMousestate.lX;
	mMouseDY=mMousestate.lY;
	mMouseDZ=mMousestate.lZ;

	// record state of previous sample
	mLastMouseLeft = mMouseLeft;
	mLastMouseRight = mMouseRight;
	mLastMouseMiddle = mMouseMiddle;

	// See which buttons were down and set accordingly
	if(mMousestate.rgbButtons[0] & 0x80)
	{
		mMouseLeft = true;
	}
	else
	{
		mMouseLeft = false;
	}

	if(mMousestate.rgbButtons[1] & 0x80)
	{
		mMouseRight = true;
	}
	else
	{
		mMouseRight = false;
	}

	if(mMousestate.rgbButtons[2] & 0x80)
	{
		mMouseMiddle = true;
	}
	else
	{
		mMouseMiddle = false;
	}


}

int MyInputs::GetMouseDX()
{
	return mMouseDX;
}

int MyInputs::GetMouseDY()
{
	return mMouseDY;
}

int MyInputs::GetMouseDZ()
{
	return mMouseDZ;
}

bool MyInputs::IfMouseNewLeftDown()
{
	return(mMouseLeft && !mLastMouseLeft);
}

bool MyInputs::IfMouseNewMiddleDown()
{
	return(mMouseMiddle && !mLastMouseMiddle);
}

bool MyInputs::IfMouseNewRightDown()
{
	return(mMouseRight && !mLastMouseRight);
}

bool MyInputs::IfMouseNewLeftUp()
{
	return(!mMouseLeft && mLastMouseLeft);
}

bool MyInputs::IfMouseNewRightUp()
{
	return(!mMouseRight && mLastMouseRight);
}

bool MyInputs::IfMouseNewMiddleUp()
{
	return(!mMouseMiddle && mLastMouseMiddle);
}

bool MyInputs::IfMouseLeftDown()
{
	return mMouseLeft;
}

bool MyInputs::IfMouseRightDown()
{
	return mMouseRight;
}

bool MyInputs::IfMouseMiddleDown()
{
	return mMouseMiddle;
}


char* MyInputs::GetKeyboardState()
{
	return mKeystates;
}

bool MyInputs::KeyPressed(unsigned char key)
{
	return ((mKeystates[key]&0x80)>0);	// Return true if 0x80 bit set (set if key was pressed)
}

bool MyInputs::NewKeyPressed(unsigned char key)
{
	bool kp = (mKeystates[key]&0x80)>0;
	bool okp = (mOldKeystates[key]&0x80)>0;

	return (kp && !okp);	// Return true if 0x80 bit set (set if key was pressed)
}


void MyInputs::SampleKeyboard()
{
	if(sLpdiKeyboard)			// Can't sample keyboard if it was not created
	{
		memcpy(mOldKeystates, mKeystates, KEYMAPSIZE);
		HRESULT err=sLpdiKeyboard->GetDeviceState(KEYMAPSIZE, &mKeystates);
		if(FAILED(err))
		{
			ErrorLogger::Writeln(L"Failed to get keyboard state.");
			ErrorLogger::Writeln(ERRORSTRING(err));
			if (err==DIERR_INPUTLOST||err==DIERR_NOTACQUIRED)
			{
				ErrorLogger::Write(L"Attempting to reacquire....");
				sLpdiKeyboard->Acquire();
				err=sLpdiKeyboard->GetDeviceState(sizeof(mMousestate), (LPVOID) &mMousestate);
				if(FAILED(err))
					ErrorLogger::Writeln(L"Failed");
				else
					ErrorLogger::Writeln(L"Success");
			}
		}

	}
}


void MyInputs::SampleJoystick()
{
	if(sLpdiJoystick)			// Can't sample joystick if it was not created
	{
		HRESULT err=sLpdiJoystick->Poll();
		if(FAILED(err))
		{
			ErrorLogger::Writeln(L"Could not poll joystick");
			ErrorLogger::Writeln(ERRORSTRING(err));
			if(err==DIERR_INPUTLOST || err==DIERR_NOTACQUIRED)
		
			{
				err=sLpdiJoystick->Acquire();			// Reacquire the keyboard
				ErrorLogger::Write(L"Attempting to reacquire...");
				if(FAILED(err))
					ErrorLogger::Writeln(L"Failed");
				else
				{
					ErrorLogger::Writeln(L"Success");
					sLpdiJoystick->Poll();
				}
			}
		}

		err=sLpdiJoystick->GetDeviceState(sizeof(mJoystickState), &mJoystickState);
		if(FAILED(err))
		{
			ErrorLogger::Writeln(L"Could not get joystick state.");
			ErrorLogger::Writeln(ERRORSTRING(err));
			if(err==DIERR_INPUTLOST || err==DIERR_NOTACQUIRED)
			{
				err=sLpdiJoystick->Acquire();			// Reacquire the joystick
				ErrorLogger::Write(L"Attempting to reacquire...");
				if(FAILED(err))
					ErrorLogger::Writeln(L"Failed");
				else
				{
					ErrorLogger::Writeln(L"Success");
					sLpdiJoystick->GetDeviceState(sizeof(mJoystickState), &mJoystickState);
				}
			}
		}

	}

}

int MyInputs::GetJoystickX()
{
	return mJoystickState.lX;
}

int MyInputs::GetJoystickY()
{
	return mJoystickState.lY;
}

int MyInputs::GetJoystickTwist()
{
	return mJoystickState.lRz;
}

int MyInputs::GetJoystickThrottle()
{
	return 100-mJoystickState.rglSlider[0];
}


bool MyInputs::IfPOVCentred()
{
	return (LOWORD(mJoystickState.rgdwPOV[0]) == 0xFFFF);
}

int MyInputs::GetJoystickPOV()
{
	return mJoystickState.rgdwPOV[0]/100;
}

bool MyInputs::IfJoystickButton(int buttonNumber)
{
	if (buttonNumber<0||buttonNumber>31)
		return false;

	if((mJoystickState.rgbButtons[buttonNumber]&0x80)>0)
	{
		return true;
	}

	return false;
}

bool MyInputs::JoystickIsAvailable()
{
	return (sLpdiJoystick!=nullptr);
}


bool MyInputs::MouseIsAvailable()
{
	return (sLpdiMouse!=nullptr);
}

bool MyInputs::KeyboardIsAvailable()
{
	return (sLpdiKeyboard!=nullptr);
}


BOOL CALLBACK MyInputs::DIEnumEffectsCallback(LPCDIEFFECTINFO pdei, LPVOID pvRef)
{
	// Not going to enumerate properly.
	// If any effect is found for the right joystick, just
	// create four standard effects and stop enumerating.
	// This is all a bit of a mess - I am setting stuff here that I just change when the
	// effects are called. It works, though.
	
    HRESULT  err;
    if((LPDIRECTINPUTDEVICE8)pvRef!= sLpdiJoystick)		// Wrong joystick
		return DIENUM_CONTINUE;							// Next effect, please

    DIEFFECT             diEffect;						// Params for created effect
	ZeroMemory(&diEffect, sizeof(diEffect));
   
	// Create the constant force

	DWORD    dwAxes[2] = { DIJOFS_X, DIJOFS_Y };	// Indicate which axes will be used
	LONG     lDirection[2] = { 18000, 90000 };				// Specify directions of axes

	// Create the constant force effect. ***********************************

	DICONSTANTFORCE      diConstantForce;

	diConstantForce.lMagnitude = DI_FFNOMINALMAX;   // Full force

	diEffect.dwSize          = sizeof(DIEFFECT); 
	diEffect.dwFlags         = DIEFF_POLAR | DIEFF_OBJECTOFFSETS;	// Using polar coordinates
	diEffect.dwDuration      = (DWORD)(1.0 * DI_SECONDS);			// 1-second pull
	diEffect.dwSamplePeriod  = 0;						// = default 
	diEffect.dwGain          = DI_FFNOMINALMAX;			// No scaling
	diEffect.dwTriggerButton = DIEB_NOTRIGGER;			// Not a button response
	diEffect.dwTriggerRepeatInterval = 0;				// Not applicable
	diEffect.cAxes                   = 2;				// Both axes involved
	diEffect.rgdwAxes                = &dwAxes[0]; 
	diEffect.rglDirection            = &lDirection[0]; 
	diEffect.lpEnvelope              = nullptr;			// No envelope used
	diEffect.cbTypeSpecificParams    = sizeof(DICONSTANTFORCE);	// Constant force-specific data
	diEffect.lpvTypeSpecificParams   = &diConstantForce;

	if(sLpdiEffectList[PULL])							// If already there
		sLpdiEffectList[PULL]->Release();

	err = sLpdiJoystick->CreateEffect(pdei->guid,		// Create it
                            &diEffect,
							&sLpdiEffectList[PULL],	// Pass by reference pointer to created effect (out)
                            NULL);

	if(FAILED(err))
	{
		ErrorLogger::Writeln(L"Failed to create a constant force effect.");
		ErrorLogger::Writeln(ERRORSTRING(err));
		sLpdiEffectList[PULL]=nullptr;
	}
 
	// Set up the shake effect ***************************************

	DIPERIODIC diPeriodic;      // type-specific parameters
	DIENVELOPE diEnvelope;      // envelope

	ZeroMemory(&diEffect, sizeof(diEffect));
	ZeroMemory(&diPeriodic, sizeof(diPeriodic));
	ZeroMemory(&diEnvelope, sizeof(diEnvelope));

	// Shake details
	diPeriodic.dwMagnitude = DI_FFNOMINALMAX; 
	diPeriodic.lOffset = 0; 
	diPeriodic.dwPhase = 0; 
	diPeriodic.dwPeriod = (DWORD)(0.05 * DI_SECONDS);

	// Envelope
	diEnvelope.dwSize = sizeof(DIENVELOPE);
	diEnvelope.dwAttackLevel = 0; 
	diEnvelope.dwAttackTime = (DWORD)(0.5 * DI_SECONDS); 
	diEnvelope.dwFadeLevel = 0; 
	diEnvelope.dwFadeTime = (DWORD)(1.0 * DI_SECONDS);
	
	// Basic effects

	diEffect.dwSize = sizeof(DIEFFECT); 
	diEffect.dwFlags = DIEFF_POLAR | DIEFF_OBJECTOFFSETS; 
	diEffect.dwDuration = (DWORD)(2 * DI_SECONDS);
	 
	diEffect.dwSamplePeriod = 0;               // = default 
	diEffect.dwGain = DI_FFNOMINALMAX;         // no scaling
	diEffect.dwTriggerButton = DIJOFS_BUTTON0;
	diEffect.dwTriggerRepeatInterval = 0;      
	diEffect.cAxes = 2; 
	diEffect.rgdwAxes = &dwAxes[0]; 
	diEffect.rglDirection = &lDirection[0]; 
	diEffect.lpEnvelope = &diEnvelope; 
	diEffect.cbTypeSpecificParams = sizeof(diPeriodic);
	diEffect.lpvTypeSpecificParams = &diPeriodic;

	err = sLpdiJoystick->CreateEffect(GUID_Sine,		// Create it
					&diEffect,
					&sLpdiEffectList[SHAKE],		// Pass by reference pointer to created effect (out)
                    NULL);

	if(FAILED(err))
	{
		ErrorLogger::Writeln(L"Failed to create a shake effect.");
		ErrorLogger::Writeln(ERRORSTRING(err));
		sLpdiEffectList[SHAKE]=nullptr;
	}

	// Set up the centering effect *********************************************

	DICONDITION diCondition[2];				// To control the strength of the centering
	
	diCondition[0].dwNegativeSaturation =10000;
	diCondition[0].lOffset =0;
	diCondition[0].dwPositiveSaturation=10000;
	diCondition[0].lNegativeCoefficient=10000;
	diCondition[0].lPositiveCoefficient=10000;
	diCondition[0].lDeadBand=0;

	diCondition[1].dwNegativeSaturation =10000;
	diCondition[1].lOffset =0;
	diCondition[1].dwPositiveSaturation=10000;
	diCondition[1].lNegativeCoefficient=10000;
	diCondition[1].lPositiveCoefficient=10000;
	diCondition[1].lDeadBand=0;

	ZeroMemory(&diEffect, sizeof(diEffect));
	diEffect.dwSize=sizeof(diEffect);
	diEffect.dwGain=10000;
	diEffect.dwDuration=INFINITE;		// Infinite?
	diEffect.dwSamplePeriod=0;
	diEffect.rglDirection=&lDirection[0];
	diEffect.dwFlags         = DIEFF_POLAR | DIEFF_OBJECTOFFSETS;
	diEffect.dwTriggerButton = DIEB_NOTRIGGER;						// Not a button response
	diEffect.cAxes                   = 2;							// Both axes involved
	diEffect.rgdwAxes                = &dwAxes[0];					// X-axis
	diEffect.lpEnvelope              = nullptr;							// No envelope used
	diEffect.cbTypeSpecificParams    = sizeof(DICONDITION)*2;			// Constant force-specific data
	diEffect.lpvTypeSpecificParams   = &diCondition[0];

	err = sLpdiJoystick->CreateEffect(GUID_Spring,		// Create it
					&diEffect,
					&sLpdiEffectList[CENTRE],		// Pass by reference pointer to created effect (out)
                    NULL);

	if(FAILED(err))
	{
		ErrorLogger::Writeln(L"Failed to create a centre effect.");
		ErrorLogger::Writeln(ERRORSTRING(err));
		sLpdiEffectList[CENTRE]=nullptr;
	}

    return DIENUM_STOP;	// As long as basic effects are found, this is OK

}  // End of callback

ErrorType MyInputs::PullJoystick(int direction, double duration, int magnitude)
{
	if(!sLpdiEffectList[PULL])
		return FAILURE;

	direction = (direction+180)%360;

	DICONSTANTFORCE diConstantForce;
	ZeroMemory(&diConstantForce, sizeof(diConstantForce));

	LONG rgDirection[2]={direction*100,0};
	DWORD    dwAxes[2] = { DIJOFS_X, DIJOFS_Y };	// Indicate which axes will be used

	DIEFFECT eff;
	ZeroMemory(&eff, sizeof(eff));
	eff.dwSize=sizeof(eff);
	eff.dwGain=magnitude*100;
	eff.dwDuration=(int)(duration * DI_SECONDS);
	eff.rglDirection=rgDirection;
	eff.lpvTypeSpecificParams = &diConstantForce;
	eff.dwFlags         = DIEFF_POLAR | DIEFF_OBJECTOFFSETS;	// Using polar coordinates
	eff.dwTriggerButton = DIEB_NOTRIGGER;			// Not a button response
	eff.cAxes                   = 2;				// Both axes involved
	eff.rgdwAxes                = &dwAxes[0]; 
	eff.lpEnvelope              = nullptr;			// No envelope used
	eff.cbTypeSpecificParams    = sizeof(DICONSTANTFORCE);	// Constant force-specific data
	eff.lpvTypeSpecificParams   = &diConstantForce;

	HRESULT err = sLpdiEffectList[PULL]->SetParameters(&eff ,  DIEP_DURATION|DIEP_GAIN|DIEP_DIRECTION);
	if(FAILED(err))
	{
		ErrorLogger::Writeln(L"Could not change pull effect");
		ErrorLogger::Writeln(ERRORSTRING(err));
		return FAILURE;
	}
	else
	{
		err=sLpdiEffectList[PULL]->Start(1,0);
		if(FAILED(err))
		{
			ErrorLogger::Writeln(L"Could not play pull effect");
			ErrorLogger::Writeln(ERRORSTRING(err));	
			return FAILURE;
		}
		return SUCCESS;
	}
}


ErrorType MyInputs::PlayShake(double duration, int magnitude, double frequency, double attack, double fade)
{
	HRESULT err;
	if(!sLpdiEffectList[SHAKE])		// If effect not present
		return FAILURE;

	DIPERIODIC diPeriodic;				// To control the frequency of the shake
	ZeroMemory(&diPeriodic, sizeof(diPeriodic));

	diPeriodic.dwMagnitude = DI_FFNOMINALMAX; 
	diPeriodic.lOffset = 0; 
	diPeriodic.dwPhase = 0; 
	diPeriodic.dwPeriod = (DWORD)(1.0/frequency * DI_SECONDS);	// Converting to time period

		// Envelope controls the attack and decay of the effect
	DIENVELOPE diEnvelope;
	ZeroMemory(&diEnvelope, sizeof(diEnvelope));

	diEnvelope.dwSize = sizeof(DIENVELOPE);
	diEnvelope.dwAttackLevel = 0; 
	diEnvelope.dwAttackTime = (DWORD)(attack * DI_SECONDS);	// Time to build the effect
	diEnvelope.dwFadeLevel = 0; 
	diEnvelope.dwFadeTime = (DWORD)(fade * DI_SECONDS);		// Time to fade the effect

	LONG rgDirection[2]={9000,0};
	DWORD    dwAxes[2] = { DIJOFS_X, DIJOFS_Y };	// Indicate which axes will be used

	DIEFFECT eff;
	ZeroMemory(&eff, sizeof(eff));
	eff.dwSize=sizeof(eff);
	eff.dwGain=magnitude*100;
	eff.dwDuration=(int)(duration * DI_SECONDS);
	eff.rglDirection=rgDirection;
	eff.lpvTypeSpecificParams = &diPeriodic;
	eff.dwFlags         = NULL;//DIEFF_POLAR | DIEFF_OBJECTOFFSETS;	// Using polar coordinates
	eff.dwTriggerButton = DIEB_NOTRIGGER;						// Not a button response
	eff.cAxes                   = 2;							// Both axes involved
	eff.rgdwAxes                = &dwAxes[0]; 
	eff.lpEnvelope              = nullptr;							// No envelope used
	eff.cbTypeSpecificParams    = sizeof(DIPERIODIC);			// Constant force-specific data
	eff.lpvTypeSpecificParams   = &diPeriodic;

	err = sLpdiEffectList[SHAKE]->SetParameters(&eff ,  DIEP_DURATION|DIEP_GAIN|DIEP_START|DIEP_TYPESPECIFICPARAMS);
	if(FAILED(err))
	{
		ErrorLogger::Writeln(L"Could not change shake effect");
		ErrorLogger::Writeln(ERRORSTRING(err));
		return FAILURE;
	}
	else
		return SUCCESS;
}

ErrorType MyInputs::CentreJoystickOff()
{
	if(sLpdiEffectList[CENTRE]==NULL)
		return FAILURE;

	HRESULT err=sLpdiEffectList[CENTRE]->Stop();

	if(FAILED(err))
	{
		ErrorLogger::Writeln(L"Failed to un-centre the joystick\n");
		ErrorLogger::Writeln(ERRORSTRING(err));
		return FAILURE;
	}
	else
		return SUCCESS;

}



ErrorType MyInputs::CentreJoystickOn(int Xmagnitude, int Ymagnitude)
{
	if(sLpdiEffectList[CENTRE]==nullptr)
		return FAILURE;

	HRESULT err;

	DICONDITION diCondition[2];

	diCondition[0].dwNegativeSaturation =10000;
	diCondition[0].lOffset =0;
	diCondition[0].dwPositiveSaturation=10000;
	diCondition[0].lNegativeCoefficient=Xmagnitude*100;
	diCondition[0].lPositiveCoefficient=Xmagnitude*100;
	diCondition[0].lDeadBand=0;

	diCondition[1].dwNegativeSaturation =10000;
	diCondition[1].lOffset =0;
	diCondition[1].dwPositiveSaturation=10000;
	diCondition[1].lNegativeCoefficient=Ymagnitude*100;
	diCondition[1].lPositiveCoefficient=Ymagnitude*100;
	diCondition[1].lDeadBand=0;

	LONG rgDirection[2]={ 0,0};
	DWORD    dwAxes[2] = { DIJOFS_X, DIJOFS_Y};	         // Indicate which axes will be used

	DIEFFECT eff;
	ZeroMemory(&eff, sizeof(eff));
	eff.dwSize=sizeof(eff);
	eff.dwGain=10000;
	eff.dwDuration=INFINITE;
	eff.dwSamplePeriod=0;
	eff.rglDirection=&rgDirection[0];
	eff.dwFlags         = DIEFF_POLAR | DIEFF_OBJECTOFFSETS;
	eff.dwTriggerButton = DIEB_NOTRIGGER;						   // Not a button response
	eff.cAxes                   = 2;							      // Both axes involved
	eff.rgdwAxes                = &dwAxes[0]; 
	eff.lpEnvelope              = nullptr;							// No envelope used
	eff.cbTypeSpecificParams    = sizeof(DICONDITION)*2;		// Constant force-specific data
	eff.lpvTypeSpecificParams   = diCondition;


	err = sLpdiEffectList[CENTRE]->SetParameters(&eff ,  DIEP_GAIN|DIEP_START|DIEP_TYPESPECIFICPARAMS);
	if(FAILED(err))
	{
		ErrorLogger::Writeln(L"Could not change center effect");
		ErrorLogger::Writeln(ERRORSTRING(err));
		return FAILURE;
	}
	else
		return SUCCESS;
	
}

