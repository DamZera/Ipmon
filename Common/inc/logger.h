#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#define LOG_LEVEL 1

#define NO_OP_FUNC \
    do {} while(0)

#if LOG_LEVEL <= 0
#define LOG_DBG(...) \
    _log(stdout, "D", __FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)
#else
    #define LOG_DBG(...) NO_OP_FUNC
#endif

#define LOG_INFO(...) \
    _log(stdout, "I", __FILE__, __LINE__,  __FUNCTION__, __VA_ARGS__)

#define LOG_WARN(...) \
    _log(stdout, "W", __FILE__, __LINE__,  __FUNCTION__, __VA_ARGS__)

#define LOG_ERR(...) \
    _log(stdout, "E", __FILE__, __LINE__,  __FUNCTION__, __VA_ARGS__)

static inline void _log(FILE* fd, const char* severity, const char* file, int line, const char* function, const char *fmt, ...)
{
    char buff[20];
    struct tm *sTm;

    time_t now = time (0);
    sTm = gmtime (&now);

    strftime (buff, sizeof(buff), "%Y-%m-%d %H:%M:%S", sTm);

    va_list args;
    fprintf(fd,"%s:%s:%s:%d::%s ", buff, severity, file, line, function);
    va_start(args, fmt);
    vfprintf(fd, fmt, args);
    va_end(args);
    fprintf(fd,"\n");
}

#endif
