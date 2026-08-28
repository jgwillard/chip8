###### Chip-8

A [CHIP-8](https://en.wikipedia.org/wiki/CHIP-8) implementation written in C with a swappable frontend (currently supports [SDL](https://en.wikipedia.org/wiki/CHIP-8)).

##### Build

Dependencies: SDL2

Ensure `sdl2-config` is available in $PATH, then run:

```
make
```

##### Usage

```
./chip8 path/to/rom [--clock-speed speed] [--debug]
```
