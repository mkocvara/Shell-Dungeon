// mysoundengine.cpp	Version 10		9/5/05
// The definition file for the methods in MySoundEngine, declared in mysoundengine.h
#define DSBCAPS_CTRLDEFAULT 0x000000E0

#include "mysoundengine.h"
#include "errorlogger.h"

MySoundEngine::MySoundEngine(HWND hwnd)
{
	// The first sound loaded will have a SoundIndex value of 1
	m_NextSoundIndex = 1;

	emptySound.m_sourceFileName = L"empty sound";

	// Initialise dsound
	HRESULT err;
	if (FAILED(DirectSoundCreate8(&DSDEVID_DefaultPlayback, &lpds, NULL)))
	{
		ErrorLogger::Writeln(L"Failed to create sound player");
		lpds=nullptr;
	}

	// Set cooperative level and check for error
	err=lpds->SetCooperativeLevel(hwnd, DSSCL_NORMAL);

	if (FAILED(err))	// If failed to set cooperative level
	{
		ErrorLogger::Writeln(L"Failed to set cooperative level\n");
		lpds->Release();
		lpds=nullptr;
		ErrorLogger::Writeln(ErrorString(err));
	}
}

MySoundEngine::~MySoundEngine()
{
	// Unload and release sound buffers
	UnloadAllSounds();
	// Release DirectSound
	Release();
}

ErrorType MySoundEngine::Release()
{
	if (lpds)			// If not already null
	{
		lpds->Release();
		lpds=nullptr;
		return SUCCESS;
	}
	return FAILURE;
}

const wchar_t* MySoundEngine::ErrorString(HRESULT err)
{
	// Returns an error string from DirectX
	return ERRORSTRING(err);
}

MySoundEngine::MySound& MySoundEngine::FindSound(SoundIndex sound)
{
	// Find a sound in the map
	std::map<SoundIndex, MySound>::iterator it = m_MySoundList.find(sound);
	if(it  == m_MySoundList.end())			// Not found
	{
		ErrorLogger::Writeln(L"SoundIndex not found");
		// Return empty sound buffer
		return emptySound;
	}
	return it->second;
}

SoundIndex MySoundEngine::LoadWav(const wchar_t* filename)
{
	// Check if already loaded
	auto it = m_FilenameList.find(filename);
	if (it != m_FilenameList.end())
	{
		return it->second;
	}

	if(!lpds)
	{
		ErrorLogger::Writeln(L"Cannot load a sound wave - No pointer to DirectSound.");
		return -1;
	}

	MySound temp;
	temp.LoadWav(filename, lpds);

	m_MySoundList.insert(std::pair<SoundIndex, MySound>(m_NextSoundIndex, temp));

	m_FilenameList.insert(std::pair<std::wstring, SoundIndex>(filename, m_NextSoundIndex));

	return m_NextSoundIndex++;
}

ErrorType MySoundEngine::Unload(SoundIndex sound)
{
	std::map<SoundIndex, MySound>::iterator it = m_MySoundList.find(sound);
	if (it != m_MySoundList.end())
	{
		MySound& sb = it->second;

		sb.Release();

		m_MySoundList.erase(it);
		m_FilenameList.erase(sb.m_sourceFileName);

		return SUCCESS;

	}

	return FAILURE;
}

ErrorType MySoundEngine::UnloadAllSounds()
{
	ErrorType answer = SUCCESS;
	std::map<SoundIndex, MySound>::iterator it = m_MySoundList.begin();
	for(;it!= m_MySoundList.end();it++ )
	{
		MySound& sb = it->second;

		sb.Release();
	}

	m_MySoundList.erase(it, m_MySoundList.end());
	m_FilenameList.clear();
	return answer;
}

ErrorType MySoundEngine::SetVolume(SoundIndex sound, int volume)
{
	MySound& sb = FindSound(sound);
	if(!sb.m_isLoaded)
	{
		ErrorLogger::Writeln(L"Sound not found in SetVolume.");
		return FAILURE;
	}
	if (sb.SetVolume(volume) == FAILURE)
	{
		ErrorLogger::Write(L"Failed to set volume for a sound:");
		ErrorLogger::Writeln(sb.m_sourceFileName.c_str());
		return FAILURE;
	}
	return SUCCESS;
}

ErrorType MySoundEngine::SetFrequency(SoundIndex sound, int frequency)
{
	MySound& sb = FindSound(sound);

	if(!sb.m_isLoaded)
	{
		ErrorLogger::Writeln(L"Sound not found in SetFrequency.");
		return FAILURE;
	}
	
	if (sb.SetFrequency(frequency) == FAILURE)
	{
		ErrorLogger::Write(L"Failed to set frequency for a sound: ");
		ErrorLogger::Writeln(sb.m_sourceFileName.c_str());
		return FAILURE;
	}
	return SUCCESS;
}

ErrorType MySoundEngine::SetPan(SoundIndex sound, int pan)
{
	MySound& sb = FindSound(sound);
	if (!sb.m_isLoaded)
	{
		ErrorLogger::Writeln(L"Sound not found in SetPan.");
		return FAILURE;
	}
	if (sb.SetPan(pan) == FAILURE)
	{
		ErrorLogger::Write(L"Failed to set pan for a sound: ");
		ErrorLogger::Writeln(sb.m_sourceFileName.c_str());
		return FAILURE;
	}
	return SUCCESS;
}

ErrorType MySoundEngine::Play(SoundIndex sound, bool looping)
{
	// The first two numbers in the Play() functions below
	// are always zero. The third controls whether to loop,
	// or just play once.

	MySound& sb = FindSound(sound);

	if(!sb.m_isLoaded)
	{
		ErrorLogger::Writeln(L"Can't play sound - Sound buffer not created.");
	}
	else
	{
		ErrorType result = sb.Play(looping, lpds);

		return result;
	}	// if lpSoundBuffer not NULL
	return FAILURE;	
}

ErrorType MySoundEngine::Stop(SoundIndex sound)
{
	MySound& sb = FindSound(sound);
	if(!sb.m_isLoaded)
	{
		ErrorLogger::Writeln(L"Sound buffer not created.");
		return FAILURE;
	}

	return sb.Stop();
}

void MySoundEngine::StopAllSounds()
{
	ErrorType answer = SUCCESS;
	std::map<SoundIndex, MySound>::iterator it = m_MySoundList.begin();
	for (; it != m_MySoundList.end(); it++)
	{
		MySound& sb = it->second;

		sb.Stop();
	}
}




MySoundEngine::MySound::MySound()
{
	for (LPDIRECTSOUNDBUFFER& pNext : lpSoundBuffers)
	{
		pNext = nullptr;
	}
}

void MySoundEngine::MySound::Release()
{
	for (LPDIRECTSOUNDBUFFER& pNext : lpSoundBuffers)
	{
		if (pNext)
		{
			pNext->Release();
			pNext = nullptr;
		}

	}
}

MySoundEngine::MySound::~MySound()
{
	
}

LPDIRECTSOUNDBUFFER MySoundEngine::MySound::GetBuffer(IDirectSound8* lpds)
{
	if (lpSoundBuffers.empty())
	{
		ErrorLogger::Writeln(L"Failed to GetBuffer, no buffers loaded.");
		return nullptr;
	}

	// find a buffer that isn't currently playing
	for (LPDIRECTSOUNDBUFFER buffer : lpSoundBuffers)
	{
		DWORD dwStatus;
		buffer->GetStatus(&dwStatus);
		bool isPlaying = (dwStatus & DSBSTATUS_PLAYING);
		if (!isPlaying)
		{
			return buffer;
		}
	}
	
	// if all are playing, make a duplicate and return it
	LPDIRECTSOUNDBUFFER tempBuffer;
	HRESULT err = lpds->DuplicateSoundBuffer(lpSoundBuffers[0], &tempBuffer);
	if (err == S_OK)
	{
		lpSoundBuffers.push_back(std::move(tempBuffer));
	}
	else
	{
		ErrorLogger::Write(L"Couldn't create buffer duplicates: ");
		ErrorLogger::Writeln(m_sourceFileName.c_str());
	}

	return lpSoundBuffers[lpSoundBuffers.size()-1];
}

ErrorType MySoundEngine::MySound::Stop()
{
	ErrorType result = SUCCESS;
	for (LPDIRECTSOUNDBUFFER& pNext : lpSoundBuffers)
	{
		if (FAILED(pNext->Stop()))
			result = FAILURE;
	}
	return result;
}

ErrorType MySoundEngine::MySound::SetVolume(int vol)
{
	ErrorType result = SUCCESS;
	for (LPDIRECTSOUNDBUFFER& pNext : lpSoundBuffers)
	{
		if (FAILED(pNext->SetVolume(vol)))
			result = FAILURE;
	}
	return result;
}

ErrorType MySoundEngine::MySound::Play(bool looping, IDirectSound8* lpds)
{
	DWORD flag = 0;
	if (looping)
	{
		flag = DSBPLAY_LOOPING;
	}
	HRESULT err = GetBuffer(lpds)->Play(0, 0, flag);
	if (FAILED(err))
	{
		ErrorLogger::Write(L"Failed to play a sound: ");
		ErrorLogger::Writeln(m_sourceFileName.c_str());
		ErrorLogger::Writeln(MySoundEngine::ErrorString(err));
		return FAILURE;
	}
	return SUCCESS;
}

ErrorType MySoundEngine::MySound::SetPan(int pan)
{
	ErrorType result = SUCCESS;
	for (LPDIRECTSOUNDBUFFER& pNext : lpSoundBuffers)
	{
		HRESULT err = pNext->SetPan(pan);
		if (FAILED(err)) result = FAILURE;
	}

	if (result == FAILURE)
	{
		ErrorLogger::Write(L"Failed to pan a sound:");
		ErrorLogger::Writeln(m_sourceFileName.c_str());
		return FAILURE;
	}
	return SUCCESS;
}

ErrorType MySoundEngine::MySound::SetFrequency(int frequency)
{
	ErrorType result = SUCCESS;
	for (LPDIRECTSOUNDBUFFER& pNext : lpSoundBuffers)
	{
		HRESULT err = pNext->SetFrequency(frequency);
		if (FAILED(err)) result = FAILURE;
	}

	if (result == FAILURE)
	{
		ErrorLogger::Write(L"Failed to set a sound frequency: ");
		ErrorLogger::Writeln(m_sourceFileName.c_str());
		return FAILURE;
	}
	return SUCCESS;
}


ErrorType MySoundEngine::MySound::LoadWav(const wchar_t* filename, IDirectSound8* lpds)
{
	m_sourceFileName = filename;

	DSBUFFERDESC dsbd;			// "Order form" for the sound
	WAVEFORMATEX formatdesc;	// Description of the format	
	HMMIO hWaveFile;			// Handle to the wave file
	MMCKINFO parent;			// A parent chunk (wav file data chunks)
	MMCKINFO child;				// A child chunk (wav file data chunks)

	UCHAR* tempBuffer;			// Pointer to a buffer to temporarily store sound
	UCHAR* tempPtr1;			// Pointer to first part of sound buffer
	UCHAR* tempPtr2;			// Pointer to second part of sound buffer
	DWORD length1;				// Length of first part of sound buffer
	DWORD length2;				// Length of second part of sound buffer

	// ***************************************************************
	// Most of what follows is some fairly complicated bits that
	// open a pcm wave file, and read the contents into the
	// directX buffer.

	// Chunk info initialised

	parent.ckid = (FOURCC)0;
	parent.cksize = 0;
	parent.fccType = (FOURCC)0;
	parent.dwDataOffset = 0;
	parent.dwFlags = 0;

	child = parent;

	// Open the wav file
	wchar_t buffer[100];
	wcscpy_s(buffer, filename);

	hWaveFile = mmioOpen(buffer, NULL, MMIO_READ | MMIO_ALLOCBUF);

	// If file could not open
	if (!hWaveFile)			
	{
		ErrorLogger::Write(L"Failed to open sound file ");
		ErrorLogger::Writeln(filename);
		return FAILURE;	
	}

	// Find the wave section
	// What is it with sound engineers? Is is because they are musicians
	// that they like using meaningless terms like "descend"?
	parent.fccType = mmioFOURCC('W', 'A', 'V', 'E');

	if (mmioDescend(hWaveFile, &parent, NULL, MMIO_FINDRIFF))
	{
		ErrorLogger::Write(L"Couldn't find wave section in wave file ");
		ErrorLogger::Writeln(filename);

		mmioClose(hWaveFile, 0); // Error - close the wave file
		return FAILURE;			
	}

	// Find the format section
	child.ckid = mmioFOURCC('f', 'm', 't', ' ');
	if (mmioDescend(hWaveFile, &child, &parent, 0) != MMSYSERR_NOERROR)
	{
		ErrorLogger::Write(L"Couldn't find format section in wave file ");
		ErrorLogger::Writeln(filename);

		mmioClose(hWaveFile, 0); // Error - close the wave file
		return FAILURE;		
	}

	// Read out the format data
	if (mmioRead(hWaveFile, (char*)&formatdesc, sizeof(formatdesc)) != sizeof(formatdesc))
	{
		ErrorLogger::Write(L"Error in wave format of ");
		ErrorLogger::Writeln(filename);

		mmioClose(hWaveFile, 0);
		return FAILURE;		
	}

	// Check this is a pcm format (a standard wav format)
	if (formatdesc.wFormatTag != WAVE_FORMAT_PCM)
	{
		ErrorLogger::Write(L"Error in wave format of ");
		ErrorLogger::Writeln(filename);

		mmioClose(hWaveFile, 0);
		return FAILURE;		
	}

	// Pop upstairs so we can then get down to data chunk
	if (mmioAscend(hWaveFile, &child, 0) != MMSYSERR_NOERROR)
	{
		ErrorLogger::Write(L"Couldn't ascend to data chunk of ");
		ErrorLogger::Writeln(filename);

		mmioClose(hWaveFile, 0);
		return FAILURE;		
	}

	// Now drop into data chunk
	child.ckid = mmioFOURCC('d', 'a', 't', 'a');

	if (mmioDescend(hWaveFile, &child, &parent, MMIO_FINDCHUNK) != MMSYSERR_NOERROR)
	{
		ErrorLogger::Write(L"Couldn't find data section in wave file ");
		ErrorLogger::Writeln(filename);

		mmioClose(hWaveFile, 0); // Error - close the wave file
		return FAILURE;			
	}


	// *************************************************************
	// Now that the info from the file has been stored, it is possible to
	// Create a sound buffer ready to hold the data, so it back to directX

	// Set the descriptor for the sound to be created.

	memset(&dsbd, 0, sizeof(dsbd));
	dsbd.dwSize = sizeof(dsbd);
	dsbd.dwFlags = DSBCAPS_CTRLDEFAULT;				// Default features
	dsbd.dwBufferBytes = child.cksize;				// Set bytes needed to store
	dsbd.lpwfxFormat = &formatdesc;					// The format descriptor (got earlier from the file)

	LPDIRECTSOUNDBUFFER tempLpSoundBuffer;
	HRESULT err = lpds->CreateSoundBuffer(&dsbd, &tempLpSoundBuffer, NULL);
	if (FAILED(err))
	{
		Release();
		ErrorLogger::Writeln(L"Could not create a sound buffer");
		ErrorLogger::Writeln(MySoundEngine::ErrorString(err));
		return FAILURE;
	}
	
	// ************************************************************
	// The file is open, the buffer is created. Now to read all the data in.

	// Load data into a buffer
	tempBuffer = (UCHAR*)malloc(child.cksize);
	mmioRead(hWaveFile, (char*)tempBuffer, child.cksize);

	// Close the file
	mmioClose(hWaveFile, 0);

	// Locking the Dsound buffer

	err = tempLpSoundBuffer->Lock(0, child.cksize, (void**)&tempPtr1,
		&length1, (void**)&tempPtr2,
		&length2, DSBLOCK_FROMWRITECURSOR);
	if (FAILED(err))
	{
		ErrorLogger::Writeln(L"Couldn't lock the sound buffer.");
		ErrorLogger::Writeln(MySoundEngine::ErrorString(err));
		free(tempBuffer);
		tempLpSoundBuffer->Release();
		tempLpSoundBuffer = nullptr;
		return FAILURE;
	}

	// Copy the two bits of the buffer
	memcpy(tempPtr1, tempBuffer, length1);
	memcpy(tempPtr2, tempBuffer + length1, length2);

	// Unlock the Dsound buffer
	err = tempLpSoundBuffer->Unlock(tempPtr1,
		length1, tempPtr2,
		length2);
	if (FAILED(err))
	{
		ErrorLogger::Writeln(L"Couldn't unlock the sound buffer.");
		ErrorLogger::Writeln(MySoundEngine::ErrorString(err));
		free(tempBuffer);
		tempLpSoundBuffer->Release();
		tempLpSoundBuffer = nullptr;
		return FAILURE;	
	}

	free(tempBuffer);

	lpSoundBuffers.push_back(std::move(tempLpSoundBuffer));
	m_isLoaded = true;

	return SUCCESS;
}