#ifndef SIGNAL_UTILS_H
#define SIGNAL_UTILS_H

typedef void (*sighandler_t)(int);

void install_signal_handlers(void);
void trap_signal(int sig, sighandler_t handler);
void detach_children(void);
void signal_exit(int sig);
void no_action_handler(int sig);

#endif
