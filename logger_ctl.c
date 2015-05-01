#include <stdio.h>
#include <string.h>

/*
This program must be built with the same log_items.h as the application you
are planning on controlling.
*/

// logging infrastructure
#include "logger.h"

// application-specific log items
#include "log_items.h"

int main(int argc, char *argv[])
{
	int exit_status = 0;

	if(argc < 2) {
usage:
		printf("usage: %s <subcmd> <args...>\n"
		       "<subcmd> can be one of:\n"
		       "help             -- this text\n"
		       "list             -- list all log IDs\n"
		       "info <ids...>    -- list descriptions for the specified ids\n"
		       "status <ids...>  -- display the log status for the specified ids\n"
		       "enable <ids...>  -- enable logging for the specified IDs\n"
		       "disable <ids...> -- disable logging for the specified IDs\n"
		       "disable_all      -- disable all logging\n"
		       "\n"
		       "The LOGGERCTLFILE environment variable must be set\n", argv[0]);
		return exit_status;
	}

	if(getenv("LOGGERCTLFILE") == NULL) {
		fprintf(stderr, "error: LOGGERCTLFILE unset\n");
		goto usage;
	}

	if(logger_init(getenv("LOGGERCTLFILE")))
		goto usage;

	if(strcmp(argv[1], "list") == 0) {

		for(int i = 0; i < LOGGER_NUM_ITEMS; i++) {
			printf("%s\n", log_item_name(i));
		}

	} else if(strcmp(argv[1], "info") == 0) {

		if(argc == 2) {
			for(enum logger_item id = 0; id < LOGGER_NUM_ITEMS; id++) {
				const char *name = log_item_name(id);
				const char *desc = log_item_desc(id);
				printf("%s -- %s\n", name, desc);
			}
		} else {
			for(int i = 2; i < argc; i++) {
				enum logger_item id = log_item_id(argv[i]);
				if(id == LOGGER_NUM_ITEMS) {
					printf("unknown log id: %s\n", argv[i]);
				} else {
					const char *desc = log_item_desc(id);
					printf("%s -- %s\n", argv[i], desc);
				}
			}
		}

	} else if(strcmp(argv[1], "status") == 0) {

		if(argc == 2) {
			for(enum logger_item id = 0; id < LOGGER_NUM_ITEMS; id++) {
				const char *name = log_item_name(id);
				printf("%s %s\n", log_enabled(id) ? "ON" : "OFF", name);
			}
		} else {
			for(int i = 2; i < argc; i++) {
				enum logger_item id = log_item_id(argv[i]);
				if(id == LOGGER_NUM_ITEMS) {
					continue;
				}

				printf("%s %s\n", log_enabled(id) ? "ON" : "OFF", argv[i]);
			}
		}

	} else if(strcmp(argv[1], "enable") == 0) {

		for(int i = 2; i < argc; i++) {
			enum logger_item id = log_item_id(argv[i]);
			if(id == LOGGER_NUM_ITEMS) {
				continue;
			}
			log_enable(id);
		}

	} else if(strcmp(argv[1], "disable") == 0) {

		for(int i = 2; i < argc; i++) {
			enum logger_item id = log_item_id(argv[i]);
			if(id == LOGGER_NUM_ITEMS) {
				continue;
			}
			log_disable(id);
		}

	} else if(strcmp(argv[1], "disable_all") == 0) {

		log_disable_all();

	} else if(strcmp(argv[1], "help") == 0 ||
	          strcmp(argv[1], "--help") == 0 ||
	          strcmp(argv[1], "-h") == 0) {

		goto usage;

	} else {

		printf("uknown command: %s\n", argv[1]);
		exit_status = -1;
		goto usage;

	}

	return 0;
}
