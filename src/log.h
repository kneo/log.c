/**
 * Copyright (c) 2020 rxi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See `log.c` for details.
 */

#ifndef LOG_H
#define LOG_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define LOG_VERSION "0.2.0"

typedef struct
{
    va_list     ap;
    const char *fmt;
    const char *file;
    struct tm  *time;
    void       *udata;
    int         line;
    int         level;
} log_Event;

typedef void (*log_LogFn)(log_Event *ev);
typedef void (*log_LockFn)(bool lock, void *udata);
typedef struct tm *(*log_LocalTimeFn)(const time_t *_timer);

enum
{
    LOG_TRACE,
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_FATAL
};

#define log_trace(...) log_log(LOG_TRACE, __FILE__, __LINE__, __VA_ARGS__)
#define log_debug(...) log_log(LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define log_info(...)  log_log(LOG_INFO, __FILE__, __LINE__, __VA_ARGS__)
#define log_warn(...)  log_log(LOG_WARN, __FILE__, __LINE__, __VA_ARGS__)
#define log_error(...) log_log(LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define log_fatal(...) log_log(LOG_FATAL, __FILE__, __LINE__, __VA_ARGS__)

void        init_log();
const char *log_level_string(int level);
void        log_set_lock(log_LockFn fn, void *udata);
void        log_set_localtime(log_LocalTimeFn fn);
/* Gates the built-in stderr target only; each callback target carries its own
 * level, checked against log_add_callback()'s level argument instead. */
void        log_set_level(int level);
void        log_set_quiet(bool enable);
int         log_add_callback(log_LogFn fn, void *udata, int level);
/* log_Event.time is filled in through log_set_localtime()'s provider, or
 * through the C library's localtime() unless LOG_CALLBACKS_ONLY is defined. */
#ifndef LOG_CALLBACKS_ONLY
int         log_add_fp(FILE *fp, int level);
#endif

void log_log(int level, const char *file, int line, const char *fmt, ...);

#endif
