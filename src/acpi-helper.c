#include <stdio.h>
#include <errno.h>

#include "acpi-helper.h"

inline int read_status_file(char *status_file, unsigned int *out){
	FILE *file = fopen(status_file, "r");
	if (file == NULL) {
		perror("Error opening AC status file");
		return errno;
	}

	int ch = fgetc(file);
	fclose(file);
	if (ch == EOF) perror("Missing status!");
	else *out = ch - '0'; // Convert char to int by subtracting ascii value of 0

	return 0;
}
