#include "Logger.h"
#include <iostream>
#include <ctime>

std::vector<LogEntry> Logger::Messages;

std::string DateTime(time_t time, const char* format)
{
	char buffer[90];
	struct tm timeInfo;
	localtime_s(&timeInfo, &time);
	strftime(buffer, sizeof(buffer), format, &timeInfo);
	return buffer;
}

void Logger::Log(const std::string& message)
{
	LogEntry logEntry;
	logEntry.type = LOG_INFO;

	time_t timestamp = time(NULL);
	std::string formattedTime = DateTime(timestamp, "%d/%b/%Y %H:%M:%S");
	logEntry.message = "LOG: [" + formattedTime + "]: " + message;

	printf("\033[1;32m%s\033[0m\n", logEntry.message.c_str());

	Messages.push_back(logEntry);
}

void Logger::Err(const std::string& message)
{
	LogEntry logEntry;
	logEntry.type = LOG_INFO;

	time_t timestamp = time(NULL);
	std::string formattedTime = DateTime(timestamp, "%d/%b/%Y %H:%M:%S");
	logEntry.message = "ERR: [" + formattedTime + "]: " + message;

	printf("\033[1;31m%s\033[0m\n", logEntry.message.c_str());

	Messages.push_back(logEntry);
}
