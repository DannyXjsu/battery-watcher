#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "acpi-helper.h"

const char *AC_PATH = "/sys/class/power_supply/AC0";
const char *AC_STATUS_FILE = "online";

ACPI acpi;
ACStatus acpi_status_previous;

inline void acpi_init(){
	acpi.status = UNPLUGGED;
	acpi.status_file[0] = '\0';
	// This holds the status number in integer, gets updated only when there's a change in status
	ACStatus acpi_status_previous = PLUGGED;
}

inline int read_status_file(char *status_file, unsigned int *out){
	FILE *file = fopen(status_file, "r");
	if (file == NULL) {
		perror("Error opening AC status file");
		printf("%s:\t%s\n", "status_file", status_file);
		return errno;
	}

	int ch = fgetc(file);
	fclose(file);
	if (ch == EOF) perror("Missing status!");
	else *out = ch - '0'; // Convert char to int by subtracting ascii value of 0

	return 0;
}
