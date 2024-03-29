#include <stdarg.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

#include "logger.hpp"
#include "utils.hpp"
#include "const.hpp"

#define COLOR_GRAY		"\e[1;30m"
#define COLOR_RED		"\e[0;31m"
#define COLOR_GREEN		"\e[0;32m"
#define COLOR_YELLOW	"\e[0;33m"
#define COLOR_RESET		"\e[0m"

static FILE *log_file;
static FILE *log_file_error;

typedef struct s_cfg {
	int quiet;
	int d;
} t_cfg;

t_cfg g_cfg = {0, 0};

static char *get_time()
{
	struct timeval t;
	struct tm *timeinfo;
	static char buf[80];

	xassert(gettimeofday(&t, NULL) != -1, "gettimeofday");
	timeinfo = localtime(&t.tv_sec);
	strftime(buf, 80, "%d.%m.%Y %H:%M:%S", timeinfo);
#ifdef __MACH__
	sprintf(strchr(buf, '\0'), ".%06d", t.tv_usec);
#else
	sprintf(strchr(buf, '\0'), ".%06ld", t.tv_usec);
#endif
	return buf;
}

static int g_tick;

static void log_print(FILE *f, const char *format, va_list ap, const char *prefix)
{
	if (g_cfg.quiet)
		return ;
	if (g_tick) {
		fprintf(f, "\n");
		g_tick = 0;
	}
	fprintf(f, "[%s] ", get_time());
	fprintf(f, "[%s] ", prefix);
	vfprintf(f, format, ap);
	fprintf(f, "\n");
}

void log_init(void)
{
	if (g_cfg.d) {
		log_file = fopen("/tmp/irc_server.log", "w");
		xassert(log_file != NULL, "fopen(\"irc_server.log\", \"w\")");
		log_file_error = log_file;
	} else {
		log_file = stdout;
		log_file_error = stderr;
	}
}

void log_tick(struct timeval *select_timeout)
{
	static int tick = 1;
	g_tick = 1;

	fprintf(log_file_error, "[%s] tick %d"
#ifdef __MACH__
			" (select timeout %zu.%06d)%s",
#else
			" (select timeout %zu.%06ld)%s",
#endif
			get_time(), tick, select_timeout->tv_sec, select_timeout->tv_usec, 
			g_cfg.d ? "\n" : "\r");
	++tick;
}

void log_debug(const char *format, ...)
{
#ifndef DEBUG
	(void)format;
	return ;
#endif
	va_list ap;

	va_start(ap, format);
	log_print(log_file, format, ap, "DEBUG");
	va_end(ap);
}

void log_info(const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	log_print(log_file, format, ap, COLOR_GREEN "INFO" COLOR_RESET);
	va_end(ap);
}

void log_warning(const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	log_print(log_file, format, ap, COLOR_YELLOW "WARNING" COLOR_RESET);
	va_end(ap);
}

void log_error(const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	log_print(log_file_error, format, ap, COLOR_RED "ERROR" COLOR_RESET);
	va_end(ap);
}

void log_fatal(const char *format, ...)
{
	va_list ap;

	va_start(ap, format);
	log_print(log_file_error, format, ap, COLOR_RED "FATAL" COLOR_RESET);
	va_end(ap);
	exit(1);
}
