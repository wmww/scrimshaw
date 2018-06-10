#include "logger.h"

#include <iostream>
#include <mutex>

std::string log_level_to_string(LogLevel level)
{
	switch (level)
	{
	case LogLevel::message: return "MESSAGE";
	case LogLevel::warning: return "WARNING";
	case LogLevel::fatal: return "FATAL";
	default: return "UNKNOWN LOG LEVEL";
	}
}

std::string log_level_color_code(LogLevel level)
{
	switch (level)
	{
	case LogLevel::message: return "\033[31m";
	case LogLevel::warning: return "\033[1;33m";
	case LogLevel::fatal: return "\033[1;31m";
	default: return "\033[0m";
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
	if (level == LogLevel::message)
		stream = &std::cout;

	if (level != LogLevel::message)
		*stream << log_level_color_code(level) << log_level_to_string(level) << "\033[0m"
				<< " ";

	*stream << "\033[0;36m"
			<< "[" << file_path << ":" << line_num;
	*stream << (line_num < 1000 ? (line_num < 100 ? (line_num < 10 ? "   " : "  ") : " ") : "");
	*stream << func_name << "]: "
			<< "\033[0m";

	*stream << message;

	*stream << std::endl;

	if (level == LogLevel::fatal)
		exit(1);
}
