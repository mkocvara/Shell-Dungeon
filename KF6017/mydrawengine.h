// mydrawengine.h
// Shell engine version 2023
// Chris Rook
// modified 06/09/2019
//		Updating LoadPicture, FindPicture, AddFont to const wchar_t to support string literals in parameters

// modified 04/02/2022
//		Changes to ReleasePicture and ReleaseBitmaps to fix a bug where filenames were not being removed from the filename map

// Modified 25/4/2023
//		Added DrawAt overload with two scales - X and Y.

#pragma once

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxguid.lib")  

#include <d3d9.h>		// directX draw
#include <d3dx9.h>		// extra directX draw stuff   
#include "ErrorLogger.h"
#include "vector2d.h"
#include "shapes.h"
#include <vector>
#include <map>
#include "errortype.h"
#include "string"
#include <memory>

class Camera;

// Macros ***************************************************
// Colour system
#define _RGB565(r,g,b)  ((((b>>3)%32)) + (((g>>2)%64)<<5) + (((r>>3)%32)<<11))
#define _XRGB(r,g,b) ((255<<24)+(r<<16) + (g<<8) +b)

// The class spec *******************************************

// This class provides a simple set of graphics functions that
// use directX for a computer game, using 32-bit graphics.
// The functions are organised in a class for convenience, rather
// than for OO principles.


// PictureIndex is just an int
// Used to index each image loaded
typedef int PictureIndex;

// Font index is just an int
// Used to index each font specified
typedef int FontIndex;


// Class to handle 2D drawing on screen
class MyDrawEngine
{

private:

	// Defines the vertex format used in Direct3D
	struct MYVERTEX
	{
		FLOAT x, y, z, rhw;
		DWORD colour;
	};

#define MYFVF (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)

	// Inner struct to store information about each picture
	struct MyPicture
	{
		LPDIRECT3DTEXTURE9  mLpdTheTexture;	// The surface that MyPicture encapsulates
		std::wstring mSourceFileName;      // The file name of the loaded image
		Vector2D mCentre;                  // Cordinates of the "centre" of the image
										  // by default this is the centre of the square file
										  // but it can be adjusted so that objects "rotate" around
										  // some other centre
		int mWidth;                        // Width of the image in pixels
		int mHeight;                       // Height of the image in pixels

	   // Public methods
		//  Internal pointer to the texture is set to NULL
		MyPicture();
	};

	// Inner struct to store information about each font
	struct MyFont
	{
		LPD3DXFONT mFont;		      // Pointer to the font
		std::wstring mFontName;		// Name of the font
		int mHeight;			         // Height of the font
		bool mBold;			         // If true, font will be bold
		bool mItalic;			         // If true, font will be italicised

		// Sets the pointer to nullptr
		MyFont();
	};

	HWND mHwnd;								// The handle to the window
	IDirect3D9* mpLPD3D;					   // Pointer to direct draw
	IDirect3DDevice9* mpLPD3DDevice;		// Pointer to the D3D device
	LPD3DXSPRITE mLPSprite;				// Sprite to draw pictures

	int mScreenWidth;						// Height of the screen
	int mScreenHeight;						// Width of the screen
	int mNativeScreenWidth;				// Height of the primary monitor at start
	int mNativeScreenHeight;				// Width of the primary monitor at start

	bool mCameraActive;					   // If true, drawing will transform using camera settings. Default is true.

	bool mStarted = false;

	bool mFullScreen;						// True if full screen. False otherwise
	std::map<PictureIndex, MyPicture> mIndexPictureMap;	      // Map of MyPicture objects
	std::map<std::wstring, PictureIndex> mFilenameIndexMap;		// Map of filenames
	std::map<FontIndex, MyFont> mIndexFontMap;			         // Map of Font objects
	PictureIndex mNextPictureIndex;		// The index of the next font to be added	
	FontIndex mNextFont;					// The index of the next font to be added

	// Postcondition:	The primary surface, the buffer, the clipper and DirectDraw have been released.
	// Returns:			SUCCESS
	ErrorType Release();

	// Reloads a specified picture - called within ReloadBitmaps
	// Parameters:
	//    pic    The index of the required picture
	ErrorType ReloadPicture(PictureIndex pic);

	// Releases all Bitmaps. Used when resetting the device or
	// on destruction.
	void ReleaseBitmaps();

	// Reloads all Bitmaps. Used after resetting the device.
	void ReloadBitmaps();

	// Releases all Bitmaps. Used when resetting the device or
	// on destruction.
	void ReleaseFonts();

	// Reloads all Fonts. Used after resetting the device.
	void ReloadFonts();

public:
	std::unique_ptr<Camera> mpCamera;		// Camera objects is used to translate world coordinates to/from
										// screen coordinates

	// Parameters:
	//		hwnd		The handle to the application's window.
	// Note: After creating a new instance, it should be started by calling MyDrawEngine::Start() instead.
	MyDrawEngine(HWND hwnd);

	// Singleton destructor - calls "Release()"
	~MyDrawEngine();

	// Some standard colours.
	typedef unsigned int Colour;
	static const Colour BLACK = 0;
	static const Colour RED = D3DCOLOR_ARGB(255, 255, 0, 0);
	static const Colour GREEN = D3DCOLOR_ARGB(255, 0, 255, 0);
	static const Colour BLUE = D3DCOLOR_ARGB(255, 0, 0, 255);
	static const Colour DARKRED = D3DCOLOR_ARGB(255, 128, 0, 0);
	static const Colour DARKGREEN = D3DCOLOR_ARGB(255, 0, 128, 0);
	static const Colour DARKBLUE = D3DCOLOR_ARGB(255, 0, 0, 128);
	static const Colour LIGHTRED = D3DCOLOR_ARGB(255, 255, 128, 128);
	static const Colour LIGHTGREEN = D3DCOLOR_ARGB(255, 128, 255, 128);
	static const Colour LIGHTBLUE = D3DCOLOR_ARGB(255, 128, 128, 255);
	static const Colour WHITE = D3DCOLOR_ARGB(255, 255, 255, 255);
	static const Colour YELLOW = D3DCOLOR_ARGB(255, 255, 255, 0);
	static const Colour CYAN = D3DCOLOR_ARGB(255, 0, 255, 255);
	static const Colour PURPLE = D3DCOLOR_ARGB(255, 255, 0, 255);
	static const Colour GREY = D3DCOLOR_ARGB(255, 128, 128, 128);

	// Public methods ***************************************

	// Returns:
	//  The method returns true if an instance has been started and false if not.
	bool IsStarted() const;

	// Precondition:
	//	filename is a NULL-terminated w_string
	// Postcondition:
	//	If filename is the name of a valid bitmap, jpg, png,
	//	the file will be loaded. The method returns a PictureIndex
	//	which can be used in other methods to draw the picture.
	//	If filename is not valid, or file loading fails for other
	//	reasons, the method returns zero, and an error is written
	//	to the log file.
	// Returns:
	//  The method returns a PictureIndex
	//	which can be used in other methods to draw the picture.
	//  Returns zero if file loading fails.
	// Notes:
	//	For best results, the file should be 1x1, 2x2, 4x4, 8x8, 16x16
	//	32x32, 64x64, etc.
	//	Transparency is supported on the alpha channel for file formats
	//	that support it.
	PictureIndex LoadPicture(const wchar_t* filename);

	// Precondition:
	//	filename is a NULL-terminated w_string
	// Postcondition:
	//	If filename is the name of a valid bitmap that has previously
	//  been loaded by the MyDrawEngine, the  method will return the 
	//  corresponding picture index. Otherwise, it will
	//  return a negative number.
	// Returns:
	//  The method returns the PictureIndex of the Image previously loaded
	//   with the same file name. If the filename has not been previously
	//   loaded, the method returns a negative picture index
	// Notes:
	//	For best results, the file should be 1x1, 2x2, 4x4, 8x8, 16x16
	//	32x32, 64x64, etc.
	//	Transparency is supported on the alpha channel for file formats
	//	that support it.
	PictureIndex FindPicture(const wchar_t* filename);

	// Postcondition:
	//  The image referenced by "pic" is released from memory.
	//  If "pic" is invalid or the picture is already unloaded,
	//  a message is written to the error log file.
	// Notes:
	//  Once this has been called, the supplied PictureIndex will no longer
	//  be usable. If it is stored in a variable, set it to zero.
	void ReleasePicture(PictureIndex pic);

	// Postcondition:
	//  A kraken has been released.
	void ReleaseKraken();

	// Postcondition:
	//	If FontName is a registered font, A new font will be loaded, and the 
	//  method will return a FontIndex referecing font for use in the
	//  WriteText, WriteDouble and WriteInt methods.
	// Else, returns zero
	// Parameters:
	//	FontName - The name of the font to be loaded - such as "Arial"
	//  Height - the hieght of the font in pixels
	//  bold - true if you want the font to be bold
	//  italic - true if you want the font to be in italics
	// Notes:
	//  The default font has a FontIndex of zero. It is an Arial font with
	//  a height of 24.
	FontIndex AddFont(const wchar_t* FontName, int height, bool bold, bool italic);

	// Postcondition:
	//  The Direct3D device has been reset and SUCCESS has been returned.
	//  OR
	//  FAILURE is returned and a message is written to the log file.
	// Notes:
	//  This is called automatically when the screen is switched between
	//  full screen and windowed or vice versa.
	//  You probably won't need to call this unless you have problems
	//  with the screen getting minimised.
	ErrorType ResetDevice();

	// Postcondition:
	//  The Direct3D device has been switched to the specified resolution and
	//   SUCCESS has been returned. Or FAILURE has been returned.
	// Side effects: The device is reset and all fonts and pictures will be
	//  reloaded.
	// Notes:
	//  A common error it to request a resolution that the device cannot support.
	//  This method does not check for valid resolution. It just returns FAILURE.
	//  If the resolution change fails, the device attempts to return to the previous 
	// resolution.
	// In fullscreen mode, the game will occupy all of the primary monitor, using
	// the requested resolution
	// In windowed mode, the game will run in a window using the specifed size
	// with the screen at the native resolution
	ErrorType ChangeResolution(int width, int height);

	// Postcondition:
	//  The Direct3D device has been switched to the resolution of the primary
	// monitor and
	//   SUCCESS has been returned. Or FAILURE has been returned.
	// Side effects: The device is reset and all fonts and pictures will be
	//  reloaded.
	ErrorType ChangeToNativeResolution();

	// Postcondition:
	//  If "pic" refers to a valid picture, "height" and "width" are set
	//  to the height and width of the image.
	// Othewise, "height" and "width" are unchanged.
	void GetDimensions(PictureIndex pic, int& height, int& width);

	// Precondition:	A window for the application has been created
	// Postcondition:	The window has been started.
	//					The primary surface and the back buffer have been initialised and cleared.
	// Parameters:
	//		hwnd		The handle to the application's window.
	//		bFullScreen	Bool determining if the application window is made fullscreen.
	// Must be called on a newly created instance before it can be used.
	ErrorType Start(bool bFullScreen);

	// Precondition:	shutdown() has not been called
	//					Neither primary nor buffer is locked.
	// Postcondition:	Primary surface and the back buffer have been flipped.
	//					(function will wait until flip is possible)
	// Returns:			SUCCESS
	ErrorType Flip();

	// Preconditions	thePicture has been initialised to contain an image
	// Postcondition	The whole image in theSurface will be placed
	//					on the back buffer. (Subject to clipping.)
	//					The centre of the image will be placed at the x,y coordinates specified
	//	Returns			SUCCESS if blit successful. FAILURE otherwise.
	// Parameters:		position - the screen location of the centre of the picture
	//					pic - the PictureIndex of the image to be drawn
	//					scale - the scale of the image. The image will be scaled from its "centre"
	//						(by default the mathematical centre of the surface)
	//					angle - the anticlockwise rotation of the image in radians
	//						the image will be rotated around its centre (by
	//						default the mathematical centre of the surface)
	//					transparency - the transparency of the image. 0.0 is opaque. 1.0 is
	//						fully transparent. Behaviour for transparency values greater
	//						than 1.0 or less than 0.0 is undefined.
	ErrorType DrawAt(Vector2D position, PictureIndex pic, float scale = 1.0, float angle = 0, float transparency = 0);


	// Preconditions	thePicture has been initialised to contain an image
	// Postcondition	The whole image in theSurface will be placed
	//					on the back buffer. (Subject to clipping.)
	//					The centre of the image will be placed at the x,y coordinates specified.
	//					The image will be scaled on it's own x,y axes according to the parameters sent.
	//	Returns			SUCCESS if blit successful. FAILURE otherwise.
	// Parameters:		position - the screen location of the centre of the picture
	//					pic - the PictureIndex of the image to be drawn
	//					scalex - the horizontal scale of the image. The image will be scaled from its "centre"
	//						(by default the mathematical centre of the surface)
	// 					scaley - the vertical scale of the image. The image will be scaled from its "centre"
	//						(by default the mathematical centre of the surface)
	//					angle - the anticlockwise rotation of the image in radians
	//						the image will be rotated around its centre (by
	//						default the mathematical centre of the surface)
	//					transparency - the transparency of the image. 0.0 is opaque. 1.0 is
	//						fully transparent. Behaviour for transparency values greater
	//						than 1.0 or less than 0.0 is undefined.
	ErrorType DrawStretchedAt(Vector2D position, PictureIndex pic, float scalex, float scaley, float angle, float transparency);

	// Precondition:	A window for the application has been created
	//					Direct3D has not already been initialised.
	// Postcondition:	A Direct3D interface has been created.
	//					The screen has been put into full-screen, exclusive mode.
	// Returns: FAILURE if the startup fails for any reason. A message will be written to the log file
	//			SUCCESS otherwise
	ErrorType StartWindow();

	// Postcondition:	The application is switched to windowed mode. If the window is
	//					not currently in windowed mode, the bitmaps and fonts will
	//					be reloaded.
	// Returns:			SUCCESS if the switch was successful (or if already in windowed mode)
	//					FAILURE otherwise  - a message is written to the log file.
	ErrorType GoWindowed();

	// Postcondition:	The application is switched to fullscreen mode. If the window is
	//					not currently in fullscreen mode, the bitmaps and fonts will
	//					be reloaded.
	// Returns:			SUCCESS if the switch was successful (or if already in fullscreen mode)
	//					FAILURE otherwise  - a message is written to the log file.
	ErrorType GoFullScreen();


	// Postcondition	A rectangle in the back buffer will be filled with the specified
	//					colour. (If partially on-screen)
	// Parameters:		destinationRect - a rectangle specifying the area to be filled.
	//								the top of the rectangle must be a smaller number than
	//								the bottom. 
	//								the left side of the rectangle must be a smaller number than
	//								the right side. 
	//					colour - an integer representing the colour to be used
	//					angle - an angle to tilt the rectangle, in radians clockwise
	//							rotation occurs around the centre of the rectangle.
	// Returns			SUCCESS if fill successful. FAILURE otherwise
	ErrorType FillRect(Rectangle2D destinationRect, unsigned int colour, float angle = 0);

	// Postcondition	A rectangle in the back buffer will be filled with the specified
	//					colour. (If partially on-screen). The rectangle will be partially
	//					transparent with regards to images already on the back buffer.
	// Parameters:		destinationRect - a rectangle specifying the area to be filled.
	//								the top of the rectangle must be a smaller number than
	//								the bottom. 
	//								the left side of the rectangle must be a smaller number than
	//								the right side. 
	//					colour - an integer representing the colour to be used
	//					transparency - a number from 0 to 1.0 to indicate how transparent
	//								the rectangle should be. 0 is opaque. 1 is fully transparent
	//								(i.e. invisible)
	//					angle - an angle to tilt the rectangle, in radians clockwise
	//							rotation occurs around the centre of the rectangle.
	// Returns			SUCCESS if fill successful. FAILURE otherwise
	ErrorType BlendRect(Rectangle2D destinationRect, unsigned int colour, float transparency, float angle = 0);

	// Postcondition	A point of the specified colour has been
	//					plotted at the location specified on on the back buffer.
	// Returns			SUCCESS
	ErrorType DrawPoint(Vector2D point, unsigned int colour);

	// Precondition		point and colour are arrays with size smaller or equal to numPoints
	// Postcondition	numPoints points of the specified colour has been
	//					plotted at x1,y1 on the back buffer.
	// Returns			SUCCESS
	ErrorType DrawPointList(Vector2D point[], unsigned int colour[], unsigned int numPoints);

	// Precondition		The back buffer is locked.
	// Postcondition	A line between start and end
	//					has been plotted on the back buffer.
	// Returns			SUCCESS
	ErrorType DrawLine(Vector2D start, Vector2D end, unsigned int colour);

	// Postcondition	A circle centred on centre with the radius "radius" has been
	//					filled on the back buffer with the specified colour
	// Returns			SUCCESS
	ErrorType FillCircle(Vector2D centre, float radius, unsigned int colour);

	// Postcondition	A circle has been
	//					filled on the back buffer with the specified colour
	// Returns			SUCCESS
	ErrorType FillCircle(Circle2D circle, unsigned int colour);

	// Postcondition	The back buffer is cleared (all black)
	// Returns			SUCCESS if successful FAILURE otherwise.
	ErrorType ClearBackBuffer();

	//	Postcondition:
	//		The "centre" of the specified rectangle is set to the specified location.
	//		All future calls to DrawAt will use the centre specified for placing the 
	//		image on screen and for rotation.
	// Notes:
	//		By default the centre is the middle of the picture (HEIGHT/2, WIDTH/2)
	ErrorType SetCentre(PictureIndex pic, Vector2D centre);

	// Postcondition:
	//		If activate == true , the engine will use the camera transformation for
	//		all subsequent drawing operations (DrawAt, FillRect, etc)
	//		If activate == false , the engine will NOT use the camera transformation for
	//		all subsequent drawing operations (DrawAt, FillRect, etc)
	void UseCamera(bool activate);


	// This method writes text to the screen at the specified coordinates.
	// Precondition:
	//  text is a null-terminated string
	// Postcondition:
	//	Text is written to the screen at the specified coordinates, using the
	//  requested font and colour.
	//  OR FAILURE has been returned. (Usually means an invalid FontIndex)
	// Parameters:
	//  x,y Screen coordinates to write the text (these will be the position of the top
	//		left position of the writing rectangle (except if centre is true).
	//  text - A null terminated string containing the text to be written.
	//  colour - 32-bit colour for the colour of the text to be written (no transparency).
	//  fontIndex - the index of the font to be used. 0 uses a default 24-point Arial font.
	//  centre - if true, the rectangle with text is centred on the passed coordinates
	// Returns:
	//	FAILURE if rendering failed or the fontIndex was invalid.
	// SUCCESS otherwise
	// NOTE: x,y version uses raw screen coordinates. position version uses camera if active
	//    but will not scale
	ErrorType WriteText(int x, int y, const wchar_t text[], int colour, FontIndex fontIndex = 0, bool centre = false);
	ErrorType WriteText(Vector2D position, const wchar_t text[], int colour, FontIndex fontIndex = 0, bool centre = false);


	// These methods writes a  number to the screen at the specified coordinates.
	// Precondition:
	//  text is a null-terminated string
	// Postcondition:
	//	Text is written to the screen at the specified coordinates, using the
	//  requested font and colour.
	//  OR FAILURE has been returned. (Usually means an invalid FontIndex)
	// Parameters:
	//  x,y Screen coordinates to write the text (these will be the position of the top
	//		left position of the writing rectangle (except if centre is true).
	//  num - The number to the written
	//  colour - 32-bit colour for the colour of the number to be written (no transparency).
	//  fontIndex - the index of the font to be used. 0 uses a default 24-point Arial font.
	//  centre - if true, the rectangle with text is centred on the passed coordinates
	// Returns:
	//	FAILURE if rendering failed or the fontIndex was invalid.
	// SUCCESS otherwise
	// NOTE: x,y version uses raw screen coordinates. position version uses camera if active
	//     but will not scale
	ErrorType WriteInt(int x, int y, int num, int colour, FontIndex fontIndex = 0, bool centre = false);
	ErrorType WriteDouble(int x, int y, double num, int colour, FontIndex fontIndex = 0, bool centre = false);
	ErrorType WriteInt(Vector2D position, int num, int colour, FontIndex fontIndex = 0, bool centre = false);
	ErrorType WriteDouble(Vector2D position, double num, int colour, FontIndex fontIndex = 0, bool centre = false);

	// Returns a string describing the directDraw error for most HRESULTs sent to it
	//static const char* ErrorString(HRESULT err);

	// Returns a bool that returns true if the game is displayed in full screen mode
	// or false if the game is in windowed mode
	bool IsWindowFullScreen(void) const;

	// Returns the height and width of the current screen resolution
	int GetScreenWidth() const
	{
		return mScreenWidth;
	}
	int GetScreenHeight() const
	{
		return mScreenHeight;
	}

	// Returns true if the specified point is visible from the current camera position.
	// If the camera has been deativated using UseCamera(false), it will report
	// using raw screen coordinates
	bool IsOnCamera(Vector2D point) const;

	// Returns true if the specified point is fully or partially visible from the current camera position
	// If the camera has been deativated using UseCamera(false), it will report
	// using raw screen coordinates
	bool IsOnCamera(const IShape2D& shape)const;

	// Returns a rectangle describing the area of the world that is visible from the current camera position
	// If the camera has been deativated using UseCamera(false), it will report
	// the screen area
	Rectangle2D GetViewport() const;


};	// End of class definition

// **************************************************************



