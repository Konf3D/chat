#include <exception>
#include "logger.h"
Logger::Logger(const std::string& file_path) : file_path(file_path) {
    if (!openFile())
        openFail();
}
Logger::~Logger() {
    if (file.is_open()) {
        file.close();
    }
}
void Logger::writeLog(const std::string& message) {
    std::lock_guard<std::mutex> lock(mutex);
    if (file.is_open() || openFile()) {
        file << message << '\n';
        logs.push(message);  // Store the log entry in memory
        return;
    }
    return openFail();
}
std::string Logger::readLog() {
    std::lock_guard<std::mutex> lock(mutex);
    if (!logs.empty()) {
        std::string logEntry = logs.front();  // Retrieve the next log entry from memory
        logs.pop();  // Remove the retrieved log entry from memory
        return logEntry;
    }
    return "";
}
std::string Logger::readSpecificLine(unsigned int line_number) {
    if (!line_number)
        return std::string();
    file.seekg(0);
    std::lock_guard<std::mutex> lock(mutex);
    std::string line;
    unsigned int current_line = 0;

    if (!file.is_open() && !openFile()) {
        openFail();
    }

    while (current_line < line_number && std::getline(file, line)) {
        current_line++;
    }

    return line;
}

bool Logger::openFile()
{
    if (!file.is_open())
        file.open(file_path, std::ios::app);
    return file.is_open();
}

void Logger::openFail()
{
    throw std::ios_base::failure("Failed to open file: " + file_path);
}