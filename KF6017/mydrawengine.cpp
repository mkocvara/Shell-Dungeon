// mydrawengine.cpp
// Shell engine version 2023
// Chris Rook
// Modified 25/09/2018
   // Fixed camera not affecting angles
// Modified 4/11/2018
   // Changed FillCircle to use positive angles.
// Modified 06/09/2019
//		Updating LoadPicture, FindPicture, AddFont to const wchar_t to support string literals in parameters


#include "mydrawengine.h"
#include <algorithm>			// Using find() in DeregisterPicture
#include "camera.h"

// *************************************************************
// Constructors and singleton management
// *************************************************************


ErrorType MyDrawEngine::Start(bool fullScreen)
{
	// Start the window for Windows
	if(FAILED(StartWindow()))
	{
		ErrorLogger::Writeln(L"Failed to start the MyDrawEngine window");
		return FAILURE;
	}

	// Start a default font
	AddFont(L"Ariel", 24, false, false);
	
	// If user has requested windowed mode
	if(!fullScreen)
		GoWindowed();

	// Clear everything
	Flip();
	ClearBackBuffer();

	mStarted = true;

	return SUCCESS;
}

// **************************************************************

// Constructor
// hwnd - handle to the Window
MyDrawEngine::MyDrawEngine(HWND hwnd)
{
	// Set pointers to NULL
	mpLPD3D = nullptr;
	mpLPD3DDevice = nullptr;

	mHwnd = hwnd;				// Remember the window handle

	mFullScreen = true;		// Start off fullscreen

	// The first font to be created will be at position 0
	mNextFont = 0;

	// First image to be created will be at position 1
	mNextPictureIndex=1;

	mLPSprite = nullptr;

	mpCamera = std::make_unique<Camera>(this);
	mCameraActive = true;

}		// Constructor

// *******************************************************************

MyDrawEngine::~MyDrawEngine()
{
	// Just make damn sure that directx is released
	Release();
}	// Destructor

// **************************************************************

// Starts the window
ErrorType MyDrawEngine::StartWindow()
{
	// Connect to direct 3D 9
	mpLPD3D=Direct3DCreate9(D3D_SDK_VERSION);
	if (mpLPD3D == nullptr)
	{
		ErrorLogger::Writeln(L"Could not connect to Direct3D");
		return FAILURE;		            // No point going any further
	}

	// Get native resolution
	mNativeScreenWidth=GetSystemMetrics(SM_CXSCREEN);
	mNativeScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	mScreenHeight = mNativeScreenHeight;
	mScreenWidth = mNativeScreenWidth;

	// Set the presentation parameters options
	D3DPRESENT_PARAMETERS d3dpp;		         // Order form for options
	ZeroMemory(&d3dpp, sizeof(d3dpp));	      // Set it all to zero

	d3dpp.Windowed = false;				         // Whatever the user requested
	d3dpp.SwapEffect = D3DSWAPEFFECT_FLIP;	   // Slightly slower, but allows access to previous back buffer if ever needed.
	d3dpp.hDeviceWindow = mHwnd;			      // Handle to the window
	d3dpp.BackBufferWidth = mScreenWidth;	   // Requested screen width
	d3dpp.BackBufferHeight = mScreenHeight;  // Requested screen height
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8; // Back buffer formattaed to 32 bit XRGB

	// Create the device
	HRESULT err;			// To store the error result
	err=mpLPD3D->CreateDevice(D3DADAPTER_DEFAULT,		// Default graphics adapter
                      D3DDEVTYPE_HAL,			// Requesting hardware abstraction layer
                      mHwnd,					   // Handle to the window. Again.
                      D3DCREATE_MIXED_VERTEXPROCESSING,	// Process vertices in software !!!!!!
                      &d3dpp,					   // The presentation parameters
                      &mpLPD3DDevice);			// Pointer to the device

	if(FAILED(err))
	{
		ErrorLogger::Writeln(L"Failed to create the device");
		ErrorLogger::Writeln(ERRORSTRING(err));
		mpLPD3D->Release();
		mpLPD3D=nullptr;
		return FAILURE;		// No point going any further
	}

	err = D3DXCreateSprite(mpLPD3DDevice, &mLPSprite );
	if(FAILED(err))
	{
		ErrorLogger::Writeln(L"Failed to create sprite");
		ErrorLogger::Writeln(ERRORSTRING(err));

		mpLPD3D->Release();
		mpLPD3D=nullptr;
		mpLPD3DDevice->Release();
		mpLPD3DDevice = nullptr;
		return FAILURE;		// No point going any further
	}

	return SUCCESS;
}		// Start window

// ********************************************************************

// Release the engine
ErrorType MyDrawEngine::Release()
{
	// Release fonts and bitmaps
	ReleaseBitmaps();
	ReleaseFonts();

	// Release the sprite
	if(mLPSprite)
	{
		mLPSprite->Release();
		mLPSprite = nullptr;
	}

	// Release Direct3D interface
	if(mpLPD3D)
	{
		mpLPD3D->Release();
		mpLPD3D = nullptr;
	}

	// Release the graphics device
	if(mpLPD3DDevice)
	{
		mpLPD3DDevice->Release();
		mpLPD3DDevice = nullptr;
	}

	return SUCCESS;
}	// Release


// *******************************************************************
// Changing mode and device resets
// *******************************************************************

// Resets the device if it goes FUBAR or when switching fullscreen/windowed
ErrorType MyDrawEngine::ResetDevice()
{
// Set the presentation parameters options
	D3DPRESENT_PARAMETERS d3dpp;			      // Order form
	ZeroMemory(&d3dpp, sizeof(d3dpp));		   // Set it all to zero

	d3dpp.Windowed = !mFullScreen;			   // Whatever the user requested
	d3dpp.SwapEffect = D3DSWAPEFFECT_FLIP;	   // Slightly slower, but allows access to previous back buffer if ever needed.
	d3dpp.hDeviceWindow = mHwnd;			      // Handle to the window
	d3dpp.BackBufferWidth = mScreenWidth;	   // Requested screen width
	d3dpp.BackBufferHeight = mScreenHeight;	// Requested screen height
	d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;	// Format is 32 bit XRGB

	// Need to release all bitmaps and fonts
	ReleaseBitmaps();
	ReleaseFonts();
	if(mLPSprite)
		mLPSprite->Release();
	mLPSprite = nullptr;

	// Reset the device
	HRESULT err = mpLPD3DDevice->Reset(&d3dpp);

	// Did it work?
	if(FAILED(err))
	{
		// Big trouble.
		ErrorLogger::Writeln(L"Failed to reset device.");
		ErrorLogger::Writeln(ERRORSTRING(err));
	}

	// Now can reload the bitmaps and fonts.
	ReloadBitmaps();
	ReloadFonts();

	HRESULT err2 = D3DXCreateSprite(mpLPD3DDevice, &mLPSprite );
	if(FAILED(err2))
	{
		ErrorLogger::Writeln(L"Failed to create sprite on device reset");
		ErrorLogger::Writeln(ERRORSTRING(err2));
	}

	// Position the window in case needed
    SetWindowPos(mHwnd, HWND_NOTOPMOST,
                     0, 0,
                     mScreenWidth,
                     mScreenHeight,
                     SWP_SHOWWINDOW);

	if(FAILED(err))
		return FAILURE;
	else
		return SUCCESS;
}		// ResetDevice

// ****************************************************************


// Releases all bitmaps. Used on shutdown and when resetting device.
// Does not delete them from the map!
void MyDrawEngine::ReleaseBitmaps()
{
	// Start at the beginning
	std::map<PictureIndex, MyPicture>::iterator picit = mIndexPictureMap.begin();

	// Loop through all bitmaps
	for(;picit!=mIndexPictureMap.end();picit++)
	{
		if(picit->second.mLpdTheTexture)
			picit->second.mLpdTheTexture->Release();
		picit->second.mLpdTheTexture = nullptr;
	}

	mIndexPictureMap.clear();
	mFilenameIndexMap.clear();
}	// Release bitmaps

// *******************************************************

// Reload all bitmaps. Needed after resetting the device
void MyDrawEngine::ReloadBitmaps()
{
	// Start at the beginning
	std::map<PictureIndex, MyPicture>::iterator it = mIndexPictureMap.begin();

	// Loop through all bitmaps
	for(;it!=mIndexPictureMap.end();it++)
	{
		// Could be faster by loading in-place, but file loading is much slower anyway
		ReloadPicture(it->first);
	}
}	// ReloadBitmaps

// ******************************************************

// Releases all fonts. Does not delete them from the map.
// Needed when resetting device or on shutdown
void MyDrawEngine::ReleaseFonts()
{
	// Start at the beginning
	std::map<FontIndex, MyFont>::iterator fit = mIndexFontMap.begin();

	// Loop through all fonts
	for(;fit!=mIndexFontMap.end();fit++)
	{
		if(fit->second.mFont)
			fit->second.mFont->Release();
		fit->second.mFont = nullptr;
	}
}	// ReleaseFonts

// *********************************************************


// Reload all fonts. Needed after resetting the device
void MyDrawEngine::ReloadFonts()
{
	// Start at the beginning
	std::map<FontIndex, MyFont>::iterator fit = mIndexFontMap.begin();

	// Loop through fonts
	for(;fit!=mIndexFontMap.end();fit++)
	{
		MyFont& rCurrentFont = fit->second;

		// Set requested boldness
		UINT boldness =FW_MEDIUM;
		if(fit->second.mBold == true)
			boldness = FW_BOLD;

		HRESULT err;

		// Create a font, using recorded parameters from when first loaded
		err = D3DXCreateFont(mpLPD3DDevice, rCurrentFont.mHeight, 0, boldness, 0, rCurrentFont.mItalic, 
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
			DEFAULT_PITCH | FF_DONTCARE, rCurrentFont.mFontName.c_str(), &rCurrentFont.mFont );

		if(FAILED(err))
		{
			ErrorLogger::Write(L"Failed to reload a font - ");
			ErrorLogger::Writeln(rCurrentFont.mFontName.c_str());
			ErrorLogger::Writeln(ERRORSTRING(err));
		}
	}
}	// Reload fonts


// **************************************************************

// Switch to windowed mode
ErrorType MyDrawEngine::GoWindowed()
{
	if(mFullScreen == false)
	{
		// Already windowed
		return SUCCESS;			// That will be fifty quid, please
	}

	// remember it is now windowed
	mFullScreen = false;

	// Reset the device
	HRESULT err= ResetDevice();

	// Clear the screen
	Flip();
	ClearBackBuffer();

	if(FAILED(err))
	{
		return FAILURE;
	}
	else
	{
		Flip();
		ClearBackBuffer();
		return SUCCESS;
	}
}		// GoWindowed

// **************************************************************

// Switch to full screen
ErrorType MyDrawEngine::GoFullScreen()
{

	if(mFullScreen == true)
	{
		// Already full screen
		return SUCCESS;			// That will be fifty quid, please
	}

	// Remember you are now full screen
	mFullScreen=true;

	// Reset the graphics card
   // This includes reloading everything
	HRESULT err= ResetDevice();

	// Clear screen to get rid of anomalies
	Flip();
	ClearBackBuffer();

	if(FAILED(err))
		return FAILURE;
	else
		return SUCCESS;
}	

// ****************************************************************

bool MyDrawEngine::IsWindowFullScreen() const
{
	return mFullScreen;
}

// ****************************************************************

ErrorType MyDrawEngine::ChangeResolution(int width, int height)
{
   // Change the information
	int oldWidth = mScreenWidth;
	int oldHeight = mScreenHeight;
	mScreenWidth = width;
	mScreenHeight = height;

   // Needs to fully reset
	ErrorType err = ResetDevice();

   // It could not reset
	if(FAILED(err))
	{
      // Change back to old configuration
		mScreenWidth = oldWidth;
		mScreenHeight = oldHeight;	
		ResetDevice();       // Reset again. What if this also fails?
		ErrorLogger::Writeln(L"Failed to change resolution. Reverting to previous.");
	}

	return err;
}

ErrorType MyDrawEngine::ChangeToNativeResolution()
{
	int oldWidth = mScreenWidth;
	int oldHeight = mScreenHeight;
	// Get native resolution
	mScreenWidth=mNativeScreenWidth;
	mScreenHeight = mNativeScreenHeight;

	ErrorType err = ResetDevice();

	if(FAILED(err))         // It could not reset
	{
		mScreenWidth = oldWidth;
		mScreenHeight = oldHeight;	
		ResetDevice();       // Reset again. What if this also fails?
		ErrorLogger::Writeln(L"Failed to change resolution. Reverting to previous.");
	}

	return err;
}

// *******************************************************************

// Reloads a picture - used when resetting the device
ErrorType MyDrawEngine::ReloadPicture(PictureIndex pic)
{
	// Set the iterator to the row we are looking for in the list of pictures.
	std::map<PictureIndex, MyPicture>::iterator picit = mIndexPictureMap.find(pic);

	// If could not find the picture requested.
	if(picit==mIndexPictureMap.end())
	{
		ErrorLogger::Writeln(L"Attempting to reload an invalid PictureIndex in RelocadPicture.");
		return FAILURE;
	}

	// Release the old picture if it is present.
	if(picit->second.mLpdTheTexture)
	{
		picit->second.mLpdTheTexture->Release();
		picit->second.mLpdTheTexture = nullptr;
	}

	// Reload the picture using mostly default settings
	HRESULT err = D3DXCreateTextureFromFileEx(mpLPD3DDevice,
		(picit->second.mSourceFileName).c_str(), D3DX_DEFAULT, D3DX_DEFAULT, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT,
		D3DX_FILTER_LINEAR, D3DX_FILTER_LINEAR,
		0xff000000,				// Colour key is black
		NULL, NULL,
		&(picit->second.mLpdTheTexture) );

	if(FAILED(err))
	{
		ErrorLogger::Write(L"Failed to create texture from file in ReloadPicture: ");
		ErrorLogger::Writeln(picit->second.mSourceFileName.c_str());
		ErrorLogger::Writeln(ERRORSTRING(err));
		return FAILURE;
	}

	return SUCCESS;
}		// Reload picture

// ***********************************************************************

//Clears the back buffer
ErrorType MyDrawEngine::ClearBackBuffer()
{
	//Clear
	HRESULT err = mpLPD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, 0,1.0f,0);

	if(FAILED(err))
	{
		ErrorLogger::Writeln(L"Could not clear the back buffer.");
		ErrorLogger::Writeln(ERRORSTRING(err));
		return FAILURE;
	}
	return SUCCESS;
}	// ClearBackBuffer



// *****************************************************************

// Presents the Back Buffer and starts new scene
ErrorType MyDrawEngine::Flip()
{
	HRESULT err;		// To store error result

	// End the scene
	mpLPD3DDevice->EndScene();
	// Present
	err= mpLPD3DDevice->Present(NULL, NULL, NULL, NULL);

	if(FAILED(err))
	{
		ErrorLogger::Writeln(L"Could not flip.");
		ErrorLogger::Writeln(ERRORSTRING(err));
		return FAILURE;
	}

	// Start the next one
	mpLPD3DDevice->BeginScene();

	if(FAILED(err))
	{
		ErrorLogger::Writeln(L"Could not start new scene after flip.");
		ErrorLogger::Writeln(ERRORSTRING(err));
		return FAILURE;
	}
	return SUCCESS;
}	// Flip

// **************************************************************
// Camera visibility information
// **************************************************************


// Returns true if the specified point is visible from the current camera position
bool MyDrawEngine::IsOnCamera(Vector2D point) const
{
	return GetViewport().Intersects(point);
}

// Returns true if the specified point is fully or partially visible from the current camera position
bool MyDrawEngine::IsOnCamera(const IShape2D& rShape)const
{
	return rShape.Intersects(GetViewport());
}

// Returns a rectangle describing the area of the world that is visible from the current camera position
Rectangle2D MyDrawEngine::GetViewport() const
{
	Vector2D tl(0.0f, 0.0f);
	Vector2D bq((float)mScreenWidth,(float)mScreenHeight);
	Rectangle2D screen;
	screen.PlaceAt(mpCamera->ReverseTransform(tl), mpCamera->ReverseTransform(bq));
	return screen;
}

void MyDrawEngine::UseCamera(bool activate)
{
	mCameraActive = activate;
}


// **************************************************************
// Loading and deleting images
// **************************************************************

// Finds a previously loaded image
PictureIndex MyDrawEngine::FindPicture(const wchar_t* filename)
{
	// Find the picture
	std::map<std::wstring, PictureIndex>::iterator picit = mFilenameIndexMap.find(filename);

	// If not found
	if (picit == mFilenameIndexMap.end())
	{
		return -1;
	}
	else
	{
		return picit->second;
	}
}

bool MyDrawEngine::IsStarted() const
{
	return mStarted;
}

// Loading a picture
PictureIndex MyDrawEngine::LoadPicture(const wchar_t* filename)
{
	// See if file with that name has already been loaded
	PictureIndex found = FindPicture(filename);
	if (found >= 0)			            // Already loaded
	{
		return found;
	}
	else					                  // File not already loaded
	{
		MyPicture tempMyPicture;			// To store picture if it ever loads
		tempMyPicture.mSourceFileName = filename;	// Remember the filename

		// Load the picture using mostly default settings
		HRESULT err = D3DXCreateTextureFromFileEx(mpLPD3DDevice,
			filename, D3DX_DEFAULT, D3DX_DEFAULT, 1, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT,
			D3DX_FILTER_LINEAR, D3DX_FILTER_LINEAR,
			0xff000000,				         // Colour key is black
			NULL, NULL,
			&tempMyPicture.mLpdTheTexture);

		if (FAILED(err))		            // Probably a bad filename
		{
			ErrorLogger::Write(L"Failed to create texture from file: ");
			ErrorLogger::Writeln(filename);
			ErrorLogger::Writeln(ERRORSTRING(err));
			return 0;			            // Return zero if couldn't load
		}

		// Get information from the picture
		D3DSURFACE_DESC desc;
		tempMyPicture.mLpdTheTexture->GetLevelDesc(0, &desc);

		// Record the height and width
		tempMyPicture.mHeight = desc.Height;
		tempMyPicture.mWidth = desc.Width;

		// Set the default centre in the middle of the picture
		tempMyPicture.mCentre.set(float(tempMyPicture.mWidth / 2), float(tempMyPicture.mHeight / 2));

		// Add the picture to the map
		mIndexPictureMap.insert(std::pair<PictureIndex, MyPicture>(mNextPictureIndex, tempMyPicture));

		mFilenameIndexMap.insert(std::pair<std::wstring, PictureIndex>(filename, mNextPictureIndex));

		// Increase the number of the next picture, and return the old number.
		return mNextPictureIndex++;
	}

}		// LoadPicture

// ****************************************************************

// Request the size of a picture
void MyDrawEngine::GetDimensions(PictureIndex pic, int& rHeight, int& rWidth)
{
	// Find the picture
	std::map<PictureIndex, MyPicture>::iterator picit = mIndexPictureMap.find(pic);

	// If not found
	if(picit==mIndexPictureMap.end())
	{
		ErrorLogger::Writeln(L"Attempting to get an invalid PictureIndex in GetDimensions.");

	}
	else
	{
		// Set the return-by-reference data
		rHeight = picit->second.mHeight;
		rWidth = picit->second.mWidth;
	}
}	// GetDimensions



// ***********************************************************************



// Sets the centre of a MyPicture
ErrorType MyDrawEngine::SetCentre(PictureIndex pic, Vector2D centre)
{
	// Find the picture
	std::map<PictureIndex, MyPicture>::iterator picit = mIndexPictureMap.find(pic);

	// If not found
	if(picit==mIndexPictureMap.end())
	{
		ErrorLogger::Writeln(L"Attempting to set centre of an invalid PictureIndex in SetCentre.");
		return FAILURE;
	}
	else
	{
		// Set the centre
		picit->second.mCentre = centre;
		return SUCCESS;
	}

}	// SetCentre



// **************************************************************

// Release a picture from memory
void MyDrawEngine::ReleasePicture(PictureIndex pic)
{
	// Find the picture in the map
	std::map<PictureIndex, MyPicture>::iterator picit = mIndexPictureMap.find(pic);

	// If not found
	if(picit==mIndexPictureMap.end())
	{
		ErrorLogger::Writeln(L"Attempting to release an invalid PictureIndex in ReleasePicture.");
		return;
	}

	// Release it and set to null
	if(picit->second.mLpdTheTexture)
		picit->second.mLpdTheTexture->Release();
	picit->second.mLpdTheTexture = nullptr;

	// Remove it from the map
	mIndexPictureMap.erase(picit);

	// Remove it from the filename map
	mFilenameIndexMap.erase(picit->second.mSourceFileName);

}		// ReleasePicture

void MyDrawEngine::ReleaseKraken()
{
	static Vector2D b = Vector2D(1300, -300);
	FillCircle(b, 90, DARKRED);
	FillCircle(b + Vector2D(-50, 5), 70, DARKRED);
	FillCircle(b + Vector2D(-80, 10), 55, DARKRED);
	FillCircle(b + Vector2D(0, 40), 20, BLACK);
	b += Vector2D(-5, 1);
	static int c = 0;
	c = c + 1;
	srand(c / 4);
	for (int i = 0; i < 3; i++)
	{
		float lip = (rand() % 200 - 100) / 10.0f;
		float rip = (rand() % 250) / 100.0f;
		if (lip > 0) rip = -rip;
		Vector2D d = b + Vector2D(-100.0f, 0 - i * 15.0f);
		for (int f = 0; f < 15; f++)
		{
			d.XValue -= 15;
			d.YValue += lip;
			lip = lip + rip;
			FillCircle(d + Vector2D(0, 30), 20.0f - f / 2, DARKRED);
		}
	}
}

// **************************************************************
// Writing text and numbers
// **************************************************************


// *****************************************************************

// Add a new font to the engine
FontIndex MyDrawEngine::AddFont(const wchar_t* FontName, int height, bool bold, bool italic)
{
	MyFont temp;			// To hold the font being created
	// Record requested parameters
	temp.mBold = bold;
	temp.mFontName = FontName;
	temp.mHeight = height;
	temp.mItalic = italic;

	// Set up the value of "THICKNESS" needed in D3DXCreateFont
	UINT boldness =FW_MEDIUM;
	if(bold == true)
		boldness = FW_BOLD;

	HRESULT err;

	// Create the font, as requested
	err = D3DXCreateFont(mpLPD3DDevice, height, 0, boldness, 0, italic, 
		DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
		DEFAULT_PITCH | FF_DONTCARE, FontName, &temp.mFont );

	if(FAILED(err))
	{
		ErrorLogger::Write(L"Failed to create font ");
		ErrorLogger::Writeln(FontName);
		ErrorLogger::Writeln(ERRORSTRING(err));
		if(temp.mFont)
			temp.mFont->Release();
		return 0;
	}
	else
	{
		// Add it to the map
		mIndexFontMap.insert(std::pair<FontIndex, MyFont>(mNextFont, temp));
		return mNextFont++;		// Return the number of the font
	}
}
 

// ********************************************************************************

// Writes text to the screen
ErrorType MyDrawEngine::WriteText(int x, int y, const wchar_t text[], int colour, FontIndex fontIndex, bool centre)
{
	// Find the requested font
	std::map<FontIndex, MyFont>::iterator fit;	// Iterator to point to the font requested
	fit = mIndexFontMap.find(fontIndex);				// Find the font
	if(fit == mIndexFontMap.end())						// If font not found
	{
		ErrorLogger::Writeln(L"Write Text called with invalid FontIndex.");
		return FAILURE;
	}

	// Rect to draw the text inside
	RECT rect;
	rect.left =x;
	rect.top = y;
	rect.right =x+50;		// Will get expanded when the rect is calculated
	rect.bottom = y+50;

	// Nonna-Raymond call to DrawText. Calculates the rect, but does not draw
	fit->second.mFont->DrawText(NULL, text, -1, &rect, DT_CALCRECT , colour);

	if (centre)
	{
		const int width = rect.right - rect.left;
		rect.left -= width / 2;
		rect.right -= width / 2;

		const int height = rect.top - rect.bottom;
		rect.top -= height / 2;
		rect.bottom -= height / 2;
	}

	// Now make the real call.
	HRESULT err;
	err = fit->second.mFont->DrawText(NULL, text, -1, &rect, 0 , colour);
	if(FAILED(err))
	{
		ErrorLogger::Writeln(L"WriteText failed to draw text.");
		ErrorLogger::Writeln(ERRORSTRING(err));
		return FAILURE;		
	}
	return SUCCESS;	

}	// End WriteText

// **********************************************************

// Writes a floating point number to the screen
ErrorType MyDrawEngine::WriteDouble(int x, int y, double num, int colour, FontIndex fontIndex, bool centre)
{
	wchar_t buffer[32];			// To store the text when converted
	swprintf_s( buffer, 32, L"%.8g", num );	// Convert the number to text

	// Write it
	return WriteText(x,y, buffer, colour, fontIndex, centre);
}	// WriteDouble

// **********************************************************
// Writes an integer point number to the screen
ErrorType MyDrawEngine::WriteInt(int x, int y, int num, int colour, FontIndex fontIndex, bool centre)
{
	wchar_t buffer[32];
	swprintf_s( buffer,32, L"%i", num );

	return WriteText(x,y, buffer, colour, fontIndex, centre);
}	// WriteInt

// *******************************************************************

ErrorType MyDrawEngine::WriteText(Vector2D position, const wchar_t text[], int colour, FontIndex fontIndex, bool centre)
{
	if (mCameraActive)
	{
		position = mpCamera->Transform(position);
	}
	return WriteText(int(position.XValue), int(position.YValue), text, colour, fontIndex, centre);
}

ErrorType MyDrawEngine::WriteInt(Vector2D position, int num, int colour, FontIndex fontIndex, bool centre)
{
	if (mCameraActive)
	{
		position = mpCamera->Transform(position);
	}
	return WriteInt(int(position.XValue), int(position.YValue), num, colour, fontIndex, centre);
}

ErrorType MyDrawEngine::WriteDouble(Vector2D position, double num, int colour, FontIndex fontIndex, bool centre)
{
	if (mCameraActive)
	{
		position = mpCamera->Transform(position);
	}
	return WriteDouble(int(position.XValue), int(position.YValue), num, colour, fontIndex, centre);
}



// **************************************************************
// Drawing Functions
// **************************************************************

// Draw a picture at the requested location
ErrorType MyDrawEngine::DrawAt(Vector2D position, PictureIndex pic, float scale, float angle, float transparency)
{
	// Find the picture
	std::map<PictureIndex, MyPicture>::iterator picit = mIndexPictureMap.find(pic);

	Vector2D originalPosition = position;
	if (mCameraActive)
	{
		position = mpCamera->Transform(position);
		scale = mpCamera->Transform(scale);
      angle = -angle;
	}

	// If not found
	if(picit==mIndexPictureMap.end())
	{
		ErrorLogger::Writeln(L"Attempting to draw an invalid PictureIndex in DrawAt.");
		WriteText(originalPosition, L"No Image", WHITE);
		return FAILURE;	
	}
	
	MyPicture& rPicture = picit->second;		// Reference to the picture for easy coding

	// Check texture is loaded
	if(!rPicture.mLpdTheTexture)
	{
		ErrorLogger::Writeln(L"Cannot render MyPicture in DrawAt. MyPicture not initialised.");
		return FAILURE;
	}

	// Start drawing
	HRESULT err = mLPSprite->Begin(D3DXSPRITE_ALPHABLEND);		// Alpha Blending requested
	if(FAILED(err))
	{
		ErrorLogger::Writeln(L"Failed to begin sprite render in DrawAt");
		ErrorLogger::Writeln(ERRORSTRING(err));
		return FAILURE;
	}

	// Specify the centre of the sprite - will be (height/2,width/2) unless user has asked for something 
	// different.
	D3DXVECTOR2 centre (rPicture.mCentre.XValue, rPicture.mCentre.YValue);

	// Create a transformation matrix for the requested scale, rotation and position.
	D3DXMATRIX transform;
	D3DXVECTOR2 scaling(scale, scale);
	D3DXVECTOR2 pos;
	pos.x = (position - rPicture.mCentre).XValue;
	pos.y = (position - rPicture.mCentre).YValue;
	D3DXMatrixTransformation2D(&transform, &centre, 0.0, &scaling, &centre, -angle, &pos);
	
	// Set the transformation matrix
	mLPSprite->SetTransform(&transform);

	// Modulate the colour to add transparency
	unsigned int alpha = int(255-255*transparency)%256;
	unsigned int colour = 0xFFFFFF+(alpha<<24);

	// Draw the sprite
	err = mLPSprite->Draw(rPicture.mLpdTheTexture, NULL, NULL, NULL, colour);

	if(FAILED(err))
	{
		ErrorLogger::Writeln(L"Failed to draw sprite in DrawAt");
		ErrorLogger::Writeln(ERRORSTRING(err));
	
		mLPSprite->End();
		return FAILURE;
	}

	// Complete the sprite
	mLPSprite->End();

	return SUCCESS;
}	// DrawAt



// **************************************************************

// Draws a line between the two coordinates
ErrorType MyDrawEngine::DrawLine( Vector2D start,  Vector2D end, unsigned int colour)
{
	if (mCameraActive)
	{
		start = mpCamera->Transform(start);
		end = mpCamera->Transform(end);
	}

	// Vertex array with two vertices needed
		MYVERTEX Vertices[] = {
		{start.XValue, start.YValue, 0.0f, 1.0f, colour,},
		{end.XValue, end.YValue, 0.0f, 1.0f, colour,},
	};

	// Pointer to DirectX Vertex buffer
	LPDIRECT3DVERTEXBUFFER9 vertexBuffer;

	// Create the buffer using my default format
	HRESULT err =mpLPD3DDevice->CreateVertexBuffer(2*sizeof(MYVERTEX),
							   0,
							   MYFVF,
							   D3DPOOL_MANAGED,
							   &vertexBuffer,
							   NULL);
	if(FAILED(err))
	{
		ErrorLogger::Writeln(L"Failed to create a vertex buffer in DrawLine");
		ErrorLogger::Writeln(ERRORSTRING(err));
		return FAILURE;
	}

	VOID* pBuff;		// Pointer to the locked buffer 

	// Lock the buffer
	err= vertexBuffer->Lock(0, 0, (void**)&pBuff, 0);    
	if(FAILED(err))
	{
		ErrorLogger::Writeln(L"Failed to lock vertex buffer in DrawLine");
		ErrorLogger::Writeln(ERRORSTRING(err));
		vertexBuffer->Release();
		return FAILURE;
	}

	// Copy vertices into the buffer
	memcpy(pBuff, Vertices, sizeof(Vertices));

	// Unlock the buffer
	err= vertexBuffer->Unlock();
	if(FAILED(err))
	{
		ErrorLogger::Writeln(L"Failed to unlock vertex buffer in DrawLine");
		ErrorLogger::Writeln(ERRORSTRING(err));
		vertexBuffer->Release();
		return FAILURE;
	}

	// Set the vertex format
	err = mpLPD3DDevice->SetFVF(MYFVF);
	if(FAILED(err))
	{
		ErrorLogger::Writeln(L"Failed to set FVF in DrawLine");
		ErrorLogger::Writeln(ERRORSTRING(err));
		vertexBuffer->Release();
		return FAILURE;
	}
	// Set a default stream
	mpLPD3DDevice->SetStreamSource(0, vertexBuffer, 0, sizeof(MYVERTEX));

	// Draw, as a line list, starting at vertex 0.
	err= mpLPD3DDevice->DrawPrimitive(D3DPT_LINELIST, 0,1);
	if(FAILED(err))
	{
		ErrorLogger::Writeln(L"Failed to draw a primitive in DrawLine");
		ErrorLogger::Writeln(ERRORSTRING(err));
		vertexBuffer->Release();
		return FAILURE;
	}
	// Release the buffer
	vertexBuffer->Release();

	return SUCCESS;
}	// DrawLine



// ******************************************************************

// Fill a circle
ErrorType MyDrawEngine::FillCircle(Circle2D circle, unsigned int colour)
{
	return FillCircle(circle.GetCentre(), circle.GetRadius(), colour);
}

ErrorType MyDrawEngine::FillCircle( Vector2D centre, float radius, unsigned int colour)
{
	if (mCameraActive)
	{
		centre = mpCamera->Transform(centre);
		radius = mpCamera->Transform(radius);
	}

	// Force a minimum radius
	if(radius<0.5) radius =0.5;

	// Calculate how may vertices will be needed to still make
	// the circle look round.
	int numVertices = int(radius * (6 / 10.0)+6);

	// The angle to rotate each vertex by to find the next vertex
	float angle = -6.285f / (numVertices-2);

	// Create an array of the vertex locations
	Vector2D* pPoints = new Vector2D[numVertices];

	// First vertex in the middle
	pPoints[0].XValue = centre.XValue;
	pPoints[0].YValue = centre.YValue;

	// Next vertex is directly below it, 2radius" pixels away
	Vector2D bottom = Vector2D(0, radius);
	// Find the remaining vertex locations - slow
	for(int i=1;i<numVertices;i++)
	{
		pPoints[i] =bottom.rotatedBy(angle*i)+centre;
	}

	// Vertex buffer to hold the vertices
	LPDIRECT3DVERTEXBUFFER9 vertexBuffer;
	HRESULT err;
	// Create a vertex buffer using my format and default values
	err = mpLPD3DDevice->CreateVertexBuffer(sizeof(MYVERTEX)*numVertices,
							   0,
							   MYFVF,
							   D3DPOOL_MANAGED,
							   &vertexBuffer,
							   NULL);

	if(FAILED(err))
	{
		ErrorLogger::Writeln(L"Failed to create a vertex buffer in DrawPointList");
		ErrorLogger::Writeln(ERRORSTRING(err));
		return FAILURE;
	}

	VOID* pBuff;		// Pointer to the locked buffer 
	
	// Lock the buffer for writing
	err= vertexBuffer->Lock(0, 0, (void**)&pBuff, 0);  
	if(FAILED(err))
	{
		ErrorLogger::Writeln(L"Failed to lock the vertex buffer in DrawPointList");
		ErrorLogger::Writeln(ERRORSTRING(err));
		return FAILURE;
	}

	// Copy vertices into the buffer
	MYVERTEX* pVertices = (MYVERTEX*)pBuff;
	for( int i=0;i<numVertices;i++)
	{
		pVertices[i].x = pPoints[i].XValue;
		pVertices[i].y = pPoints[i].YValue;
		pVertices[i].z = 0.0f;
		pVertices[i].rhw=1.0f;
		pVertices[i].colour = colour;
	}

	// Writing finished - unlock
	vertexBuffer->Unlock();

	// Set my vertex format and a default stream source
	mpLPD3DDevice->SetFVF(MYFVF);
	mpLPD3DDevice->SetStreamSource(0, vertexBuffer, 0, sizeof(MYVERTEX));

	// Draw using a triangle fan
	err= mpLPD3DDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0,numVertices-2);
	if(FAILED(err))
	{
		ErrorLogger::Writeln(L"Failed to draw primitive in DrawPoint");
		ErrorLogger::Writeln(ERRORSTRING(err));
		vertexBuffer->Release();
		delete[] pPoints;
		return FAILURE;
	}
	// Release the buffer
	vertexBuffer->Release();
	// Delete the array
	delete[] pPoints;
	return SUCCESS;
}	// FillCircle



//****************************************************************

// Fills a rectangle on the screen
ErrorType MyDrawEngine::FillRect(Rectangle2D destinationRect, unsigned int colour, float angle)
{

	if (mCameraActive)
	{
		destinationRect = mpCamera->Transform(destinationRect);
	}

	// Four corners
	Vector2D p1, p2, p3, p4;
	if (angle!=0.0f)		// Need to rotate due to angle
	{
		p1 = (destinationRect.GetBottomRight()-destinationRect.GetCentre()).rotatedBy(-angle)+destinationRect.GetCentre();
		p2 = (destinationRect.GetTopRight()-destinationRect.GetCentre()).rotatedBy(-angle)+destinationRect.GetCentre();
		p3 = (destinationRect.GetBottomLeft()-destinationRect.GetCentre()).rotatedBy(-angle)+destinationRect.GetCentre();
		p4 = (destinationRect.GetTopLeft()-destinationRect.GetCentre()).rotatedBy(-angle)+destinationRect.GetCentre();
	}
	else		// No rotation needed
	{
		p1 = destinationRect.GetBottomRight();
		p2 = destinationRect.GetTopRight();
		p3 = destinationRect.GetBottomLeft();
		p4 = destinationRect.GetTopLeft();
	}
	// Put corners into a vertex array - get the order right
	MYVERTEX Vertices[] = {
		{p1.XValue, p1.YValue, 0.0f, 1.0f, colour,},
		{p2.XValue, p2.YValue, 0.0f, 1.0f, colour,},
		{p3.XValue, p3.YValue, 0.0f, 1.0f, colour,},
		{p4.XValue, p4.YValue, 0.0f, 1.0f, colour,},
	};

	// Create a vertex buffer for drawing
	LPDIRECT3DVERTEXBUFFER9 vertexBuffer;
	HRESULT err;
	err = mpLPD3DDevice->CreateVertexBuffer(4*sizeof(MYVERTEX),
							   0,
							   MYFVF,
							   D3DPOOL_MANAGED,
							   &vertexBuffer,
							   NULL);

	if(FAILED(err))
	{
		ErrorLogger::Writeln(L"Failed to create a vertex buffer in Fill Rect");
		ErrorLogger::Writeln(ERRORSTRING(err));
		return FAILURE;
	}

	VOID* pBuff;		// Pointer to the locked buffer 

	// Lock buffer for writing
	err= vertexBuffer->Lock(0, 0, (void**)&pBuff, 0);  
	if(FAILED(err))
	{
		ErrorLogger::Writeln(L"Failed to lock the vertex buffer in FillRect");
		ErrorLogger::Writeln(ERRORSTRING(err));
		return FAILURE;
	}

	// Copy vertices into the buffer
	memcpy(pBuff, Vertices, sizeof(Vertices));

	// Writing complete - unlock
	vertexBuffer->Unlock();

	// Set my vertex format and a default stream
	mpLPD3DDevice->SetFVF(MYFVF);
	mpLPD3DDevice->SetStreamSource(0, vertexBuffer, 0, sizeof(MYVERTEX));

	// Draw using a triangle strip
	err= mpLPD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0,2);
	if(FAILED(err))
	{
		ErrorLogger::Writeln(L"Failed to draw primitive in FillRect");
		ErrorLogger::Writeln(ERRORSTRING(err));
		vertexBuffer->Release();
		return FAILURE;
	}
	// Release the buffer
	vertexBuffer->Release();

	return SUCCESS;
}

// ******************************************************************


ErrorType MyDrawEngine::BlendRect(Rectangle2D destinationRect, unsigned int colour, float transparency, float angle)
{

	if (mCameraActive)
	{
		destinationRect = mpCamera->Transform(destinationRect);
	}

	// Calculate transparency as an integer 0-255
	int trans = int(255 - 255 * transparency)%256;

	// Remove existing alpha component
	colour = colour & 16777215;

	// Insert requested alpha
	colour = colour + (trans<<24);

	// Four corners
	Vector2D p1, p2, p3, p4;
	if (angle!=0.0f)		// Rotate using angle
	{
		p1 = (destinationRect.GetTopRight()-destinationRect.GetCentre()).rotatedBy(angle)+destinationRect.GetCentre();
		p2 = (destinationRect.GetBottomRight()-destinationRect.GetCentre()).rotatedBy(angle)+destinationRect.GetCentre();
		p3 = (destinationRect.GetTopLeft()-destinationRect.GetCentre()).rotatedBy(angle)+destinationRect.GetCentre();
		p4 = (destinationRect.GetBottomLeft()-destinationRect.GetCentre()).rotatedBy(angle)+destinationRect.GetCentre();
	}
	else			// No need to rotate
	{
		p1 = destinationRect.GetTopRight();
		p2 = destinationRect.GetBottomRight();
		p3 = destinationRect.GetTopLeft();
		p4 = destinationRect.GetBottomLeft();
	}
	// Vertex array
	MYVERTEX Vertices[] = {
		{p1.XValue, p1.YValue, 0.0f, 1.0f, colour,},
		{p2.XValue, p2.YValue, 0.0f, 1.0f, colour,},
		{p3.XValue, p3.YValue, 0.0f, 1.0f, colour,},
		{p4.XValue, p4.YValue, 0.0f, 1.0f, colour,},
	};

	// Create a buffer
	LPDIRECT3DVERTEXBUFFER9 vertexBuffer;
	HRESULT err;
	err = mpLPD3DDevice->CreateVertexBuffer(4*sizeof(MYVERTEX),
							   0,
							   MYFVF,
							   D3DPOOL_MANAGED,
							   &vertexBuffer,
							   NULL);

	if(FAILED(err))
	{
		ErrorLogger::Writeln(L"Failed to create a vertex buffer in BlendRect");
		ErrorLogger::Writeln(ERRORSTRING(err));
		return FAILURE;
	}

	VOID* pBuff;		// Pointer to the locked buffer 

	// Lock buffer for writing
	err= vertexBuffer->Lock(0, 0, (void**)&pBuff, 0);  
	if(FAILED(err))
	{
		ErrorLogger::Writeln(L"Failed to lock the vertex buffer in BlendRect");
		ErrorLogger::Writeln(ERRORSTRING(err));
		return FAILURE;
	}

	// Copy vertices into the buffer

	memcpy(pBuff, Vertices, sizeof(Vertices));

	// Unlock, since writing is complete
	vertexBuffer->Unlock();

	// Set a vertex format and stream source
	mpLPD3DDevice->SetFVF(MYFVF);
	mpLPD3DDevice->SetStreamSource(0, vertexBuffer, 0, sizeof(MYVERTEX));

	// Enable alpha blending
	mpLPD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	mpLPD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	mpLPD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	mpLPD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// Draw rectangle as a triangle strip - two triangles, starting at zero.
	err= mpLPD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0,2);
	if(FAILED(err))
	{
		ErrorLogger::Writeln(L"Failed to draw primitive in BlendRect");
		ErrorLogger::Writeln(ERRORSTRING(err));
		vertexBuffer->Release();
		return FAILURE;
	}
	vertexBuffer->Release();

	return SUCCESS;
}	// Blend rectangle

// **************************************************************

// Draws a single dot
ErrorType MyDrawEngine::DrawPoint(Vector2D point, unsigned int colour)
{

	if (mCameraActive)
	{
		point = mpCamera->Transform(point);
	}
	// An "array" with one entry
	MYVERTEX Vertices[] = {
		{point.XValue, point.YValue, 0.0f, 1.0f, colour,},
	};

	// Create a vertex buffer
	LPDIRECT3DVERTEXBUFFER9 vertexBuffer;
	HRESULT err;
	err = mpLPD3DDevice->CreateVertexBuffer(sizeof(MYVERTEX),
							   0,
							   MYFVF,
							   D3DPOOL_MANAGED,
							   &vertexBuffer,
							   NULL);

	if(FAILED(err))
	{
		ErrorLogger::Writeln(L"Failed to create a vertex buffer in DrawPoint");
		ErrorLogger::Writeln(ERRORSTRING(err));
		return FAILURE;
	}

	VOID* pBuff;		// Pointer to the locked buffer 

	err= vertexBuffer->Lock(0, 0, (void**)&pBuff, 0);  
	if(FAILED(err))
	{
		ErrorLogger::Writeln(L"Failed to lock the vertex buffer in DrawPoint");
		ErrorLogger::Writeln(ERRORSTRING(err));
		return FAILURE;
	}

	// Copy vertices into the buffer

	memcpy(pBuff, Vertices, sizeof(Vertices));

	vertexBuffer->Unlock();

	// Set a vertex format and stream source
	mpLPD3DDevice->SetFVF(MYFVF);
	mpLPD3DDevice->SetStreamSource(0, vertexBuffer, 0, sizeof(MYVERTEX));

	// Draw a point list, with a single entry
	err= mpLPD3DDevice->DrawPrimitive(D3DPT_POINTLIST, 0,1);
	if(FAILED(err))
	{
		ErrorLogger::Writeln(L"Failed to draw primitive in DrawPoint");
		ErrorLogger::Writeln(ERRORSTRING(err));
		vertexBuffer->Release();
		return FAILURE;
	}
	vertexBuffer->Release();
	return SUCCESS;
}

// **************************************************************

ErrorType MyDrawEngine::DrawPointList(Vector2D points[], unsigned int colours[], unsigned int numPoints)
{
	if(numPoints<=0)
	{
		ErrorLogger::Writeln(L"Requested less than one point in DrawPointList.");
		return FAILURE;
	}

	// Create a vertex buffer
	LPDIRECT3DVERTEXBUFFER9 vertexBuffer;
	HRESULT err;
	err = mpLPD3DDevice->CreateVertexBuffer(sizeof(MYVERTEX)*numPoints,
							   0,
							   MYFVF,
							   D3DPOOL_MANAGED,
							   &vertexBuffer,
							   NULL);

	if(FAILED(err))
	{
		ErrorLogger::Writeln(L"Failed to create a vertex buffer in DrawPointList");
		ErrorLogger::Writeln(ERRORSTRING(err));
		return FAILURE;
	}

	VOID* pBuff;		// Pointer to the locked buffer 

	// Lock buffer for writing
	err= vertexBuffer->Lock(0, 0, (void**)&pBuff, 0);  
	if(FAILED(err))
	{
		ErrorLogger::Writeln(L"Failed to lock the vertex buffer in DrawPointList");
		ErrorLogger::Writeln(ERRORSTRING(err));
		return FAILURE;
	}

	// Copy vertices into the buffer
	MYVERTEX* pVertices = (MYVERTEX*)pBuff;
	for(unsigned int i=0;i<numPoints;i++)
	{
		Vector2D p = points[i];
		if (mCameraActive)
		{
			p = mpCamera->Transform(p);
		}
		pVertices[i].x = p.XValue;
		pVertices[i].y = p.YValue;
		pVertices[i].z = 0.0f;
		pVertices[i].rhw=1.0f;
		pVertices[i].colour = colours[i];
	}

	// Finished writing
	vertexBuffer->Unlock();

	// Set a vertex format and stream source
	mpLPD3DDevice->SetFVF(MYFVF);
	mpLPD3DDevice->SetStreamSource(0, vertexBuffer, 0, sizeof(MYVERTEX));

	// Draw using a point list
	err= mpLPD3DDevice->DrawPrimitive(D3DPT_POINTLIST, 0,numPoints);
	if(FAILED(err))
	{
		ErrorLogger::Writeln(L"Failed to draw primitive in DrawPoint");
		ErrorLogger::Writeln(ERRORSTRING(err));
		vertexBuffer->Release();
		return FAILURE;
	}
	vertexBuffer->Release();
	return SUCCESS;
}	// DrawPointList


// **************************************************************
// Inner struct constructors
// **************************************************************

// MyFont constructor - just set pointer to NULL
MyDrawEngine::MyFont::MyFont()
{
	mFont = nullptr;
}

// **************************************************************

// MyPicture constructor -  set pointer to NULL and dimensions to zero
MyDrawEngine::MyPicture::MyPicture()
{
	mLpdTheTexture = nullptr;
	mWidth =0;
	mHeight=0;
}


