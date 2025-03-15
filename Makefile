CC = cc

# compiler and linker flags
CFLAGS = $(shell sdl2-config --cflags)
LDFLAGS = $(shell sdl2-config --libs)

SRCS = main.c chip8.c
OBJS = $(SRCS:.c=.o)
TARGET = chip8

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET)
