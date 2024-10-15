#include "common_utils.h"
#include "log_utils.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

void* xmalloc(size_t sz) {
	void *p;

	p = malloc(sz);
	if (!p) log_exit("failed to allocate memory");
	
	return p;
}

void upcase(char *str) {
	char *p;

	for (p = str; *p; p++) {
		*p = (char)toupper((int) *p);
	}

}
