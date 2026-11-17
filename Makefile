CC     = gcc
CFLAGS = -Wall -std=c99
TARGET = mazerush
SRC    = $(wildcard src/*.c)

ifeq ($(OS),Windows_NT)
    RAYLIB ?= C:/raylib/w64devkit
    CC       = $(RAYLIB)/bin/gcc.exe
    export PATH := $(RAYLIB)/bin;$(PATH)
    CFLAGS += -I$(RAYLIB)/include
    LDLIBS  = -L$(RAYLIB)/lib -lraylib -lopengl32 -lgdi32 -lwinmm -lshcore
    EXT     = .exe
else ifeq ($(shell uname -s),Darwin)
    LDLIBS  = -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
else
    LDLIBS  = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
endif

all: $(TARGET)$(EXT)

$(TARGET)$(EXT): $(SRC)
	$(CC) $(SRC) -o $(TARGET)$(EXT) $(CFLAGS) $(LDLIBS)

run: all
	./$(TARGET)$(EXT)

clean:
ifeq ($(OS),Windows_NT)
	cmd /C "if exist $(TARGET).exe del /q $(TARGET).exe"
else
	rm -f $(TARGET) $(TARGET).exe
endif

.PHONY: all run clean