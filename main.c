#include "main.h"
#include "server.h"
#include "signal_utils.h"
#include "log_utils.h"
#include "constants.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>

int debug_mode = 0;

static struct option longopts[] = {
	{"debug",	no_argument,		&debug_mode, 1},
	{"chroot",	no_argument,		NULL, 'c'},
	{"user",	required_argument, 	NULL, 'u'},
	{"group",	required_argument, 	NULL, 'g'},
	{"port",	required_argument, 	NULL, 'p'},
	{"help",	no_argument,		NULL, 'h'},
	{0, 0, 0, 0}
};

int main(int argc, char *argv[]) {
	int server_fd;
	char *port = DEFAULT_PORT;
	char *docroot;
	int do_chroot = 0;
	char *user = NULL;
	char *group = NULL;
	int opt;

	while ((opt = getopt_long(argc, argv, "", longopts, NULL)) != -1) {
		switch (opt) {
			case 0:
				break;
			case 'c':
				do_chroot = 1;
				break;
			case 'u':
				user = optarg;
				break;
			case 'g':
				group = optarg;
				break;
			case 'p':
				port = optarg;
				break;
			case 'h':
				fprintf(stdout, USAGE, argv[0]);
				exit(0);
			case '?':
				fprintf(stderr, USAGE, argv[0]);
				exit(1);
		}
	}
	
	// check number of arguments passed to program
	if (optind != argc - 1) {
		fprintf(stderr, USAGE, argv[0]);
		exit(1);
	}

	docroot = argv[optind];
	if (do_chroot) {
		setup_environment(docroot, user, group);
		docroot = "";
	}

	install_signal_handlers();
	server_fd = listen_socket(port);

	if (!debug_mode) {
		openlog(SERVER_NAME, LOG_PID | LOG_NDELAY, LOG_DAEMON);
		become_daemon();
	}

	server_main(server_fd, docroot);
	exit(0);
}
