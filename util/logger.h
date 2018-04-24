#pragma once

#include <string>

enum LogLevel { LOG_MESSAGE, LOG_WARNING, LOG_ERROR };

void log_internal(LogLevel level, std::string file_path, std::string func_name, int line_num, std::string message);

#define log_message(msg) log_internal(LOG_MESSAGE, __FILE__, __func__, __LINE__, msg);
#define log_warning(msg) log_internal(LOG_MESSAGE, __FILE__, __func__, __LINE__, msg);
#define log_error(msg) log_internal(LOG_MESSAGE, __FILE__, __func__, __LINE__, msg);
