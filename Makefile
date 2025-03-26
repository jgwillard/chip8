CC = cc

# compiler and linker flags
CFLAGS = $(shell sdl2-config --cflags) -Wall -Wextra -std=c11 -g
LDFLAGS = $(shell sdl2-config --libs)

SRCS = main.c chip8.c opcodes.c
OBJS = $(SRCS:.c=.o)
TARGET = chip8

all: compile_commands.json format_json $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "{ \"command\": \"$(CC) $(CFLAGS) -c $< -o $@\", \"directory\": \"$(PWD)\", \"file\": \"$<\" }," >> compile_commands.json

compile_commands.json: clean_json
	@echo "[" > compile_commands.json
	@$(MAKE) --no-print-directory $(OBJS)
	@sed -i '' -e '$$ s/,$$//' compile_commands.json
	@echo "]" >> compile_commands.json

format_json:
	@json_pp < compile_commands.json > tmp.json && mv tmp.json compile_commands.json

clean:
	rm -f $(TARGET) $(OBJS) compile_commands.json

clean_json:
	@rm -f compile_commands.json
