CFLAGS= --std=c99 -Wall -Wextra -g3 -D_POSIX_C_SOURCE=200112L -D_GNU_SOURCE

default: all
all: logger_ctl logger_test
logger_ctl: logger_ctl.c logger.c log_items.c
	gcc ${CFLAGS} -o $@ $^
logger_test: logger_test.c logger.c log_items.c
	gcc ${CFLAGS} -o $@ $^
clean:
	-rm logger_ctl logger_test
