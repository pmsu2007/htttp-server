#ifndef REQUEST_H
#define REQUEST_H

#include <stdio.h>

struct HTTPHeaderField {
	char *name;
	char *value;
	struct HTTPHeaderField *next;
};

struct HTTPRequest {
	int protocol_minor_version;
	char *method;
	char *path;
	struct HTTPHeaderField *header;
	char *body;
	long length;
};

struct HTTPRequest* read_request(FILE *in);
void read_request_line(struct HTTPRequest *req, FILE *in);
struct HTTPHeaderField* read_header_field(FILE *in);
void free_request(struct HTTPRequest *req);
long content_length(struct HTTPRequest *req);
char* lookup_header_field_value(struct HTTPRequest *req, char *name);

#endif
