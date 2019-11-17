#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <include/dcsd_status.h>
#include <include/daemon.h>

void usage(const char *toolname)
{
	fprintf(stdout, "usage : %s [OPTIONS]\n", toolname);
	fprintf(stdout, " -s\tstop deamon\n");
	fprintf(stdout, " -n\tno daemon\n");
	fprintf(stdout, " -r\treset\n");
	fprintf(stdout, " -h\thelp\n");
}

int main(int argc, char const *argv[])
{
	const char *pidfile = "dcsd_status.pid";
	int stop = 0, no_daemon = 0;
	int dcsd_status_pid = 0, killer = -1;

	for(int i = 0; i < argc; i++) {
		if (!strcmp(argv[i], "-s")) {
			stop = 1;
		} else if (!strcmp(argv[i], "-r")) {
			set_led(4);
			return 0;
		} else if (!strcmp(argv[i], "-n")) {
			no_daemon = 1;
		} else if (!strcmp(argv[i], "-h")) {
			usage(argv[0]);
			return 0;
		}
	}

	if (stop) {
		dcsd_status_pid = get_instance_pid(pidfile);

		/* No need to kill something that does not exist*/
		if (dcsd_status_pid == -1)
			return 0;

		printf("[pid] %d\n", dcsd_status_pid);

		killer = kill(dcsd_status_pid, SIGINT);
		if (killer == -1) {
			printf("[e] unable to stop dcsd_status\n");
			return -1;
		}

		remove(pidfile);
		printf("[i] stopped\n");

		return 0;
	}

	if (no_daemon) {
		set_led(device_mode());
	} else {
		daemonize(pidfile);
		while(1) {
			set_led(device_mode());
			sleep(1);
		}
	}

	return 0;
}
