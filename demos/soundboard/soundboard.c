#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "haptyk.h"

#define CLR_RESET	"\x1b[0m"
#define CLR_PURPLE	"\x1b[35m"

void logf(FILE* file, const char* tag, const char* color, const char* fmt, ...) {
	fprintf(file, "%s[%s%s%s] ", CLR_RESET, color, tag, CLR_RESET);

	va_list args;
	va_start(args, fmt);
	vfprintf(file, fmt, args);
	va_end(args);
}


#if DEBUG
	#define debugf(F, __VA_ARGS__) logf(stdout, "DBUG", CLR_PURPLSE, F, A)
#else
	#define debugf(F, A) return
#endif

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
		return 1;
	}

	const char* bt_addr = argv[1];

	printf("Haptyk soundboard demo\n");

	printf("Connecting to Haptyk device '%s'\n", bt_addr);
	haptyk_init(bt_addr);
	printf("Connected\n");

	struct haptyk_buttons_t prev_data, curr_data;

	haptyk_get_data(&curr_data);

	while (1) {
		haptyk_get_data(&prev_data);
	}

	return 0;
}

uint8_t load_sounds(const char* config_file) {
	FILE* sound_file = fopen(config_file, "r");

	if (sound_file == NULL) {
		fprintf(stderr, "Failed to open config file '%s'\n", config_file);
		return -1;
	}

	char* line = NULL;
	ssize_t read;
	size_t len = 0;

	uint8_t sound_index = 0;
	while ((read = getline(&line, &len, sound_file)) != -1) {
		debugf("%d => '%s'\n", sound_index, line);
	}

	fclose(sound_file);
	free(line);

	return sound_index;
}
