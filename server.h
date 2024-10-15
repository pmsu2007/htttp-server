#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>

void setup_environment(char *root, char *user, char *group);
int listen_socket(char *port);
void become_daemon(void);
void server_main(int server_fd, char *docroot);
void service(FILE *in, FILE *out, char *docroot);

#endif
