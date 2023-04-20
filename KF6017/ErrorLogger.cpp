// Errorlogger.cpp
// Shell engine version 2020
// Chris Rook
// Last modified 20/09/2018

#include "errorlogger.h"
#include <math.h>
#include <string>

const wchar_t ErrorLogger::msFilename[]=L"errors.log";
ErrorLogger ErrorLogger::msInstance;
int ErrorLogger::msLineCount=0;


ErrorLogger::ErrorLogger()
{
#ifdef LOGGING
	mFile.open(msFilename);

	LPSYSTEMTIME time = new SYSTEMTIME();
	GetLocalTime(time);

	std::wstring timeLog = L"Game Ran on " +
		std::wstring((time->wDay <= 9) ? L"0" : L"") + // for some reason the construction here is necessary, but not elsewhere.
		std::to_wstring(time->wDay) + L'/' +
		((time->wMonth <= 9) ? L"0" : L"") +
		std::to_wstring(time->wMonth) + L'/' +
		std::to_wstring(time->wYear) + L", " +
		((time->wHour <= 9) ? L"0" : L"") +
		std::to_wstring(time->wHour) + L":" +
		((time->wMinute <= 9) ? L"0" : L"") +
		std::to_wstring(time->wMinute);

	Writeln(timeLog.c_str());
	std::wstring separator(L"====================================");
	separator.resize(timeLog.length());
	Writeln(separator.c_str());
	delete time;
#endif
}

ErrorLogger::~ErrorLogger()
{
#ifdef LOGGING
	mFile.close();
#endif
}

// Writes the given text, followed by a new line
// Increments LineCount
// Will not write if LineCount < MAXLINES
void ErrorLogger::Writeln(const wchar_t text[])
{
	Write(text);
	Write(L"\n");
}

// Writes the given text
// Will not write if LineCount < MAXLINES
void ErrorLogger::Write(const wchar_t text[])
{
#ifdef LOGGING
	if(msLineCount<MAXLINES)
	{
		OutputDebugString(text);
		msInstance.mFile << text;
		if(++msLineCount == MAXLINES)
		{
			OutputDebugString(L"\nErrorLogger limit reached. Who taught you to progam?");
			msInstance.mFile << L"\nErrorLogger limit reached. Who taught you to progam?";
			msInstance.mFile.flush();
		}
	}
#endif
#ifdef SLOWLOG
	msInstance.mFile.flush();
#endif
}

// Writes the given number, followed by a new line
// Increments LineCount
// Will not write if LineCount < MAXLINES
void ErrorLogger::Writeln(double num)
{
	Write(num);
	Write(L"\n");
}

// Writes the given number
// Will not write if LineCount < MAXLINES
void ErrorLogger::Write(double num)
{
#ifdef LOGGING
	wchar_t buffer[32];
	swprintf_s( buffer,32, L"%.8g", num );
	Write(buffer);
#endif
}

