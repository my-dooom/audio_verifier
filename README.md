# WAV Generator and Analyzer

A simple C program that generates and analyzes WAV audio files.

## Features

- Generate sinusoidal WAV files with customizable frequency, amplitude, and duration
- Analyze existing WAV files

## Building

To build the project, run:

```bash
make
```

This will compile the source files and create the `analyze_wav` executable.

## Usage

### Generating a WAV file

Run the program to generate a default sinusoidal WAV file:

```bash
./analyze_wav
```

This creates a file called `sinusoid.wav` with the default parameters.

### Analyzing a WAV file

The program can also analyze WAV files (implementation details in `analyze_wav.c`).

## Files

- `main.c` - Main entry point
- `generator.c` - WAV file generation implementation
- `generator.h` - Header file with constants and function declarations
- `analyze_wav.c` - WAV file analysis implementation
- `Makefile` - Build configuration

## Requirements

- C compiler (gcc recommended)
- Standard C libraries