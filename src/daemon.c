#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>

/* name is obvious*/
bool file_exists(const char* file)
{
	if(access(file, F_OK ) != -1) {
		return true;
	} else {
		return false;
	}
}

/* simple function to run the tool as a deamon */
int daemonize(const char *pidfile)
{
	pid_t process_id = 0;
	pid_t sid = 0;
	char *pid_val = NULL;
	size_t len = 0;

	FILE *fp = NULL;

	if (file_exists(pidfile)) {
		fp = fopen(pidfile, "r");

		fprintf(stdout, "[-] dcsd_status is already running\n");
		getline(&pid_val, &len, fp);

		fprintf(stdout, "[i] PID = %s\r", pid_val);
		fclose(fp);
		exit(-1);
	}

	/* create child process */
	process_id = fork();

	/* check if fork() failed */
	if (process_id < 0) {
		perror("fork");
		exit(1);
	}

	/* PARENT PROCESS. Let's kill it. */
	if (process_id > 0) {
		fp = fopen(pidfile, "a+");
		fprintf(stdout, "[i] PID %d\r", process_id);
		fprintf(fp, "%d\n",process_id);
		fclose(fp);
		/* return success in exit status */
		exit(0);
	}

	/* unmask the file mode */
	umask(0);

	/* set new session */
	sid = setsid();
	if(sid < 0) {
		exit(1);
	}


#ifdef DEBUG
	fprintf(stdout, "[i] debug mode\n");
#else
	/* Close stdin. stdout and stderr */
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
#endif

	return 0;
}

int get_instance_pid(const char *file)
{
	FILE *fp = NULL;
	char *pid_val = NULL;
	size_t len = 0;

	fp = fopen(file, "r");

	/* check if we can access file */
	if (fp == NULL) {
		fprintf(stderr, "[e] could not open %s\n", file);
		return -1;
	}

	/* get the first line of the file which is PID */
	getline(&pid_val, &len, fp);
	fclose(fp);

	/* return PID of server */
	return atoi(pid_val);
}
