#include "server.h"
#include "request.h"
#include "response.h"
#include "log_utils.h"
#include "constants.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <grp.h>
#include <pwd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/errno.h>

/* Changing user and group permissions, Restricting file system process can access */
void setup_environment(char *root, char *user, char *group) {
	struct passwd *pw;
	struct group *gr;

	if (!user || !group) {
		fprintf(stderr, "use both of --user and --group\n");
		exit(1);
	}

	gr = getgrnam(group);
	if (!gr) {
		fprintf(stderr, "no such group: %s\n", group);
		exit(1);
	}

	if (setgid(gr->gr_gid) < 0) {
		perror("setgid(2)");
		exit(1);
	}

	if (initgroups(user, gr->gr_gid) < 0) {
		perror("initgroups(2)");
		exit(1);
	}

	pw = getpwnam(user);
	if (!pw) {
		fprintf(stderr, "no such user: %s\n", user);
		exit(1);
	}
	
	chroot(root);
	
	if (setuid(pw->pw_uid) < 0) {
		perror("setuid(2)");
		exit(1);
	}
}

/* Create a listening socket */
int listen_socket(char *port) {
	struct addrinfo hints, *res, *ai;
	int err;

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_INET; // IPv4
	hints.ai_socktype = SOCK_STREAM; // TCP Connection
	hints.ai_flags = AI_PASSIVE; // Server Socket
	
	if ((err = getaddrinfo(NULL, port, &hints, &res)) != 0)
		log_exit(gai_strerror(err));
	
	for (ai = res; ai; ai = ai->ai_next) {
		int sock;

		sock = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
		if (sock < 0) continue;
		
		if (bind(sock, ai->ai_addr, ai->ai_addrlen) < 0) {
			close(sock);
			continue;
		}
		
		if (listen(sock, MAX_BACKLOG) < 0) {
			close(sock);
			continue;
		}

		freeaddrinfo(res);
		return sock;
	}

	log_exit("failed to listen socket");
	return -1;
}

void become_daemon(void) {
	int n;

	if (chdir("/") < 0) {
		log_exit("chdir(2) failed: %s", strerror(errno));
	}

	freopen("/dev/null", "r", stdin);
	freopen("/dev/null", "w", stdout);
	freopen("/dev/null", "w", stderr);
	
	n = fork();
	if (n < 0) {
		log_exit("fork(2) failed: %s", strerror(errno));
	}

	if (n != 0) {
		_exit(0);
	}

	// create new session and detach from terminal
	if (setsid() < 0) {
		log_exit("setsid(2) failed: %s", strerror(errno));
	}
}

/* Accept incoming client connections */
void server_main(int server_fd, char *docroot) {
	
	for (;;) {
		struct sockaddr_storage addr;
		socklen_t addrlen = sizeof addr;
		int sock;
		int pid;

		sock = accept(server_fd, (struct sockaddr*) &addr, &addrlen);
		if (sock < 0) log_exit("accept(2) failed: %s", strerror(errno));

		pid = fork();
		if (pid < 0) exit(3);
		if (pid == 0) {
			FILE *inf = fdopen(sock, "r");
			FILE *outf = fdopen(sock, "w");

			service(inf, outf, docroot);
			exit(0);
		}

		close(sock);
	}
}

/* Process HTTP Request and Return HTTP Response */
void service(FILE *in, FILE *out, char *docroot) {
	struct HTTPRequest *req;

	req = read_request(in);
	respond_to(req, out, docroot);
	
	free_request(req);
}
