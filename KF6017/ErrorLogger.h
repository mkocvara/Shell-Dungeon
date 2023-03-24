#pragma once

#include <fstream>
#include <windows.h>

//using namespace std;

#define LOGGING			// Remove this to prevent logging
#define SLOWLOG			// Remove to prevent log flush (will log faster, but 
					         // may not write to file in the event of a program crash


// We used to be able to get helpful messages from DirectX, but this is now
// much more complicated.
// To avoid a major re-write, I am using this placeholder for now.
#define ERRORSTRING(x) L"Error not specified"


// The error logger allows the user to write error messages to a log file
// Due to changes in DirectX messaging, some features of the log system
// have been removed from earlier versions.
// See errorlogger.cpp for the file name of the log file (probably "error.log")
class ErrorLogger
{
private:
	static ErrorLogger msInstance;		// Errorlogger is a singleton
										// Programmer note - this singleton seems a bit confused.
										// It works, so leave it for now, but it needs to be tidied up.
	ErrorLogger();
	~ErrorLogger();
	std::wofstream mFile;				// The file written to
	const static wchar_t msFilename[];	// The file name
	const static int MAXLINES=10000;	// Maximum number of lines that will be written
										// - to prevent a massive file.

	static int msLineCount;				// Current number of lines that have been written.
										// Programmer note- why is this a static?

public:
	static void Writeln(const wchar_t text[]);	// Writes the given text, followed by a new line
												// Increments LineCount
												// Will not write if LineCount < MAXLINES

	static void Writeln(double num);			// Writes the given number, followed by a new line
												// Increments LineCount
												// Will not write if LineCount < MAXLINES

	static void Write(const wchar_t text[]);	// Writes the given text
												// Will not write if LineCount < MAXLINES

	static void Write(double num);				// Writes the given number
												// Will not write if LineCount < MAXLINES
};
