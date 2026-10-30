CC      ?= cc
CFLAGS  ?= -O2 -g
CFLAGS  += -std=c17 -Wall -Wextra -Wpedantic -D_DEFAULT_SOURCE
PREFIX  ?= $(HOME)/.local

SRC := $(wildcard src/*.c)
SRC := $(filter-out src/embed.c,$(SRC))
OBJ := $(SRC:src/%.c=build/%.o) build/embedded.o

# Everything bundled into the binary (for `clings reset` and `clings init`).
EMBED_FILES := info.toml compile_flags.txt exercises/clings.h exercises/README.md \
	$(sort $(wildcard exercises/*/*.c exercises/*/README.md)) \
	$(sort $(wildcard solutions/*/*.c)) solutions/README.md

.PHONY: all clean install uninstall dev-check

all: clings

clings: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

build/%.o: src/%.c src/common.h | build
	$(CC) $(CFLAGS) -c -o $@ $<

build/embed: src/embed.c | build
	$(CC) $(CFLAGS) -o $@ $<

build/embedded.c: build/embed $(EMBED_FILES)
	build/embed $@ $(EMBED_FILES)

build/embedded.o: build/embedded.c src/common.h
	$(CC) $(CFLAGS) -Isrc -c -o $@ $<

build:
	mkdir -p build

dev-check: clings
	./clings dev check

install: clings
	install -Dm755 clings $(DESTDIR)$(PREFIX)/bin/clings

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/clings

clean:
	rm -rf build clings
