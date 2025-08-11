#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>

#include "acpi-helper.h"
#include "battery-watcher.h"

char input_programs_plug[MAXIMUM_INPUT_PROGRAMS/2][FILENAME_MAX/2];
char input_programs_unplug[MAXIMUM_INPUT_PROGRAMS/2][FILENAME_MAX/2];
unsigned int programs_quota_plug = 0;
unsigned int programs_quota_unplug = 0;

Flags flags = 0;

// TODO: print help for a single argument
inline void print_help(bool unknown, char *arg)
{
	printf("usage: battery-watcher [-h] [-l] [-v] [-i executable] [-o executable]\n\n");
	if (unknown){
		printf("battery-watcher: error: Unknown arguments: %s\n", arg);
	} else {
		printf("Reads AC status and detects state changes.\n\n");
		printf("option:\n");
		printf("\t-h\tShow this help message and exit.\n");
        printf("\t-i\tRelative path to file that gets executed when AC plugged.\n");
        printf("\t-o\tRelative path to file that gets executed when AC unplugged.\n");
		printf("\t-b\tAbsolute path to ACPI battery status file");
		printf("\t-l\tEnables lite mode - run once and execute file depending of current AC state.\n");
		printf("\t-v\tEnable verbose output - prints current state every loop.\n");
	}
}

inline bool resolve_arguments(int argc, char **argv){
	for (size_t i = 1; i < argc; i++){
		// If argument found
		if (argv[i][0] == '-')
			switch(argv[i][1]){
				// I am fully aware that my code is unsafe — i don't care
                case 'i':
                    if (i+1 >= argc){
                        printf("No input file was given to execute (-i)\n");
                        print_help(false, "i");
                        return false;
                    }
                    strcpy(input_programs_plug[programs_quota_plug], argv[i+1]);
                    programs_quota_plug++;
                    break;
                case 'o':
                    if (i+1 > argc){
                        printf("No input file was given to execute (-o)\n");
                        print_help(false, "o");
                        return false;
                    }
                    strcpy(input_programs_unplug[programs_quota_unplug], argv[i+1]);
                    programs_quota_unplug++;
                    break;
				case 'b':
					if (i+1 > argc){
						printf("No input file was given for ACPI battery status");
						print_help(false, "b");
						return false;
					}
					strcpy(acpi.status_file, argv[i+1]);
					break;
				case 'v':
					flags |= VERBOSE;
					break;
				case 'l':
					flags |= LITE;
					break;
				case 'h':
					print_help(false, NULL);
					return false;
					break;
				default:
					print_help(true, argv[i]);
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
	if(!is_flag_set(VERBOSE))
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

inline int exec_program(const char* path, ...){
    size_t len = 0;
	va_list argv;
	va_start(argv, path);
    pid_t pid = fork();

	if (pid < 0) {
		// Fork failed
		perror("Unable to fork process");
		return errno;
	}

	if (pid == 0) {
		// In child process

		// Suppress stdout/stderr:
		//freopen("/dev/null", "w", stdout);
		//freopen("/dev/null", "w", stderr);

        // Don't know if this is the best way of getting arguments
        // FIXME: God this hurts my brain
        // FODASEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE
        /*
        char args[strlen(path)];
		const char *str = path;
        for (size_t i = 0; path[i] != '\0'; i++){
            str = va_arg(argv, const char*);
            snprintf(args, strlen(str), "%s ", str);
            printf("Executing: %s %s\n", path, str);
        }
        */
        //char * const c_args[sizeof(*args)];
        //str = NULL;

        verbose_printf("Executing: %s\n", path);
        static char *args[]={NULL};
        int err = execv(path, args);

		// If exec fails:
		perror("Unable to execute program");
		exit(EXIT_FAILURE);
        if (err) return err;
	} else {
		// parent: wait for child to finish
		waitpid(pid, NULL, 0);
	}
}