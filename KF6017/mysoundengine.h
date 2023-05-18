// MySoundEngine.cpp
// Shell engine version 2020
// Chris Rook
// Modified 8/11/2018
//		Start() now returns an ErrorType, to be consistent with MyDrawEngine

// Modified 4/2/2022
//		Major rewrite. MySound is now a class and handles more functionality
//		Each sound buffer is loaded four times to allow simultaneous playing
//		Each sound can only be loaded once. If the same sound is loaded a second time,
//		LoadWAV will return the index of the existing sound

// Modified by 19002175
//		Changed from singleton to normal instanced when implementing ServiceManager.
//		Renamed variables to a style consistent with other work.
//		Changed constant number of sound buffers to a dynamic one using std::vector<>, 
//			creating new ones when there's demand for them and reusing ones that aren't active at the time.

#pragma once

#include <string>
#include <dsound.h>		// directX draw
#include "errortype.h"
#include <map>
#include <memory>
#include <vector>

typedef unsigned int SoundIndex;
typedef unsigned int MusicIndex;

#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "winmm.lib")

// Class to load an play .wav files
class MySoundEngine
{
	class MySound
	{
	public:
		// This is a long pointer to the DSound buffer. DirectX
		// functions can be called on it, once it it initialised.
		std::vector<LPDIRECTSOUNDBUFFER> mLpdSoundBuffers;
		bool mIsLoaded = false;
		std::wstring mSourceFileName;

		MySound();

		void Release();
		~MySound();
		LPDIRECTSOUNDBUFFER GetBuffer(IDirectSound8* lpds);
		ErrorType Stop();
		ErrorType SetVolume(int vol);
		ErrorType Play(bool looping, IDirectSound8* lpds);
		ErrorType SetPan(int pan);
		ErrorType LoadWav(const wchar_t* filename, IDirectSound8* lpds);
		ErrorType SetFrequency(int frequency);


	};
	std::map<SoundIndex, MySound> mIndexSoundsMap;	// Map of MySound objects
	std::map<std::wstring, SoundIndex> mFilenameIndexMap;		// Map of filenames
	SoundIndex mNextSoundIndex;

private:
	// Releases the memory and COM objects=. Needs to be called
	// before closing the program. (Gets called by the constructor,
	// but best to call it explicitly as well.)
	// Returns FAILURE if the COM object was not NULL already
	// Returns SUCCESS otherwise
	ErrorType Release();

	// Finds a reference to the specified MySound in the SoundIndex map.
	// If not found, will return a reference to the empty MySound 
	MySound& FindSound(SoundIndex);

	IDirectSound8 *mpLpds;

	MySound mEmptySound;

public:
	// hwnd is the handle of the main window
	// Precondition:
	//	DirectSound is installed on the computer
	MySoundEngine(HWND hwnd);

	// Calls Release().
	~MySoundEngine();

	// Returns a string describing the directDraw error for most HRESULTs sent to it
	static const wchar_t* ErrorString(HRESULT err);

	// Loads a wave file and returns a SoundIndex that can be used to
	// use that sound in other methods.
	// If the file has previously been loaded, it will not be loaded again. Instead, this function
	// returns the SoundIndex of the existing file.
	// If the file fails to load (bad format or not found), returns -1.
	// Returns: A SoundIndex to the loaded file or -1.
	// Parameters: filename - Null terminated string with the filename of the file to load
	SoundIndex LoadWav(const wchar_t* filename);

	// Unloads the specified sound from memory
	// Returns SUCCESS if the sound was found. FAILURE otherwise
	// Parameters: sound - the PictureIndex of the sound to unload
	ErrorType Unload(SoundIndex sound);

	// Unloads all sounds from memory
	// Returns SUCCESS always
	ErrorType UnloadAllSounds();

	// Sets the volume of the specified sound. 
	// 0 is full volume -10000 is silent
	// Returns SUCCESS if the sound was found. FAILURE otherwise
	// Parameters: sound - the SoundIndex of the sound to set
	// Notes: settings above -3000 work best
	ErrorType SetVolume(SoundIndex sound, int volume);

	// Sets the sample frequency of the specified sound. 
	// If this differs from the original frequency, the sound will become
	// high-pitched and short or deep and long
	// Returns SUCCESS if the sound was found. FAILURE otherwise
	// Parameters: sound - the PictureIndex of the sound to set
	ErrorType SetFrequency(SoundIndex sound, int frequency);

	// Sets the left-right pan of the specified sound. 
	// -10000 for left 0 for centre 10000 for right
	// Returns SUCCESS if the sound was found. FAILURE otherwise
	// Parameters: sound - the PictureIndex of the sound to set
	// Notes: Settings between -3000 and 3000 work best
	ErrorType SetPan(SoundIndex sound, int pan);

	// Plays the specified sound.
	// Returns SUCCESS if the sound was found and can play. FAILURE otherwise
	// Parameters: sound - the PictureIndex of the sound to set
	// looping - if true, this will cause the sound to loop repeatedly until told to stop
	ErrorType Play(SoundIndex sound, bool looping=false);

	// Stops playing the specified sound.
	// Returns SUCCESS if the sound was found. FAILURE otherwise
	// Parameters: sound - the PictureIndex of the sound to set
	ErrorType Stop(SoundIndex sound);

	// Stops all sounds
	void StopAllSounds();
};