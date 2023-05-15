#pragma once
#include <string>

class StringUtil
{
public:
	// trim from start (in place)
    static inline void ltrim(std::string& s);

// trim from end (in place)
    static inline void rtrim(std::string& s);

// trim from both ends (in place)
    static inline void trim(std::string& s);

// trim from start (copying)
    static inline std::string ltrim_copy(std::string s);

// trim from end (copying)
    static inline std::string rtrim_copy(std::string s);

// trim from both ends (copying)
    static inline std::string trim_copy(std::string s);
};

