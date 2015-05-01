#if ! defined(LOG_ITEMS_H_INCLUDED)
#define LOG_ITEMS_H_INCLUDED

/*
The LOGGER_ITEMS macro must be kept up to date with the applicattion's needs.
When this header changes, log_items.c must be recompiled.
*/

#include <stdlib.h>

#define LOGGER_ITEMS \
	X(FIZZ, "fizz", "fizz logging item") \
	X(BUZZ, "buzz", "buzz logging item") \
	X(NUM_ITEMS, NULL, NULL) // required last element

#define X(id, name, desc) LOGGER_ ## id,
enum logger_item {
	LOGGER_ITEMS
};
#undef X

// returns LOGGER_NUM_ITEMS if name not found
enum logger_item log_item_id(const char *name);
const char *log_item_name(enum logger_item item);
const char *log_item_desc(enum logger_item item);

#endif
