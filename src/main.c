#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <include/dcsd_status.h>
#include <include/deamon.h>

void usage(const char *toolname){
	fprintf(stdout, "usage : %s [OPTIONS]\n", toolname);
	fprintf(stdout, " -s\tstop deamon\n");
	fprintf(stdout, " -h\thelp\n");
}

int main(int argc, char const *argv[])
{	
	const char *pidfile = "dcsd_status.pid";
	int stop = 0, dcsd_status_pid = 0;

	for(int i = 0; i < argc; i++) {
		if (!strcmp(argv[i], "-s"))
		{
			stop = 1;
		} else if (!strcmp(argv[i], "-h")){
			usage(argv[0]);
			return 0;
		}
	}

	if (stop){
		dcsd_status_pid = get_instance_pid(pidfile);
		printf("%d\n", dcsd_status_pid);

		/* No need to kill something that does not exist*/
		if (dcsd_status_pid == -1)
			return 0;

		remove(pidfile);
		kill(dcsd_status_pid, SIGINT);
		printf("[i] stopped\n");
		return 0;
	}

	deamonize(pidfile);
	while(1){
		set_led(device_mode());
		sleep(1);
	}
	return 0;
}
