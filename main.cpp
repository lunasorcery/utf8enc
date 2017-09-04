#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <unistd.h>

bool isHexChar(char c) {
	if (c >= '0' && c <= '9')
		return true;
	if (c >= 'A' && c <= 'F')
		return true;
	if (c >= 'a' && c <= 'f')
		return true;
	return false;
}

bool isValidInput(const char* arg) {
	if (strlen(arg) < 3)
		return false;
	if (arg[0] != 'u' && arg[0] != 'U')
		return false;
	if (arg[1] != '+')
		return false;
	for (int i = 2; i < strlen(arg); ++i)
		if (!isHexChar(arg[i]))
			return false;
	return true;
}

uint32_t parseInput(const char* arg) {
	return strtol(arg + 2, nullptr, 16);
}

void printValueAsUTF8(uint32_t value) {
	char buffer[5];
	if (value <= 0x7f) {
		buffer[0] = value & 0x7F;
		buffer[1] = 0x00;
	} else if (value <= 0x7FF) {
		buffer[0] = 0xC0 | ((value >> 6) & 0x1F);
		buffer[1] = 0x80 | (value & 0x3F);
		buffer[2] = 0x00;
	} else if (value <= 0xFFFF) {
		buffer[0] = 0xE0 | ((value >> 12) & 0x0F);
		buffer[1] = 0x80 | ((value >> 6) & 0x3F);
		buffer[2] = 0x80 | (value & 0x3F);
		buffer[3] = 0x00;
	} else if (value <= 0x10FFFF) {
		buffer[0] = 0xF0 | ((value >> 18) & 0x7);
		buffer[1] = 0x80 | ((value >> 12) & 0x3F);
		buffer[2] = 0x80 | ((value >> 6) & 0x3F);
		buffer[3] = 0x80 | (value & 0x3F);
		buffer[4] = 0x00;
	} else {
		return;
	}
	printf("%s", buffer);
}

int main(int argc, char** argv) {
	for (int i = 1; i < argc; ++i) {
		if (!isValidInput(argv[i])) {
			printf("Malformed codepoint '%s'\n", argv[i]);
			return 1;
		}
		uint32_t value = parseInput(argv[i]);
		if (value > 0x10FFFF) {
			printf("Codepoint '%s'\n is too large to represent in UTF-8\n", argv[i]);
		}
		printValueAsUTF8(value);
	}
	if (argc > 1 && isatty(fileno(stdout))) {
		printf("\n");
	}
}
