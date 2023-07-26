# Plan

If we want to write code, we ought to first make a plan.

## Chip8 System State

- A union (or struct) which represents all of the state of the CHIP8 system, including:
	- registers
	- memory
	- frame buffer
	- etc.

## Algorithm

1. Initialise the CHIP8 system state

2. Load the ROM into memory

3. Enter the main processing loop:

	a. execute current instruction
	b. update the UI (as required)

How does user input (polling) fit into this?