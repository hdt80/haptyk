#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <vector>
#include <string>
#include <fstream>

extern "C" {
	#include "haptyk.h"
}

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

#define BUTTON_IS_NOW_PRESSED(x) \
	curr_data.x == 0 && prev_data.x == 1

#define BUTTON_NOW_NOT_PRESSED(x) \
	curr_data.x == 1 && prev_data.x == 0

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
uint8_t load_sounds(const std::string& config_file);

// Play a sound loaded from the config file
//
// index - Sound index to play
//
void play_sound(uint8_t index);

std::vector<std::string> sound_file;

int main(int argc, char** argv) {
	if (argc != 2) {
		print_usage();
		return 1;
	}

	uint8_t sound_code = load_sounds("config");
	// Load the sound config
	if (sound_code == -1) {
		errorf("Failed to load sound config file");
		return 1;
	}
	infof("Loaded %d sounds\n", sound_code);

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
		haptyk_print(&prev_data);

		if (BUTTON_IS_NOW_PRESSED(b0)) { play_sound(0); }
		if (BUTTON_IS_NOW_PRESSED(b1)) { play_sound(1); }
		if (BUTTON_IS_NOW_PRESSED(b2)) { play_sound(2); }
		if (BUTTON_IS_NOW_PRESSED(b3)) { play_sound(3); }
		if (BUTTON_IS_NOW_PRESSED(b4)) { play_sound(4); }
		if (BUTTON_IS_NOW_PRESSED(b5)) { play_sound(5); }
		if (BUTTON_IS_NOW_PRESSED(b6)) { play_sound(6); }
		if (BUTTON_IS_NOW_PRESSED(b7)) { play_sound(7); }
		if (BUTTON_IS_NOW_PRESSED(b8)) { play_sound(8); }
		if (BUTTON_IS_NOW_PRESSED(b9)) { play_sound(9); }
		if (BUTTON_IS_NOW_PRESSED(b10)) { play_sound(10); }
		if (BUTTON_IS_NOW_PRESSED(b11)) { play_sound(11); }

		curr_data = prev_data;
	}

	return 0;
}

uint8_t load_sounds(const std::string& config_file) {
	std::ifstream file(config_file);
	std::string line;

	uint8_t sound_files = 0;

	while (std::getline(file, line)) {
		debugf("Read '%s'\n", line.c_str());

		std::ifstream sfile(line);
		if (sfile.good()) {
			sound_file.push_back(line);
			++sound_files;
		} else {
			warnf("Failed to open '%s'\n", line.c_str());
		}
	}

	return sound_files;
}

void play_sound(uint8_t index) {
	if (index > sound_file.size()) {
		warnf("%d not loaded, only %d loaded", index, sound_file.size());
		return;
	}

	const std::string& sound_name = sound_file[index];

	system(("aplay " + sound_name).c_str());
}
