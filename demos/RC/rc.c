#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "haptyk.h"

#define CLR_RED		"\x1b[31m"
#define CLR_GREEN	"\x1b[32m"
#define CLR_YELLOW	"\x1b[33m"
#define CLR_BLUE	"\x1b[34m"
#define CLR_PURPLE	"\x1b[35m"
#define CLR_CYAN	"\x1b[36m"
#define CLR_RESET	"\x1b[0m"

#if DEBUG
#pragma message "Building debug version"
#endif

#define infof(fmt, ...) \
	do { msgf(stdout, "INFO", CLR_BLUE, fmt, ##__VA_ARGS__); } while (0)

#define warnf(fmt, ...) \
	do { msgf(stdout, "WARN", CLR_YELLOW, fmt, ##__VA_ARGS__); } while (0)

#define errorf(fmt, ...) \
	do { msgf(stderr, "ERRO", CLR_RED, fmt, ##__VA_ARGS__); } while (0)

#if DEBUG
#define debugf(fmt, ...) \
	do { msgf(stdout, "DBUG", CLR_PURPLE, fmt, ##__VA_ARGS__); } while(0)
#else
#define debugf(fmt, ...) (void)0
#endif


void msgf(FILE* file, const char* tag, const char* color, const char* fmt, ...) {
	fprintf(file, "%s[%s%s%s] ", CLR_RESET, color, tag, CLR_RESET);

	va_list args;
	va_start(args, fmt);
	vfprintf(file, fmt, args);
	va_end(args);
}


// Print usage for this demo
//
void print_usage() {
	printf("Usage: ./soundboard <addr>\n");
	printf("\taddr - Bluetooth address to connect");
}

int main(int argc, char** argv) {
	if (argc != 2) {
		print_usage();
		return 1;
	}

	const char* bt_addr = argv[1];

	infof("Haptyk soundboard demo\n");

	infof("Connecting to Haptyk device '%s'\n", bt_addr);
	uint8_t init_code = haptyk_init(bt_addr);
	debugf("Return status from haptyk_init %d\n", init_code);
	if (init_code != 0) {
		errorf("Failed to connect to '%s'\n", bt_addr);
		return 1;
	}
	infof("Connected\n");

	struct haptyk_buttons_t prev_data, curr_data;

	haptyk_get_data(&curr_data);

	while (1) {
		uint8_t ret_code;
		prev_data = curr_data;
		if ((ret_code = haptyk_get_data(&curr_data)) != 0) {
			errorf("Error reading button data from '%s'\n", bt_addr);
		}
			if (curr_data.b3 != 0) //go forward 
					//send F
					//use stty to configure speed, cat to /dev/ttyS0 lol
			else 
					//send f

			if (curr_data.b5 != 0) //go backward
					//send B
			else 
					//send b

			if (curr_data.b0 != 0) //go right
					//send R
			else 
					//send r

			if (curr_data.b7 != 0) //go left
					//send L
			else 
					//send l
		}
	}

	return 0;
}

