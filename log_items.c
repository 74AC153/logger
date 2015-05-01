#include <string.h>

#include "log_items.h"

#define X(id, name, desc) name,
static const char *item_names[] = {
	LOGGER_ITEMS
};
#undef X

#define ARRLEN(X) (sizeof(X) / sizeof(X[0]))

enum logger_item log_item_id(const char *name)
{
	int i;
	for(i = 0; i < (int)ARRLEN(item_names); i++) {
		if(strcmp(item_names[i], name) == 0)
			break;
	}
	return (enum logger_item) i;
}

const char *log_item_name(enum logger_item item)
{
	if(item >= ARRLEN(item_names))
		return NULL;

	return item_names[item];
}

#define X(id, name, desc) desc,
static const char *item_descriptions[] = {
	LOGGER_ITEMS
};
#undef X

const char *log_item_desc(enum logger_item item)
{
	if(item >= ARRLEN(item_descriptions))
		return NULL;

	return item_descriptions[item];
}
