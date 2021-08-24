#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdarg.h>

#define NO_OP_FUNC \
    do {} while(0)

#define LOG_DBG(...) \
    _log(stdout, "D", __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)

#define LOG_INFO(...) \
    _log(stdout, "I", __FILE__, __LINE__,  __FUNCTION__, __VA_ARGS__)

#define LOG_WARN(...) \
    _log(stdout, "W", __FILE__, __LINE__,  __FUNCTION__, __VA_ARGS__)

#define LOG_ERR(...) \
    _log(stdout, "E", __FILE__, __LINE__,  __FUNCTION__, __VA_ARGS__)


static inline void _log(FILE* fd, const char* severity, const char* file, int line, const char* function, const char *fmt, ...)
{
    va_list args;
    fprintf(fd,"%s:%s:%d::%s ", severity, file, line, function);
    va_start(args, fmt);
    vfprintf(fd, fmt, args);
    va_end(args);
    fprintf(fd,"\n");
}

#endif
