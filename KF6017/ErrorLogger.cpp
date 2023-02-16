// Errorlogger.cpp
// Shell engine version 2020
// Chris Rook
// Last modified 20/09/2018

#include "errorlogger.h"
#include <math.h>
#include <string>

const wchar_t ErrorLogger::Filename[]=L"errors.log";
ErrorLogger ErrorLogger::instance;
int ErrorLogger::LineCount=0;


ErrorLogger::ErrorLogger()
{
#ifdef LOGGING
	file.open(Filename);

	LPSYSTEMTIME time = new SYSTEMTIME();
	GetSystemTime(time);

	std::wstring timeLog = L"Game Ran on " +
		std::to_wstring(time->wDay) + L'/' +
		std::to_wstring(time->wMonth) + L'/' +
		std::to_wstring(time->wYear) + L", " +
		std::to_wstring(time->wHour) + L":" +
		std::to_wstring(time->wMinute);
	
	Writeln(timeLog.c_str());
	std::wstring separator(L"===================================");
	separator.resize(timeLog.length());
	Writeln(separator.c_str());
	delete time;
#endif
}

ErrorLogger::~ErrorLogger()
{
#ifdef LOGGING
	file.close();
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
	if(LineCount<MAXLINES)
	{
		OutputDebugString(text);
		instance.file << text;
		if(++LineCount == MAXLINES)
		{
			OutputDebugString(L"\nErrorLogger limit reached. Who taught you to progam?");
			instance.file << L"\nErrorLogger limit reached. Who taught you to progam?";
			instance.file.flush();
		}
	}
#endif
#ifdef SLOWLOG
	instance.file.flush();
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

