// camera.h
// Shell engine version 2023
// Chris Rook
// Last modified 06/09/2019
//		Updating to const wchar_t to support string literals in parameters
//	Added assumption of windowed when in DEBUG config, using _DEBUG macro in WinMain


#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "errortype.h"
#include "gamecode.h"
#include "errorlogger.h"


// Includes *************************************************************************************

#define WIN32_LEAN_AND_MEAN		// Gives you SDK windows

//#include "mytools.h"


#include <ddraw.h>
#include <stdio.h>				// IO stuff
#include <windows.h>			   // Windows headers all-in-one
#include <iostream>			   // IO stuff
#include <windowsx.h>			// Windows macros


// Defines **************************************************************************************

#define WINDOW_CLASS_NAME L"WINCLASS1"	
								// Defines the name of the window class I am going to use


// Globals ************************************************************************************

bool g_WindowClosed;			// A variable that records whether or not the window has been asked to close
bool g_ApplicationActive;		// A variable that is true if the application is active (i.e. foreground) false otherwise
HWND g_hWnd = NULL;
HINSTANCE g_hInstance = NULL;


// Function Declarations **********************************************************************************
ErrorType CreateDXWindow(const wchar_t* title, int width, int height);
void KillDXWindow();
bool CheckCommandLineFor(const char* flag, const char* cmdLine);

// Window proc **********************************************************************************
// Remember -- all the windows will use this winproc!!!
// The window proc handles all windows event messages
// In this case, it mostly ignores them or runs the defaul behaviour
// The default behaviour is often to ignore them too.
// Note that multiple messages could happen in a single frame - this will be called each time
LRESULT CALLBACK WindowProc(	// The event handler. It's a callback function
							HWND hwnd,		// The handle to the window that called
							UINT msg,		// The message sent
							WPARAM wparam,
							LPARAM lparam)	// These further subclass the message sent,
											// but I don't plan to use them
{
	PAINTSTRUCT ps;				// Use by WM_PAINT
	HDC hdc;					      // A handle to a device context. May be used by WM_PAINT,
								      // but not right now.

	// Decide what the message is
	switch(msg)
	{
	case WM_CREATE:				// If window has been created
		{
								      // Do any initialisation
			return(0);			   // Return success
		}
		break;

	case WM_ACTIVATEAPP:
		if(wparam==TRUE)	      // Window going active
		{
			g_ApplicationActive=true;
		}
		else
		{
			g_ApplicationActive=false;			
		}

	case WM_SETCURSOR:
		SetCursor(NULL);
		return (0);
		break;

	case WM_PAINT:
		{
			hdc=BeginPaint(hwnd, &ps);		// Validate the window
			EndPaint(hwnd, &ps);

			// I havn't really done much here - just did a standard paint-job to pretend
			// the window has been properly painted.
			// hwnd tells it what to paint - the window, of course
			// ps is the address of a structure holding the rectangle to bve drawn
			// hdc is a graphics context that describes the video system
         // For this program, drawing is actually handled by Direct3D

			return (0);//DefWindowProc(hwnd, msg, wparam, lparam);			// return success
		}
		break;

	case WM_CHAR:                    // A button press. I will actually handle input with DirectInput
		{
			return (0);
		}
		break;

	case WM_DESTROY:        			// Something wants to kill the application
		{

			PostQuitMessage(0);	      // Sends a quit message onto the windows queue

			return(0);			         // return success
		}
		break;

	default:
		break;

	}	// End the switch

	// Any messages not handled are done by the default handler
	return (DefWindowProc(hwnd, msg, wparam, lparam));
					// Ever feel like switching round wparam and lparam,
					// just to see what happens?
               // We programmers know how to make our own excitement

}	// End of WinProc


// Winmain **************************************************************************************

int WINAPI WinMain(HINSTANCE hInstance,			// A number that registers the instance of this program
				   HINSTANCE hPrevInstance,		// Not used in W95+. Null
				   LPSTR lpCmdLine,				// Pointer to a string that started this whole thing off
				   int nCmdShow)				// Integer that tells how the window is to appear. Active/maximized, etc
{
	// Activate memory leak detection - will report in visual studio when program exits
	// Requires debug configuration
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	
	MSG			msg;					// A general message
	memset(&msg,0, sizeof(msg));

	// Set global instance
	g_hInstance = hInstance;

	//Find out if the user wants the game windowed or full screen
	bool bFullScreen = true;

	// Assume windowed if debugging
#ifdef _DEBUG
	bFullScreen = false;
#endif // _DEBUG


	if(CheckCommandLineFor("windowed", lpCmdLine)==true)
		bFullScreen = false;

	if(CreateDXWindow(L"Shell Engine", 800, 600) == SUCCESS)
	{
		// The event loop
		
		g_WindowClosed=false;
		int gameError=Game::instance.Setup(bFullScreen, g_hWnd, g_hInstance);			// Initialise the game
		g_ApplicationActive=true;							// Window is now foreground

		if (gameError == FAILURE)							// If game failed to initialise
			g_WindowClosed = true;
		else
		while(!g_WindowClosed)
		{												         // Infinite loop
			while(PeekMessage(&msg, NULL, 0,0,PM_REMOVE))	// If there is a message in the queue, remove it and....
			{
				TranslateMessage(&msg);					   // Translate the message - it's voodoo

				DispatchMessage(&msg);					   // And send it to the Window proc

				if (msg.message == WM_QUIT)				// If is is "quit"
				{
					g_WindowClosed = true;
				}
			}

			if (!g_WindowClosed && g_ApplicationActive)		// Check I'm not already dead
			{
				gameError=Game::instance.Main();					// Play the real game stuff
				if(gameError==FAILURE)
					g_WindowClosed = true;
			}

		}		// End infinite loop

		Game::instance.Shutdown();									// Clear up the game

	}	// End if window created

	return((int)msg.wParam);								      // Return to windows

}	// End WinMain

ErrorType CreateDXWindow(const wchar_t* title, int width, int height)
{
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				   // Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values

	WindowRect.left		= (long)0;		// Set Left Value To 0
	WindowRect.right	= (long)width;	   // Set Right Value To Requested Width
	WindowRect.top		= (long)0;		   // Set Top Value To 0
	WindowRect.bottom	= (long)height;	// Set Bottom Value To Requested Height

	g_hInstance			= GetModuleHandle(NULL);				// Grab An Instance For Our Window
	ZeroMemory(&wc, sizeof(WNDCLASS));

	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC) WindowProc;					// WndProc Handles Messages
	wc.cbClsExtra		= 0;									      // No Extra Window Data
	wc.cbWndExtra		= 0;									      // No Extra Window Data
	wc.hInstance		= g_hInstance;							   // Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);		// Load The Arrow Pointer
	wc.hbrBackground	= NULL;									   // No Background Required For GL
	wc.lpszMenuName		= nullptr;								// We Don't Want A Menu
	wc.lpszClassName	= WINDOW_CLASS_NAME;					   // Set The Class Name

	if (!RegisterClass(&wc))									   // Attempt To Register The Window Class
	{
		ErrorLogger::Writeln(L"Failed to register the window class.");
		return FAILURE;
	}


	dwExStyle=WS_EX_APPWINDOW;			                  // Window Extended Style
	dwStyle=WS_POPUP;										      // Windows Style
	ShowCursor(true);											   // Don't Hide Mouse Pointer
	WindowRect.left = 0;
	WindowRect.right=width;
	WindowRect.top = 0;
	WindowRect.bottom=height;


	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(g_hWnd=CreateWindowEx(dwExStyle,							// Extended Style For The Window
								WINDOW_CLASS_NAME,					   // Class Name
								title,								      // Window Title
								dwStyle,							         // Required Window Style
								0, 0,								         // Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								g_hInstance,					// Instance
								NULL)))							// Dont Pass Anything To WM_CREATE
	{
		KillDXWindow();								// Reset The Display
		ErrorLogger::Writeln(L"Failed to create the window.");
		return FAILURE;
	}

	ShowWindow(g_hWnd, SW_SHOW);

	return SUCCESS;									// Success
	// Caution! Early returns
}

// Kill The Window Properly
void KillDXWindow()								
{

	// Switch Back To The Desktop
	ChangeDisplaySettings(NULL, 0);

	// Show Mouse Pointer
	ShowCursor(TRUE);

	if (g_hWnd && !DestroyWindow(g_hWnd))				// Are We Able To Destroy The Window?
	{
		ErrorLogger::Writeln(L"Could Not Release gHwnd.");
		g_hWnd=NULL;									// Set m_hWnd To NULL
	}

	if (!UnregisterClass(WINDOW_CLASS_NAME, g_hInstance))			// Are We Able To Unregister Class
	{
		ErrorLogger::Writeln(L"Could Not Unregister Class.");
		g_hInstance=NULL;									// Set m_hInstance To NULL
	}
}

// Function that checks the "LPSTR lpCmdLine" parameter for the presence of a specific flag
bool CheckCommandLineFor(const char* flag, const char* pCmdLine)
{
	int numArguments			= 0;

	//Initialise our point in the command line string
	int wordStart	= 0;

	// Store the total length of the command line string
	int length = (int)strlen(pCmdLine);

	//Is the length of the command line string greater than one?
	if(length > 1)
	{
		//Yes it is, we have a string, lets count how many arguments it has
		for(int x = 0;x<length;++x)
		{
			//Is the current character a space, or have we run off the end of the string?
			if(pCmdLine[x] == ' ' || x == length - 1)
			{
				//Yep! increment our number of command line arguments
				++numArguments;
			}
		}

		//Lets start checking the line
		//Loop through the command line 
		for(int x = 0;x<length;++x)
		{
			if(x == length - 1) ++x;

			//are we at the end of the current word?
			if(pCmdLine[x] == ' ' || x == length)
			{
				//Yes, get the length of the argument
				int argLen = (x - wordStart) + 1;

				//Create a new character array for the string in the ArgV array
				char* argument = new char[argLen];

				//Copy the word into the newly created string
				int z = 0;
				for(int y=wordStart;z<argLen;++y)
				{
					argument[z] = pCmdLine[y];
					++z;
				}

				//Null terminate the string
				argument[z-1] = '\0';

				if(_stricmp(argument, flag)==0)
				{
					delete[] argument;
					return true;
				}

				delete[] argument;
				//Record where the next word will start in the array
				wordStart = x + 1;
			}
		}
	}
	return false;
}
