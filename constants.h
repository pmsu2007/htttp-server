#ifndef CONSTANTS_H
#define CONSTANTS_H

#define USAGE "Usage: %s [--port=n] [--chroot --user=u --group=g] [--debug] <docroot> \n"

#define SERVER_NAME "Simple HTTP"
#define SERVER_VERSION "1.0"
#define HTTP_MINOR_VERSION 0

#define DEFAULT_PORT "80"
#define MAX_BACKLOG 5

#define BLOCK_BUF_SIZE 1024
#define LINE_BUF_SIZE 4096
#define TIME_BUF_SIZE 64
#define MAX_REQUEST_BODY_LENGTH (1024 * 1024)

#endif
