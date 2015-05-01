#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>
#include <stdarg.h>
#include <syslog.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "logger.h"

int mempage_fd = -1;
size_t mempage_len = 0;
unsigned char *mempage_addr = MAP_FAILED;

int log_to_stdout = 0;
int log_to_stderr = 0;

unsigned logger_max_items(void)
{
	return sysconf(_SC_PAGESIZE) * CHAR_BIT;
}

int logger_init(const char *ctl_path)
{
	// clean up if this is being re-called
	if(mempage_addr != MAP_FAILED) {
		munmap(mempage_addr, mempage_len);
		mempage_addr = MAP_FAILED;
		mempage_len = 0;
	}
	if(mempage_fd >= 0) {
		close(mempage_fd);
		mempage_fd = -1;
	}


	mempage_len = sysconf(_SC_PAGESIZE);

	if(ctl_path == NULL) {
		mempage_addr =
			mmap(NULL, mempage_len,
			     PROT_READ | PROT_WRITE,
			     MAP_PRIVATE | MAP_ANONYMOUS,
			     -1, 0);
		if(mempage_addr == MAP_FAILED) {
			perror("mmap(mempage_fd)");
			fprintf(stderr, "fd=%d len=%zu\n", mempage_fd, mempage_len);
			goto error;
		}
		return 0;
	}

	mempage_fd = open(ctl_path, O_RDWR | O_CREAT, 0644);
	if(mempage_fd < 0) {
		perror("open(ctl_path)");
		fprintf(stderr, "ctl_path=%s\n", ctl_path);
		goto error;
	}

	int status = ftruncate(mempage_fd, mempage_len);
	if(status < 0) {
		perror("ftruncate(mempage_fd)");
		fprintf(stderr, "ctl_path=%s fd=%d\n", ctl_path, mempage_fd);
		goto error;
	}

	mempage_addr =
		mmap(NULL, mempage_len, PROT_READ | PROT_WRITE, MAP_SHARED, mempage_fd, 0);
	if(mempage_addr == MAP_FAILED) {
		perror("mmap(mempage_fd)");
		fprintf(stderr, "fd=%d len=%zu\n", mempage_fd, mempage_len);
		goto error;
	}

	return 0;

error:
	if(mempage_fd >= 0)
		close(mempage_fd);
	mempage_fd = -1;

	if(mempage_addr != MAP_FAILED)
		munmap(mempage_addr, mempage_len);
	mempage_addr = MAP_FAILED;
	mempage_len = 0;

	return -1;
}

void log_enable(unsigned item)
{
	unsigned bit = item & (CHAR_BIT-1);
	unsigned byte = item % CHAR_BIT;
	unsigned mask = 0x1 << bit;
	mempage_addr[byte] |= mask;
}

void log_disable(unsigned item)
{
	unsigned bit = item & (CHAR_BIT-1);
	unsigned byte = item % CHAR_BIT;
	unsigned char mask = 0x1 << bit;
	mempage_addr[byte] &= ~mask;
}

void log_disable_all(void)
{
	memset(mempage_addr, 0, mempage_len);
}

int log_enabled(unsigned item)
{
	unsigned bit = item & (CHAR_BIT-1);
	unsigned byte = item % CHAR_BIT;
	unsigned mask = 0x1 << bit;
	return (mempage_addr[byte] & mask) != 0;
}

int logger_output_ctl(int flags)
{
	if(flags & LOGGER_TO_STDOUT)
		log_to_stdout = 1;
	else
		log_to_stdout = 0;
		
	if(flags & LOGGER_TO_STDERR)
		log_to_stderr = 1;
	else
		log_to_stderr = 0;

	return 0;
}

static void __log_msg_va(const char *fmt, va_list ap)
{
	if(log_to_stdout)
		vfprintf(stdout, fmt, ap);

	if(log_to_stderr)
		vfprintf(stderr, fmt, ap);
}

void log_msg(unsigned item, const char *fmt, ...)
{
	if(!log_enabled(item))
		return;

	va_list ap;
	va_start(ap, fmt);
	__log_msg_va(fmt, ap);
	va_end(ap);
}

void log_msg_anyof_va(unsigned *items, const char *fmt, va_list ap)
{
	for(size_t i = 0; items[i] < logger_max_items(); i++) {
		if(log_enabled(items[i])) {
			__log_msg_va(fmt, ap);
			break;
		}
	}
}

void log_msg_anyof(unsigned *items, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	log_msg_anyof_va(items, fmt, ap);
	va_end(ap);
}

