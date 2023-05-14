#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>
#include "timestamp.h"


// Returns the local time as a std::tm struct.
std::tm get_local_time(std::time_t timer)
{
    //remake//https://stackoverflow.com/a/38034148
    std::tm bt{};
#if defined(__unix__)
    localtime_r(&timer, &bt);
#elif defined(_WIN32)
    localtime_s(&bt, &timer);
#else
    static std::mutex mtx;
    std::scoped_lock lock(mtx);
    bt = *std::localtime(&timer);
#endif
    return bt;
}

// Returns the current time stamp as a string in the specified format.
std::string time_stamp(const std::string& fmt)
{
    //remake//https://stackoverflow.com/a/38034148
    auto bt = get_local_time(std::time(nullptr));
    std::ostringstream oss;
    oss << std::put_time(&bt, fmt.c_str());
    return oss.str();
}