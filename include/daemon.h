#include <stdbool.h>

bool file_exists(const char* file);
int daemonize(const char *pidfile);
int get_instance_pid(const char *file);
