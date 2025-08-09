#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdbool.h>
#include <errno.h>

#include "acpi-helper.h"
#include "battery-watcher.h"

// ##### EXPLANATION #####
// This program will read ACPI status file of AC power supply to see if a change in
// state (whether the AC was plugged or unplugged) happened and runs scripts accordingly
//
// This solution uses an infinite loop to read a file every second (or another custom set delay time)
// to detect state changes and play a set sound, the 'sleep' function is used carefully since it's not
// resource free
//
// Customize the defines to your needs

// ##### PROGRAM DEFINES #####
#define nameof(var) #var

// Main program runs in a while loop, it's highly inefficient to keep reading a file that barely ever changes, save some CPU usage by waiting a little between each loop
#define SLEEP_TIME 1


// ##### MAIN #####
int main(int argc, char **argv){
	// ARGUMENTS
	if(!resolve_arguments(argc, argv))
		return 0;

	// PROGRAM
	ACPI acpi;
	acpi.status = UNPLUGGED;
	// This holds the status number in integer, gets updated only when there's a change in status
	ACStatus acpi_status_previous = PLUGGED;

	// CONVERSIONS
	// Convert the AC define paths into a single string
	snprintf(acpi.status_file, sizeof(acpi.status_file), "%s/%s", AC_PATH, AC_STATUS_FILE);

	// Convert the sounds define into a single string

	// Get current AC status before starting loop
	errno = read_status_file(acpi.status_file, &acpi_status_previous);
	if (errno != 0) return errno;

	// If verbose; print info, otherwise get to looping
	verbose_printf("\033[1,0mPROGRAM STARTING WITH VERBOSE FLAG SET\033[0m\n");
	verbose_printf("%s:\t%s\n", nameof(acpi.status_file), acpi.status_file);
	if (is_flag_set(LITE))
		verbose_printf("Lite mode is enabled\n");
	verbose_printf("\nLOG START:\n");

	// MAIN LOOP
	do {
		// Get current status
		errno = read_status_file(acpi.status_file, &acpi.status);
		if (errno != 0) return errno; // Smooth brain error detection

		// If no change happened, wait and go to next loop iteration
		if (acpi_status_previous == acpi.status && !is_flag_set(LITE)){
			//verbose_printf("No state change detected\n", argv[0]); // This bombs log files lol
			sleep(SLEEP_TIME);
			continue;
		}

		acpi_status_previous = acpi.status;
		switch(acpi.status){
			case PLUGGED:
				verbose_printf("AC was plugged\n");
				for (size_t i = 0; i < programs_quota_plug; i++){
					exec_program(input_programs_plug[i]);
				}
				continue;
				break;
			case UNPLUGGED:
				verbose_printf("AC was unplugged\n");
				for (size_t i = 0; i < programs_quota_unplug; i++){
					exec_program(input_programs_unplug[i]);
				}
				continue;
				break;
			default:
		}		

	} while (!is_flag_set(LITE));

	return 0;
}