CC     = gcc
CFLAGS = -Wall -std=c99 -Isrc
TARGET = mazerush
SRC    = $(wildcard src/*.c)
HEADERS = $(wildcard src/*.h)

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

$(TARGET)$(EXT): $(SRC) $(HEADERS)
	$(CC) $(SRC) -o $(TARGET)$(EXT) $(CFLAGS) $(LDLIBS)

run: all
	./$(TARGET)$(EXT)

# Testa a logica pura (logic.c e collectible.c) sem abrir janela.
test: tests/test_logic.c src/logic.c src/collectible.c $(HEADERS)
	$(CC) tests/test_logic.c src/logic.c src/collectible.c -o test_logic$(EXT) $(CFLAGS) -lm
	./test_logic$(EXT)

clean:
ifeq ($(OS),Windows_NT)
	cmd /C "if exist $(TARGET).exe del /q $(TARGET).exe"
	cmd /C "if exist test_logic.exe del /q test_logic.exe"
else
	rm -f $(TARGET) $(TARGET).exe test_logic test_logic.exe
endif

.PHONY: all run test clean
