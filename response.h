#ifndef RESPONSE_H
#define RESPONSE_H

#include <stdio.h>

struct HTTPRequest;

struct FileInfo {
	char *path;
	long size;
	int ok;
};

void respond_to(struct HTTPRequest *req, FILE *out, char *docroot);

void do_file_response(struct HTTPRequest *req, FILE *out, char *docroot);
void output_common_header_fields(struct HTTPRequest *req, FILE *out, char *status);

void method_not_allowed(struct HTTPRequest *req, FILE *out);
void not_implemented(struct HTTPRequest *req, FILE *out);
void not_found(struct HTTPRequest *req, FILE *out);

struct FileInfo* get_fileinfo(char *docroot, char *path);
char* build_fspath(char *docroot, char *urlpath);
void free_fileinfo(struct FileInfo *info);
char* guess_content_type(struct FileInfo *info);

#endif
