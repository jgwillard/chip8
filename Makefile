CC = cc

# compiler flags
CFLAGS = $(shell sdl2-config --cflags)

# linker flags
LDFLAGS = $(shell sdl2-config --libs)

SRCS = main.c chip8.c

TARGET = chip8

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) -o $(TARGET) $(SRCS) $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(TARGET)
