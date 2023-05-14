#pragma once
#include <ctime>

// Returns the local time as a std::tm struct.
std::tm get_local_time(std::time_t timer);
// Returns the current time stamp as a string in the specified format.
std::string time_stamp(const std::string& fmt = "%F %T");
// Timestamp as string with format YYYY-MM-DD HH-MM-SS.
class TimeStamp
{
public:
    operator std::string() const
    {
        return time_stamp();
    }
};