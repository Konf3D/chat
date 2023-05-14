#include "logentry.h"

LogEntry::LogEntry(const std::string& message, SeverityLevel severityLevel)
    : message(message), severityLevel(severityLevel) {
    timestamp = time_stamp();
}

void LogEntry::setLogger(const std::string& logger) {
    this->logger = logger;
}

void LogEntry::setThreadID(int threadID){
    this->threadID = threadID;
}

void LogEntry::setAdditionalMetadata(const std::string& key, const std::string& value){
    additionalMetadata[key] = value;
}

LogEntry::operator std::string() const {
    std::string logString;
    logString += "[" + timestamp + "]";
    logString += " [" + severityLevelToString(severityLevel) + "]";
    if (!logger.empty()) {
        logString += " [" + logger + "]";
    }
    if (threadID >= 0) {
        logString += " [Thread ID: " + std::to_string(threadID) + "]";
    }
    for (const auto& data : additionalMetadata) {
        logString += " [" + data.first + ": " + data.second + "]";
    }
    logString += " " + message;
    return logString;
}

std::string LogEntry::severityLevelToString(SeverityLevel level) const
{
    switch (level) {
    case SeverityLevel::debug:
        return "DEBUG";
    case SeverityLevel::info:
        return "INFO";
    case SeverityLevel::warning:
        return "WARNING";
    case SeverityLevel::error:
        return "ERROR";
    case SeverityLevel::critical:
        return "CRITICAL";
    default:
        return "UNKNOWN";
    }
}