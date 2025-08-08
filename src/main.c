#include "fswatch.h"

int main(const int argc, char** argv) {

	// TODO: Improve argument parsing and add safe-guards
	if (argc < 1)
			return -1;

	const char* path = argv[ARG_FILE_IDX];

	printf("Watching file: '%s'\n", path);

	watch_directory(path);

	return 0;
}