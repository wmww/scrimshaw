#include "logger.h"

#include <iostream>

std::string log_level_to_string(LogLevel level)
{
	switch (level)
	{
	case LOG_MESSAGE: return "MESSAGE";
	case LOG_WARNING: return "WARNING";
	case LOG_ERROR: return "LOG_ERROR";
	default: return "UNKNOWN LOG LEVEL";
	}
}

void log_internal(LogLevel level, std::string file_path, std::string func_name, int line_num, std::string message)
{
	(void)func_name;

	auto stream = &std::cerr;
	if (level == LOG_MESSAGE) stream = &std::cout;

	if (level != LOG_MESSAGE) *stream << log_level_to_string(level) << " ";

	*stream << "[" << file_path << ":" << line_num << "]: " << message << std::endl;
}
