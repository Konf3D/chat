#pragma once
#include <fstream>
#include <string>
#include <mutex>
#include <queue>

class Logger {
public:
    Logger(const std::string& file_path);
    ~Logger();
    void writeLog(const std::string& message);
    std::string readLog();
    std::string readSpecificLine(unsigned int line_number);
private:
    //try to open file in case of failure
    bool openFile();
    //throw exception if file cannot be opened or reopened
    void openFail();
private:
    std::string file_path;
    std::fstream file;
    std::mutex mutex;
    std::queue<std::string> logs;
};