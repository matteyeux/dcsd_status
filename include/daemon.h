#include <stdbool.h>
bool file_exists(const char* file);
int deamonize(const char *pidfile);
int get_instance_pid(const char *file);
