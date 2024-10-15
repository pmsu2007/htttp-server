#include "main.h"
#include "log_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <syslog.h>


/* Logging and Exit */
void log_exit(const char *fmt, ...) {
	va_list ap;

	va_start(ap, fmt);
	if (debug_mode) {
		vfprintf(stderr, fmt, ap);
		fputc('\n', stderr);
	}
	else {
		vsyslog(LOG_ERR, fmt, ap);
	}
	va_end(ap);

	exit(1);
}
