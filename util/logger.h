#pragma once
#include "util/std.h"

enum class LogLevel { message, warning, fatal };

void log_internal(LogLevel level, std::string file_path, std::string func_name, int line_num, std::string message);

#define log_message_off(msg)
#define log_message(msg) log_internal(LogLevel::message, __FILE__, __func__, __LINE__, msg);
#define log_warning(msg) log_internal(LogLevel::warning, __FILE__, __func__, __LINE__, msg);
#define log_fatal(msg) log_internal(LogLevel::fatal, __FILE__, __func__, __LINE__, msg);
#define assertion_text(condition) "assertion \033[40;37m" #condition "\033[0m failed"
#define assert_else(condition, action)          \
	if (!(condition))                           \
	{                                           \
		log_warning(assertion_text(condition)); \
		action;                                 \
	}
#define assert_nonfatal(condition) assert_else(condition, )
#define assert_fatal(condition)               \
	if (!(condition))                         \
	{                                         \
		log_fatal(assertion_text(condition)); \
	}
