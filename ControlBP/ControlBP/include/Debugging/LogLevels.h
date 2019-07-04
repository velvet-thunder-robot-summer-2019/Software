#ifndef LOG_LEVELS_H_
#define LOG_LEVELS_H_

#define WARNING_LVL 0
#define INFO_LVL 1
#define DEBUG_LVL 2

void setLogLevel(int level);

void log_debug(char *s);

void log_info(char *s);

void log_warning(char *s);

#endif //LOG_LEVELS_H_