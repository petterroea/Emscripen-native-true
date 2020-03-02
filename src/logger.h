#ifndef LOGGER_H
#define LOGGER_H

#include <string>

enum LogSeverity {
    INFO,
    WARNING,
    ERROR
};

void logMessage(LogSeverity severity, std::string message);
std::string severityToString(LogSeverity severity);

#endif // LOGGER_H
