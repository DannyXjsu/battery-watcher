#pragma once

// ##### FLAGS #####
/**
 * @brief Global flags variable
 * 
 */
extern unsigned int flags;
/**
 * @brief Verbose flag binary value
 * 
 */
extern const unsigned int flag_verbose;
/**
 * @brief Lite mode flag binary value
 * 
 */
extern const unsigned int flag_lite;

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
extern bool set_flags(int argc, char **argv);
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