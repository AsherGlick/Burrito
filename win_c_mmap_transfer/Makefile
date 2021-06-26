# all: gw2mumbleudp.exe
all: a.exe

CC=gcc
WINECC=i686-w64-mingw32-gcc
CFLAGS=-Wall -Os -g -lws2_32

# assets/shmwrapper2.bin: shmwrapper2.c
# 	$(CC) $< $(CFLAGS) -o $@

# gw2mumbleudp.exe: gw2mumbleudp.c
a.exe: gw2mumbleudp.c
	$(WINECC) $< $(CFLAGS) -mconsole -o $@

# bindata.go: assets/shmwrapper1.exe assets/shmwrapper2.bin
# 	go-bindata -pkg=wineshm -ignore=.gitkeep assets/

clean:
	rm -f assets/shmwrapper1.exe assets/shmwrapper2.bin bindata.go

# vim: syntax=make ts=4 sw=4 sts=4 sr noet
