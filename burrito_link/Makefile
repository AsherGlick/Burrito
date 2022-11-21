CC=gcc
WINECC=i686-w64-mingw32-gcc
CFLAGS=-Wall -Os -g -lws2_32

# Build all targets
all: burrito_link.exe

# Create the CLI Executable
burrito_link.exe: gw2mumbleudp.c linked_memory.h
	$(WINECC) $< $(CFLAGS) -mconsole -o $@

# Remove all targets and generated files
clean:
	rm -f burrito_link.exe
