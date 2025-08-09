#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <errno.h>

#include "battery-watcher.h"

const char *AC_PATH = "/sys/class/power_supply/AC0";
// This is the file that stores the status of the AC, the "online" provives a 0 or a 1
const char *AC_STATUS_FILE = "online";

unsigned int flags = 0;
const unsigned int flag_verbose	= 	(1<<0);	// 1
const unsigned int flag_lite	=	(1<<1);	// 2

inline void print_help(bool unknown, char *arg)
{
	printf("usage: battery-watcher [-h] [-l] [-v] [-m]\n\n");
	if (unknown){
		printf("battery-watcher: error: Unknown arguments: %s\n", arg);
	} else {
		printf("Reads AC status and detects state changes..\n\n");
		printf("option:\n");
		printf("\t-h\tShow this help message and exit.\n");
		printf("\t-l\tEnables lite mode - run once and play sound depending of current AC state.\n");
		printf("\t-m\tMute - skip playing audio when the state changes.\n");
		printf("\t-v\tEnable verbose output - prints current state every loop.\n");
	}
}

inline bool set_flags(int argc, char **argv){
	for (size_t i = 1; i < argc; i++){
		// If argument found
		if (argv[i][0] == '-')
			switch(argv[i][1]){
				case 'v':
					flags |= flag_verbose;
					break;
				case 'l':
					flags |= flag_lite;
					break;
				case 'h':
					print_help(0, NULL);
					return false;
					break;
				default:
					print_help(1, argv[i]);
					return false;
					break;
			}
	}
	return true;
}

inline bool is_flag_set(unsigned int flag){
	return (flags & flag);
}

inline int verbose_printf(const char *format, ...){
	if(!is_flag_set(flag_verbose))
		return 0;
	size_t len = 0;
	va_list argv;
	va_start(argv, format);

	// Loop through the string
	for (size_t i = 0; format[i] != '\0'; i++)
	{
		if (format[i] != '%'){
			putchar(format[i]);
			len++;
		} else {
			i++;
			switch (format[i]){
				case 's':
				{
					// Get the elipsis arguments
					char *str = va_arg(argv, typeof(str));
					int j;
					for (j = 0; str[j] != '\0'; j++){
						putchar(str[j]);
						len++;
					}
					break;
				}
				/*case 'd': // I don't really need this
					char *str;
					str = va_arg(argv, char *) + '0';
					int j;
					for (j = 0; str[j] != '\0'; j++){
						putchar(str[j]);
						len++;
					}
					break;
					*/
				default:
					break;
			}
		}
	}
	va_end(argv);
	return len;
}