#pragma once

// Maximum amount of programs to execute
#define MAXIMUM_INPUT_PROGRAMS 32
// Maximum size of the filename for the input programs
//#define FILENAME_MAX 256

// The programs to execute 
// divided bc shared between plug and unplug
extern char input_programs_plug[MAXIMUM_INPUT_PROGRAMS/2][FILENAME_MAX/2];
extern char input_programs_unplug[MAXIMUM_INPUT_PROGRAMS/2][FILENAME_MAX/2];
// Total amount of input files to execute
extern unsigned int programs_quota_plug;
extern unsigned int programs_quota_unplug;

// TODO: change this to an enum
// ##### FLAGS #####
/**
 * @brief Program flags
 */
typedef enum EFlags{
    INPUT_PLUGGED = (1<<0), // @brief Inputs a file to execute when power supply is plugged
    INPUT_UNPLUGGED = (1<<1), // @brief Inputs a file to execute when power supply is unplugged
    INPUT_ACTATUS = (1<<2), // @brief Inputs a file to execute when power supply is unplugged
	VERBOSE = (1<<3), // @brief Prints information to the screen
	LITE = (1<<4), // @brief Run a single loop iteration
}Flags;
/** @brief Global flags variable */
extern Flags flags;

/**
 * @brief Print help message
 * 
 * @param unknown Help message when an unknown argument was passed
 * @param arg Which argument was given - Pass null if irrelevant
 */
extern void print_help(bool unknown, char *arg);
/**
 * @brief Set all flags with the passed arguments
 * 
 * @param argc Program main's argc
 * @param argv Program main's argv
 * @return true Should continue to main loop
 * @return false Program should exit 
 */
extern bool resolve_arguments(int argc, char **argv);
/**
 * @brief Check if flag is set in the global flags variable
 * 
 * @param flag 
 * @return true Flag was set
 * @return false Flag was not set
 */
extern bool is_flag_set(unsigned int flag);
/**
 * @brief Prints to stdout only if the verbose flag is set
 * 
 * @param format The string to format
 * @param ... Pass variables here
 * @return int The length of the final printed string
 * @note Only supports strings '%s'
 */
extern int verbose_printf(const char *format, ...);

/**
 * @brief Execute a program
 * 
 * @param path Path to where this program is located, environment path can be used as well
 * @param ... Arguments to pass to the program
 * @return int The program exit code
 */
extern int exec_program(const char* path, ...);