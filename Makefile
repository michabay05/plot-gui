SRCDIR = src
INCDIR = include
VENDOR_INC_DIR = vendor/raylib/include
OBJDIR_DEBUG = obj/debug
OBJDIR_RELEASE = obj/release
BINDIR_DEBUG = bin/debug
BINDIR_RELEASE = bin/release

COMPILER = gcc
COMMON_COMPILERFLAGS = -Wall -Wextra -pedantic -std=c17 -I$(INCDIR) -I$(VENDOR_INC_DIR)
COMPILERFLAGS_DEBUG = -g
COMPILERFLAGS_RELEASE = -O3
LIB_PATH = vendor/raylib/lib
LDFLAGS = -L$(LIB_PATH) -lraylib -lwinmm -lgdi32 -lopengl32

SOURCES = $(wildcard $(SRCDIR)/*.c)
OBJECTS_DEBUG = $(patsubst $(SRCDIR)/%.c, $(OBJDIR_DEBUG)/%.o, $(SOURCES))
OBJECTS_RELEASE = $(patsubst $(SRCDIR)/%.c, $(OBJDIR_RELEASE)/%.o, $(SOURCES))
BIN_NAME = plot-gui
BINARY_DEBUG = $(BINDIR_DEBUG)/$(BIN_NAME)
BINARY_RELEASE = $(BINDIR_RELEASE)/$(BIN_NAME)

.PHONY: all clean debug debug_setup release release_setup

all: debug release

debug: $(BINARY_DEBUG)

$(BINARY_DEBUG): $(OBJECTS_DEBUG)
	$(COMPILER) $^ -o $@ $(LDFLAGS)

$(OBJDIR_DEBUG)/%.o: $(SRCDIR)/%.c
	$(COMPILER) $(COMMON_COMPILERFLAGS) $(COMPILERFLAGS_DEBUG) -c $< -o $@

release: $(BINARY_RELEASE)

$(BINARY_RELEASE): $(OBJECTS_RELEASE)
	$(COMPILER) $^ -o $@ $(LDFLAGS)

$(OBJDIR_RELEASE)/%.o: $(SRCDIR)/%.c
	$(COMPILER) $(COMMON_COMPILERFLAGS) $(COMPILERFLAGS_RELEASE) -c $< -o $@

clean:
	rm -rf $(OBJDIR_DEBUG)/* $(OBJDIR_RELEASE)/* $(BINDIR_DEBUG)/* $(BINDIR_RELEASE)/*
