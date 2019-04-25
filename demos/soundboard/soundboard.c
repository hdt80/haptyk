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

// Load the config file with the sound data
//
// config_file - Filename relative to the current directory
//
uint8_t load_sounds(const char* config_file);

// Play a sound loaded from the config file
//
// index - Sound index to play
//
void play_sound(uint8_t index);

int main(int argc, char** argv) {
	if (argc != 2) {
		print_usage();
		return 1;
	}

	// Load the sound config
	if (load_sounds("config") == -1) {
		errorf("Failed to load sound config file");
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
		if ((ret_code = haptyk_get_data(&prev_data)) != 0) {
			errorf("Error reading button data from '%s'\n", bt_addr);
		}
	}

	return 0;
}

uint8_t load_sounds(const char* config_file) {
	FILE* sound_file = fopen(config_file, "r");

	if (sound_file == NULL) {
		warnf("Failed to open config file '%s'\n", config_file);
		return -1;
	}

	char* line = NULL;
	ssize_t read;
	size_t len = 0;

	uint8_t sound_index = 0;
	while ((read = getline(&line, &len, sound_file)) != -1) {
		debugf("%d => '%s'\n", sound_index, line);

		free(line);
		line = NULL;
	}

	fclose(sound_file);

	return sound_index;
}
