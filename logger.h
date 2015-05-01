#if ! defined(LOGGER_H_INCLUDED)
#define LOGGER_H_INCLUDED

#include <stdarg.h>

unsigned logger_max_items(void);

int logger_init(const char *ctl_path);

int log_enabled(unsigned item);
void log_enable(unsigned item);
void log_disable(unsigned item);
void log_disable_all(void);

#define LOGGER_TO_STDOUT 1
#define LOGGER_TO_STDERR 2
int logger_output_ctl(int flags);

void log_msg(unsigned item, const char *fmt, ...);
void log_msg_va(unsigned item, const char *fmt, va_list ap);

void log_msg_anyof(unsigned *items, const char *fmt, ...);
void log_msg_anyof_va(unsigned *items, const char *fmt, va_list ap);

#endif
