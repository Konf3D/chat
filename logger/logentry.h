#include <string>
#include <unordered_map>
#include "timestamp.h"

enum class SeverityLevel {
    debug,
    info,
    warning,
    error,
    critical
};

class LogEntry {
public:
    LogEntry(const std::string& message, SeverityLevel severityLevel);
    ~LogEntry() = default;

    void setLogger(const std::string& logger);
    void setThreadID(int threadID);
    void setAdditionalMetadata(const std::string& key, const std::string& value);
    operator std::string() const;

private:
    std::string severityLevelToString(SeverityLevel level) const;

    std::string timestamp;
    std::string message;
    SeverityLevel severityLevel;
    std::string logger;
    int threadID = -1;
    std::unordered_map<std::string, std::string> additionalMetadata;
};