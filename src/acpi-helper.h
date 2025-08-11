#pragma once
#include <linux/limits.h>

// Change this path according to you system - ACPI is a mess and location of these files vary a lot
extern const char *AC_PATH;
// This is the file that stores the status of the AC, the "online" provives a 0 or a 1
extern const char *AC_STATUS_FILE;

typedef enum Status{
	UNPLUGGED,
	PLUGGED,
}ACStatus;

typedef struct{
	// Location to where the AC power supply status file is located at
	char status_file[PATH_MAX];
	// Holds the current status of the AC power supply status
	ACStatus status;
}ACPI;

extern ACPI acpi;
extern ACStatus acpi_status_previous;

/**
 * @brief Initializes ACPI stuff
 * 
 */
extern void acpi_init();

/**
 * @brief Open file and only read the first character
 * @details Open file and only read the first character, which is the only one relevant, and present
 * 
 * @param status_file Location of the status file
 * @param out Where to output the status read from the file
 * @return int Error codes, same as errno, 0 for no errors
 */
extern int read_status_file(char *status_file, unsigned int *out);
