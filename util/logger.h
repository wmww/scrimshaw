#pragma once

#include <string>

enum LogLevel { LOG_MESSAGE, LOG_WARNING, LOG_FATAL };

void log_internal(LogLevel level, std::string file_path, std::string func_name, int line_num, std::string message);

#define log_message(msg) log_internal(LOG_MESSAGE, __FILE__, __func__, __LINE__, msg);
#define log_warning(msg) log_internal(LOG_WARNING, __FILE__, __func__, __LINE__, msg);
#define log_fatal(msg) log_internal(LOG_FATAL, __FILE__, __func__, __LINE__, msg);
#define assert_else(condition, action) if (!(condition)) { log_warning("failed assertion '" #condition "'"); action; }
#define assert_nonfatal(condition) assert_else(condition, )
#define assert_fatal(condition) if (!(condition)) { log_fatal("failed assertion '" #condition "'"); }
