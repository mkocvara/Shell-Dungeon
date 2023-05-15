#include "StringUtil.h"


// Trim functions from https://stackoverflow.com/a/217605
 
// trim from start (in place)
inline void StringUtil::ltrim(std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
        }));
}

// trim from end (in place)
inline void StringUtil::rtrim(std::string& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
        }).base(), s.end());
}

// trim from both ends (in place)
inline void StringUtil::trim(std::string& s) {
    rtrim(s);
    ltrim(s);
}

// trim from start (copying)
inline std::string StringUtil::ltrim_copy(std::string s) {
    ltrim(s);
    return s;
}

// trim from end (copying)
inline std::string StringUtil::rtrim_copy(std::string s) {
    rtrim(s);
    return s;
}

// trim from both ends (copying)
inline std::string StringUtil::trim_copy(std::string s) {
    trim(s);
    return s;
}