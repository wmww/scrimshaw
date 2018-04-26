#include "logger.h"

#include <iostream>
#include <mutex>

std::string log_level_to_string(LogLevel level)
{
	switch (level)
	{
	case LOG_MESSAGE: return "MESSAGE";
	case LOG_WARNING: return "WARNING";
    case LOG_FATAL: return "LOG_FATAL";
	default: return "UNKNOWN LOG LEVEL";
	}
}

std::mutex log_mutex;

void log_internal(LogLevel level, std::string file_path, std::string func_name, int line_num, std::string message)
{
	std::string const toplevel = "/";
	auto pos = file_path.find_last_of(toplevel);
	if (pos != std::string::npos)
		file_path = file_path.substr(pos + toplevel.size());

	std::lock_guard<std::mutex> lock(log_mutex);

	auto stream = &std::cerr;
	if (level == LOG_MESSAGE)
		stream = &std::cout;

	if (level != LOG_MESSAGE)
		*stream << log_level_to_string(level) << ": ";

	*stream << message << " [ " << file_path << ":" << line_num << " - " << func_name << "()"
			<< " ] " << std::endl;
    if (level == LOG_FATAL)
        exit(1);
}
