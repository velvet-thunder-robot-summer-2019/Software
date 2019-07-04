#include<Arduino.h>

#include "Debugging/LogLevels.h"

int log_level = DEBUG_LVL;

void setLogLevel(int level)
{
    if (log_level <= DEBUG_LVL && log_level >= WARNING_LVL) {
        log_level = level;
    }
}

void log_debug(char *s)
{
    if (log_level <= DEBUG_LVL) {
        Serial.println(s);
    }
}

void log_info(char *s) 
{
    if (log_level <= DEBUG_LVL) {
        Serial.println(s);
    }
}

void log_warning(char *s)
{
    if (log_level <= DEBUG_LVL) {
        Serial.println(s);
    }
}