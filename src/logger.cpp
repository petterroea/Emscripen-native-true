#include <iostream>
#include "logger.h"


void logMessage(LogSeverity severity, std::string message)
{
    std::cout << "[" << severityToString(severity) << "] " << message << std::endl;
}

std::string severityToString(LogSeverity severity)
{
    switch(severity) {
    case INFO:
        return "INFO";
    case WARNING:
        return "WARNING";
    case ERROR:
        return "ERROR";
    default:
        return "unknown";
    }
}
