# This Makefile is deprecated in favor of the CMakeLists.txt file, but is going
# to stick around for a little bit until whenever we deem that enough testing
# has been done on the files outputted by the CMake build process.

CC=gcc
WINECC=i686-w64-mingw32-gcc
CFLAGS=-Wall -Os -g -lws2_32

# Build all targets
all: burrito_link.exe

# Create the CLI Executable
burrito_link.exe: burrito_link.c linked_memory.h
	$(WINECC) $< $(CFLAGS) -mconsole -o $@

# Remove all targets and generated files
clean:
	rm -f burrito_link.exe
