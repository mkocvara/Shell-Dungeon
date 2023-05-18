// myinputs.cpp
// Shell engine version 2020
// Chris Rook
// Last modified 20/09/2018

#include "myinputs.h"
#include "errorlogger.h"

// *************************************************************************************
// Implementation of the global EnumerateJoystick function 

MyInputs* MyInputs::instance=nullptr;	
LPDIRECTINPUTDEVICE8 MyInputs::lpdijoystick=nullptr;	// The dirextInput joystick
LPDIRECTINPUTDEVICE8 MyInputs::lpdikeyboard=nullptr;	// The directInput keyboard
LPDIRECTINPUTDEVICE8 MyInputs::lpdimouse=nullptr;		// The directInput mouse	
LPDIRECTINPUT8 MyInputs::lpdi=nullptr;	
LPDIRECTINPUTEFFECT MyInputs::mrglpdiEffectList[3];

BOOL CALLBACK MyInputs::EnumerateJoystick(LPCDIDEVICEINSTANCE lpddi, LPVOID pDeviceFoundGUID)
// Nothing to see here, folks. Move along. Move along.
{
	HRESULT err;

    // Obtain an interface to the enumerated joystick.
    err = lpdi->CreateDevice(lpddi->guidInstance,  
                                &lpdijoystick, NULL);

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

ErrorType MyInputs::Start(HINSTANCE hinst, HWND hwnd)
{
	if(instance)
	{
		Terminate();
	}
	instance = new MyInputs(hinst, hwnd);

	if (instance)
		return SUCCESS;
	else
		return FAILURE;
}

ErrorType MyInputs::Terminate()
{
	// Need to stop all effects
	// -> Stop does not seem to work
	instance->PullJoystick(0,.1,0);
	if(mrglpdiEffectList[SHAKE])
	{
		mrglpdiEffectList[SHAKE]->Stop();
		mrglpdiEffectList[SHAKE]->Release();
	}
	if(mrglpdiEffectList[PULL]) 
	{
		mrglpdiEffectList[PULL]->Stop();
		mrglpdiEffectList[PULL]->Release();
	}
	if(mrglpdiEffectList[CENTRE]) 
	{
		mrglpdiEffectList[CENTRE]->Stop();
		mrglpdiEffectList[CENTRE]->Release();
	}

	if(instance)
	{
		delete instance;
		instance = nullptr;
		return SUCCESS;
	}
	return FAILURE;
}

MyInputs* MyInputs::GetInstance()
{
	return instance;
}

MyInputs::MyInputs(HINSTANCE hinst, HWND hwnd)
:mkiMaxJoystickAxis(100)
{
	lpdi=nullptr;

	lpdikeyboard=nullptr;
	lpdimouse=nullptr;
	lpdijoystick=nullptr;

	miMouseDX=0;
	miMouseDY=0;
	mbMouseLeft=false;
	mbMouseRight = false;
	mbMouseMiddle = false;

	mrglpdiEffectList[PULL]=nullptr;
	mrglpdiEffectList[SHAKE]=nullptr;
	mrglpdiEffectList[CENTRE]=nullptr;

	
	HRESULT err =DirectInput8Create(hinst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&lpdi, NULL);
	if (FAILED(err))
	{
		// Failed to get DInput.
		ErrorLogger::Writeln(L"Failed to create direct input.\n");
		ErrorLogger::Writeln(ERRORSTRING(err));

		return;
	}

	// Direct input is set up - handle the keyboard next ********************
	err=lpdi->CreateDevice(GUID_SysKeyboard, &lpdikeyboard, NULL);
	if (FAILED(err))
	{
		// Failed to get a keyboard!
		ErrorLogger::Writeln(L"Failed to find a keyboard\n");
		ErrorLogger::Writeln(ERRORSTRING(err));
	}

	// Setting format for the device to be a default keyboard
	if(lpdikeyboard)
	{
		err=lpdikeyboard->SetDataFormat(&c_dfDIKeyboard);
		if (FAILED(err))
		{
			// Failed to set format
			ErrorLogger::Writeln(L"Failed to set keyboard format\n");
			ErrorLogger::Writeln(ERRORSTRING(err));
			lpdikeyboard->Release();
			lpdikeyboard=nullptr;
		}
	}

	// Setting cooperation level to BACKGROUND (gets input whether on top or not)
	//   and EXCLUSIVE (let other programs use the keyboard, too)
	if(lpdikeyboard)
	{
		err=lpdikeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
		if (FAILED(err))
		{
			// Failed to set cooperative level
			ErrorLogger::Writeln(L"Failed to set keyboard cooperative level");
			ErrorLogger::Writeln(ERRORSTRING(err));
			lpdikeyboard->Release();
			lpdikeyboard=nullptr;
		}
	}
	// Acquire keyboard
	if(lpdikeyboard)
	{
		err=lpdikeyboard->Acquire();
		if (FAILED(err))
		{
			// Failed to set cooperative level
			ErrorLogger::Writeln(L"Failed to acquire keyboard on init!");
			ErrorLogger::Writeln(ERRORSTRING(err));

			// Commenting out - Don't release the keyboard, just try to reacquire next time around
			//lpdikeyboard->Release();
			//lpdikeyboard=NULL;
		}
	}

	// Set all of the keyboard array to zero
	memset(mrgcKeystate, 0, KEYMAPSIZE);
	memset(mrgcOldKeystate, 0, KEYMAPSIZE);

	// Keyboard ready. Next is the mouse **************************************
	err=lpdi->CreateDevice(GUID_SysMouse, &lpdimouse, NULL);
	if (FAILED(err))
	{
		// Failed to get a mouse
		ErrorLogger::Writeln(L"Failed to find a mouse");
		ErrorLogger::Writeln(ERRORSTRING(err));
	}

	// Setting cooperation level to FOREGROUND (only gets input when on top)
	//   and EXCLUSIVE (let other programs use the mouse, too)
	if(lpdimouse)
	{
		err=lpdimouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND|DISCL_EXCLUSIVE);
		if (FAILED(err))
		{
			// Failed to set cooperative level
			ErrorLogger::Writeln(L"Failed to set mouse cooperative level");
			ErrorLogger::Writeln(ERRORSTRING(err));
			lpdimouse->Release();
			lpdimouse=nullptr;
		}
	}

	// Setting format for the device to be a default mouse
	if(lpdimouse)
	{
		err=lpdimouse->SetDataFormat(&c_dfDIMouse);
		if (FAILED(err))
		{
			// Failed to set format
			ErrorLogger::Writeln(L"Failed to set mouse format");
			ErrorLogger::Writeln(ERRORSTRING(err));
			lpdimouse->Release();
			lpdimouse=nullptr;
		}
	}

	// Aquire mouse
	if(lpdimouse)
	{
		err=lpdimouse->Acquire();
		if (FAILED(err))
		{
			// Failed to set cooperative level
			ErrorLogger::Writeln(L"Failed to acquire mouse");
			ErrorLogger::Writeln(ERRORSTRING(err));
			lpdimouse->Release();
			lpdimouse=nullptr;

		}
	}

	// Mouse is armed and hungry. Next try the joystick

	// First need to enumerate the joysticks and just claim the first one connected
	// (a bit simple - alternative is to give users a choice from a list, but too much hassle.)

	// Enumerate the attached joystick devices. This will repeatedly call my "EnumerateJoysticks()
	//  function, and will put the joystick address in lpdijoystick.

	err=lpdi->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumerateJoystick, NULL, DIEDFL_ATTACHEDONLY);
	if (FAILED(err))
	{
		ErrorLogger::Writeln(L"Failed to enumerate joystick devices.");
		ErrorLogger::Writeln(ERRORSTRING(err));
	}

	// Set the cooperative level - want exclusive access

	if(lpdijoystick)
	{
		err=lpdijoystick->SetCooperativeLevel(hwnd, DISCL_BACKGROUND | DISCL_EXCLUSIVE);
		if(FAILED(err))
		{
			// Could not set cooperative level
			lpdijoystick->Release();
			lpdijoystick=nullptr;
			ErrorLogger::Writeln(L"Could not set joystick cooperative level\n");
			ErrorLogger::Writeln(ERRORSTRING(err));
		}
	}

	// Set the data format for the joystick

	if(lpdijoystick)
	{
		err=lpdijoystick->SetDataFormat(&c_dfDIJoystick);
		if(FAILED(err))
		{
			// Could not set data format
			lpdijoystick->Release();
			lpdijoystick=nullptr;
			ErrorLogger::Writeln(L"Could not set joystick data format");
			ErrorLogger::Writeln(ERRORSTRING(err));
		}
	}

	if(lpdijoystick)
	{
		// Set joystick ranges - start with x axis
    // No error reporting here. Any problems are probably just because it does not have the specified axis

		DIPROPRANGE joystickAxisRange;		// A struct to store the details of the range you want to set

		joystickAxisRange.lMax = mkiMaxJoystickAxis;				// Set max range to the constant
		joystickAxisRange.lMin = -mkiMaxJoystickAxis;				// Same for min range
		joystickAxisRange.diph.dwSize=sizeof(DIPROPRANGE);			// Tell it its own size
		joystickAxisRange.diph.dwHeaderSize=sizeof(DIPROPHEADER);	// Tell it the size of its header
		joystickAxisRange.diph.dwObj = DIJOFS_X;					// Setting the x-axis
		joystickAxisRange.diph.dwHow = DIPH_BYOFFSET;				// Use offset (as always) for getting data

		lpdijoystick->SetProperty(DIPROP_RANGE, &joystickAxisRange.diph);

		// Y-axis next
		joystickAxisRange.lMax = mkiMaxJoystickAxis;				// Set max range to the constant
		joystickAxisRange.lMin = -mkiMaxJoystickAxis;				// Same for min range
		joystickAxisRange.diph.dwSize=sizeof(DIPROPRANGE);			// Tell it its own size
		joystickAxisRange.diph.dwHeaderSize=sizeof(DIPROPHEADER);	// Tell it the size of its header
		joystickAxisRange.diph.dwObj = DIJOFS_Y;					// Setting the y-axis
		joystickAxisRange.diph.dwHow = DIPH_BYOFFSET;				// Use offset (as always) for getting data

		lpdijoystick->SetProperty(DIPROP_RANGE, &joystickAxisRange.diph);

		// twist-axis
		joystickAxisRange.lMax = mkiMaxJoystickAxis;				// Set max range to the constant
		joystickAxisRange.lMin = -mkiMaxJoystickAxis;				// Same for min range
		joystickAxisRange.diph.dwSize=sizeof(DIPROPRANGE);			// Tell it its own size
		joystickAxisRange.diph.dwHeaderSize=sizeof(DIPROPHEADER);	// Tell it the size of its header
		joystickAxisRange.diph.dwObj = DIJOFS_RZ;					// Setting the z-axis
		joystickAxisRange.diph.dwHow = DIPH_BYOFFSET;				// Use offset (as always) for getting data
		
		lpdijoystick->SetProperty(DIPROP_RANGE, &joystickAxisRange.diph);

		// Finally, throttle-axis
		joystickAxisRange.lMax = mkiMaxJoystickAxis;				// Set max range to the constant
		joystickAxisRange.lMin = 0;				// Same for min range
		joystickAxisRange.diph.dwSize=sizeof(DIPROPRANGE);			// Tell it its own size
		joystickAxisRange.diph.dwHeaderSize=sizeof(DIPROPHEADER);	// Tell it the size of its header
		joystickAxisRange.diph.dwObj = DIJOFS_SLIDER(0);					// Setting the z-axis
		joystickAxisRange.diph.dwHow = DIPH_BYOFFSET;				// Use offset (as always)
		
		err=lpdijoystick->SetProperty(DIPROP_RANGE, &joystickAxisRange.diph);
	}

   // If, after all of that, we don't have a pointer to the joystick,
   // then it probably is simply not there
	if(lpdijoystick)
	{
		// Finally, acquire the joystick
		if(FAILED(lpdijoystick->Acquire()))
		{
            // If that failed, release everything
			lpdijoystick->Release();
			lpdijoystick=nullptr;
			ErrorLogger::Writeln(L"Failed to aquire joystick");
			ErrorLogger::Writeln(ERRORSTRING(err));
		}
	}

	// Enumerate the effects
	if(lpdijoystick)
		err = lpdijoystick->EnumEffects(&(this->DIEnumEffectsCallback), lpdijoystick, DIEFT_ALL);
}


MyInputs::~MyInputs()
{
	Release();
}

void MyInputs::Release()
{
	// Release all objects
	if (lpdimouse)
	{
		lpdimouse->Release();
		lpdi=nullptr;
	}
	if (lpdikeyboard)
	{
		lpdikeyboard->Release();
		lpdikeyboard=nullptr;
	}

	if (lpdijoystick)
	{
		lpdijoystick->Release();
		lpdijoystick=nullptr;
	}

	if (lpdi) 
	{
		lpdi->Release();
		lpdi=nullptr;
	}
	mJoystickGUID.Data1=NULL;		// Set joystick guid back to null
}




void MyInputs::SampleMouse()
{
	if(!lpdimouse) 
	{
		ErrorLogger::Writeln(L"No mouse - cannot sample.");	
		return;
	}

	// Get state of mouse from DInput
	HRESULT err=lpdimouse->GetDeviceState(sizeof(msMousestate), (LPVOID) &msMousestate);
	if(FAILED(err))
	{	
		ErrorLogger::Writeln(L"Failed to get mouse state.");
		ErrorLogger::Writeln(ERRORSTRING(err));
		if (err==DIERR_INPUTLOST || err==DIERR_NOTACQUIRED)
		{
			ErrorLogger::Write(L"Attempting to reacquire....");
			lpdimouse->Acquire();
			err=lpdimouse->GetDeviceState(sizeof(msMousestate), (LPVOID) &msMousestate);
			if(FAILED(err))
				ErrorLogger::Writeln(L"Failed");
			else
				ErrorLogger::Writeln(L"Success");
		}
	}

	// Record mouse movements in member variables
	miMouseDX=msMousestate.lX;
	miMouseDY=msMousestate.lY;
	miMouseDZ=msMousestate.lZ;

	// record state of previous sample
	mbLastMouseLeft = mbMouseLeft;
	mbLastMouseRight = mbMouseRight;
	mbLastMouseMiddle = mbMouseMiddle;

	// See which buttons were down and set accordingly
	if(msMousestate.rgbButtons[0] & 0x80)
	{
		mbMouseLeft = true;
	}
	else
	{
		mbMouseLeft = false;
	}

	if(msMousestate.rgbButtons[1] & 0x80)
	{
		mbMouseRight = true;
	}
	else
	{
		mbMouseRight = false;
	}

	if(msMousestate.rgbButtons[2] & 0x80)
	{
		mbMouseMiddle = true;
	}
	else
	{
		mbMouseMiddle = false;
	}


}

int MyInputs::GetMouseDX()
{
	return miMouseDX;
}

int MyInputs::GetMouseDY()
{
	return miMouseDY;
}

int MyInputs::GetMouseDZ()
{
	return miMouseDZ;
}

bool MyInputs::IfMouseNewLeftDown()
{
	return(mbMouseLeft && !mbLastMouseLeft);
}

bool MyInputs::IfMouseNewMiddleDown()
{
	return(mbMouseMiddle && !mbLastMouseMiddle);
}

bool MyInputs::IfMouseNewRightDown()
{
	return(mbMouseRight && !mbLastMouseRight);
}

bool MyInputs::IfMouseNewLeftUp()
{
	return(!mbMouseLeft && mbLastMouseLeft);
}

bool MyInputs::IfMouseNewRightUp()
{
	return(!mbMouseRight && mbLastMouseRight);
}

bool MyInputs::IfMouseNewMiddleUp()
{
	return(!mbMouseMiddle && mbLastMouseMiddle);
}

bool MyInputs::IfMouseLeftDown()
{
	return mbMouseLeft;
}

bool MyInputs::IfMouseRightDown()
{
	return mbMouseRight;
}

bool MyInputs::IfMouseMiddleDown()
{
	return mbMouseMiddle;
}


char* MyInputs::GetKeyboardState()
{
	return mrgcKeystate;
}

bool MyInputs::KeyPressed(unsigned char key)
{
	return ((mrgcKeystate[key]&0x80)>0);	// Return true if 0x80 bit set (set if key was pressed)
}

bool MyInputs::NewKeyPressed(unsigned char key)
{
	bool kp = (mrgcKeystate[key]&0x80)>0;
	bool okp = (mrgcOldKeystate[key]&0x80)>0;

	return (kp && !okp);	// Return true if 0x80 bit set (set if key was pressed)
}


void MyInputs::SampleKeyboard()
{
	if(lpdikeyboard)			// Can't sample keyboard if it was not created
	{
		memcpy(mrgcOldKeystate, mrgcKeystate, KEYMAPSIZE);
		HRESULT err=lpdikeyboard->GetDeviceState(KEYMAPSIZE, &mrgcKeystate);
		if(FAILED(err))
		{
			ErrorLogger::Writeln(L"Failed to get keyboard state.");
			ErrorLogger::Writeln(ERRORSTRING(err));
			if (err==DIERR_INPUTLOST||err==DIERR_NOTACQUIRED)
			{
				ErrorLogger::Write(L"Attempting to reacquire....");
				lpdikeyboard->Acquire();
				err=lpdikeyboard->GetDeviceState(sizeof(msMousestate), (LPVOID) &msMousestate);
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
	if(lpdijoystick)			// Can't sample joystick if it was not created
	{
		HRESULT err=lpdijoystick->Poll();
		if(FAILED(err))
		{
			ErrorLogger::Writeln(L"Could not poll joystick");
			ErrorLogger::Writeln(ERRORSTRING(err));
			if(err==DIERR_INPUTLOST || err==DIERR_NOTACQUIRED)
		
			{
				err=lpdijoystick->Acquire();			// Reacquire the keyboard
				ErrorLogger::Write(L"Attempting to reacquire...");
				if(FAILED(err))
					ErrorLogger::Writeln(L"Failed");
				else
				{
					ErrorLogger::Writeln(L"Success");
					lpdijoystick->Poll();
				}
			}
		}

		err=lpdijoystick->GetDeviceState(sizeof(msJoystickState), &msJoystickState);
		if(FAILED(err))
		{
			ErrorLogger::Writeln(L"Could not get joystick state.");
			ErrorLogger::Writeln(ERRORSTRING(err));
			if(err==DIERR_INPUTLOST || err==DIERR_NOTACQUIRED)
			{
				err=lpdijoystick->Acquire();			// Reacquire the joystick
				ErrorLogger::Write(L"Attempting to reacquire...");
				if(FAILED(err))
					ErrorLogger::Writeln(L"Failed");
				else
				{
					ErrorLogger::Writeln(L"Success");
					lpdijoystick->GetDeviceState(sizeof(msJoystickState), &msJoystickState);
				}
			}
		}

	}

}

int MyInputs::GetJoystickX()
{
	return msJoystickState.lX;
}

int MyInputs::GetJoystickY()
{
	return msJoystickState.lY;
}

int MyInputs::GetJoystickTwist()
{
	return msJoystickState.lRz;
}

int MyInputs::GetJoystickThrottle()
{
	return 100-msJoystickState.rglSlider[0];
}


bool MyInputs::IfPOVCentred()
{
	return (LOWORD(msJoystickState.rgdwPOV[0]) == 0xFFFF);
}

int MyInputs::GetJoystickPOV()
{
	return msJoystickState.rgdwPOV[0]/100;
}

bool MyInputs::IfJoystickButton(int buttonNumber)
{
	if (buttonNumber<0||buttonNumber>31)
		return false;

	if((msJoystickState.rgbButtons[buttonNumber]&0x80)>0)
	{
		return true;
	}

	return false;
}

bool MyInputs::JoystickIsAvailable()
{
	return (lpdijoystick!=nullptr);
}


bool MyInputs::MouseIsAvailable()
{
	return (lpdimouse!=nullptr);
}

bool MyInputs::KeyboardIsAvailable()
{
	return (lpdikeyboard!=nullptr);
}


BOOL CALLBACK MyInputs::DIEnumEffectsCallback(LPCDIEFFECTINFO pdei, LPVOID pvRef)
{
	// Not going to enumerate properly.
	// If any effect is found for the right joystick, just
	// create four standard effects and stop enumerating.
	// This is all a bit of a mess - I am setting stuff here that I just change when the
	// effects are called. It works, though.
	
    HRESULT  err;
    if((LPDIRECTINPUTDEVICE8)pvRef!= lpdijoystick)		// Wrong joystick
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

	if(mrglpdiEffectList[PULL])							// If already there
		mrglpdiEffectList[PULL]->Release();

	err = lpdijoystick->CreateEffect(pdei->guid,		// Create it
                            &diEffect,
							&mrglpdiEffectList[PULL],	// Pass by reference pointer to created effect (out)
                            NULL);

	if(FAILED(err))
	{
		ErrorLogger::Writeln(L"Failed to create a constant force effect.");
		ErrorLogger::Writeln(ERRORSTRING(err));
		mrglpdiEffectList[PULL]=nullptr;
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

	err = lpdijoystick->CreateEffect(GUID_Sine,		// Create it
					&diEffect,
					&mrglpdiEffectList[SHAKE],		// Pass by reference pointer to created effect (out)
                    NULL);

	if(FAILED(err))
	{
		ErrorLogger::Writeln(L"Failed to create a shake effect.");
		ErrorLogger::Writeln(ERRORSTRING(err));
		mrglpdiEffectList[SHAKE]=nullptr;
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

	err = lpdijoystick->CreateEffect(GUID_Spring,		// Create it
					&diEffect,
					&mrglpdiEffectList[CENTRE],		// Pass by reference pointer to created effect (out)
                    NULL);

	if(FAILED(err))
	{
		ErrorLogger::Writeln(L"Failed to create a centre effect.");
		ErrorLogger::Writeln(ERRORSTRING(err));
		mrglpdiEffectList[CENTRE]=nullptr;
	}

    return DIENUM_STOP;	// As long as basic effects are found, this is OK

}  // End of callback

ErrorType MyInputs::PullJoystick(int direction, double duration, int magnitude)
{
	if(!mrglpdiEffectList[PULL])
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

	HRESULT err = mrglpdiEffectList[PULL]->SetParameters(&eff ,  DIEP_DURATION|DIEP_GAIN|DIEP_DIRECTION);
	if(FAILED(err))
	{
		ErrorLogger::Writeln(L"Could not change pull effect");
		ErrorLogger::Writeln(ERRORSTRING(err));
		return FAILURE;
	}
	else
	{
		err=mrglpdiEffectList[PULL]->Start(1,0);
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
	if(!mrglpdiEffectList[SHAKE])		// If effect not present
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

	err = mrglpdiEffectList[SHAKE]->SetParameters(&eff ,  DIEP_DURATION|DIEP_GAIN|DIEP_START|DIEP_TYPESPECIFICPARAMS);
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
	if(mrglpdiEffectList[CENTRE]==NULL)
		return FAILURE;

	HRESULT err=mrglpdiEffectList[CENTRE]->Stop();

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
	if(mrglpdiEffectList[CENTRE]==nullptr)
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


	err = mrglpdiEffectList[CENTRE]->SetParameters(&eff ,  DIEP_GAIN|DIEP_START|DIEP_TYPESPECIFICPARAMS);
	if(FAILED(err))
	{
		ErrorLogger::Writeln(L"Could not change center effect");
		ErrorLogger::Writeln(ERRORSTRING(err));
		return FAILURE;
	}
	else
		return SUCCESS;
	
}

