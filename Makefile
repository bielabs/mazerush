CC     = gcc
CFLAGS = -Wall -std=c99
TARGET = mazerush
SRC    = $(wildcard src/*.c)
HEADERS = $(wildcard src/*.h)

ifeq ($(OS),Windows_NT)
    RAYLIB ?= C:/raylib/raylib/src
    CFLAGS += -I$(RAYLIB)
    LDLIBS  = -L$(RAYLIB) -lraylib -lopengl32 -lgdi32 -lwinmm -lshcore
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

clean:
	rm -f $(TARGET) $(TARGET).exe

.PHONY: all run clean
