CC = cc

# compiler and linker flags
CFLAGS = $(shell sdl2-config --cflags)
LDFLAGS = $(shell sdl2-config --libs)

SRCS = main.c chip8.c
OBJS = $(SRCS:.c=.o)
TARGET = chip8

all: compile_commands.json $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "{ \"directory\": \"$(PWD)\", \"command\": \"$(CC) $(CFLAGS) -c $< -o $@\", \"file\": \"$<\" }," >> compile_commands.json

compile_commands.json: clean_json
	@echo "[" > compile_commands.json
	@$(MAKE) --no-print-directory $(OBJS)
	@sed -i '' -e '$$ s/,$$//' compile_commands.json
	@echo "]" >> compile_commands.json

clean:
	rm -f $(TARGET) $(OBJS) compile_commands.json

clean_json:
	@rm -f compile_commands.json
