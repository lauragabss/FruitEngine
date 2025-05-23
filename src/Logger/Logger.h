#pragma once
#include <string>
#include <vector>


enum LogType
{
	LOG_INFO,
	LOG_WARNING,
	LOG_ERROR
};

struct LogEntry
{
	LogType type;
	std::string message;
};

class Logger 
{
public:
	static std::vector<LogEntry> Messages;
	static void Log(const std::string& message);
	static void Err(const std::string& message);
};
