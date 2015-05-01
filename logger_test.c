#include <stdio.h>
#include <unistd.h>

// logging infrastructure
#include "logger.h"

// application-specific logging items
#include "log_items.h"

int main(void)
{
	if(getenv("LOGGERCTLFILE") == NULL) {
		fprintf(stderr, "error: LOGGERCTLFILE unset\n");
		return -1;
	}

	if(logger_init(getenv("LOGGERCTLFILE")))
		return -1;

	logger_output_ctl(LOGGER_TO_STDERR);

	int i = 0;
	while(++i < 101) {
		if(i % 2 == 0) 
			log_msg(LOGGER_FIZZ, "fizz %d\n", i);

		if(i % 3 == 0)
			log_msg(LOGGER_BUZZ, "buzz %d\n", i);

		sleep(1);
	}

	return 0;
}
