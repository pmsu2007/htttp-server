#include "signal_utils.h"
#include "log_utils.h"
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/errno.h>

void install_signal_handlers(void) {
	trap_signal(SIGPIPE, signal_exit);
	detach_children();
}

void trap_signal(int sig, sighandler_t handler) {
	struct sigaction act;

	act.sa_handler = handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_RESTART;

	if (sigaction(sig, &act, NULL) < 0) {
		log_exit("sigaction() failed: %s", strerror(errno));
	}
}

void detach_children() {
	struct sigaction act;

	act.sa_handler = no_action_handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_RESTART | SA_NOCLDWAIT;

	if (sigaction(SIGCHLD, &act, NULL) < 0) {
		log_exit("sigaction() failed: %s", strerror(errno));
	}
}

void signal_exit(int sig) {
	log_exit("exit by signal %d", sig);
}

void no_action_handler(int sig) {
	;
}
