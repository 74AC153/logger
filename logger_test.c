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
			log_msg(LOGGER_FIZZ, "fizz");

		if(i % 3 == 0)
			log_msg(LOGGER_BUZZ, "buzz");

		if((i % 2 == 0) && (i % 3 != 0))
			log_msg(LOGGER_FIZZ, "\n");
		else if((i % 2 != 0) && (i % 3 == 0))
			log_msg(LOGGER_BUZZ, "\n");
		else if((i % 2 == 0) && (i % 3 == 0))
			log_msg_anyof(ITEMS(LOGGER_FIZZ, LOGGER_BUZZ), "\n");

		sleep(1);
	}

	return 0;
}
