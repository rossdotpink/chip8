#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"

typedef union chip8 {
  uint8_t v_register[NUM_REGISTERS];
  uint16_t index_register;
  uint8_t stack[SIZE_STACK];
  uint8_t stack_pointer;
  uint8_t delay_timer;
  uint8_t sound_timer;
  uint64_t frame_buffer[32]; // TODO is this the best idea?
  long current_rom_size;
  uint8_t memory[SIZE_RAM];
} chip8_t;

int load_rom_file(char *, chip8_t *);

int main(int argc, char* argv[]) {
  chip8_t * system = (chip8_t *) malloc(sizeof(chip8_t));        // Declare our system state object

  load_rom_file(argv[1], system);

  for(int i = 0; i < system->current_rom_size; i += 2) {
    printf("0x%03x\t%02x%02x\n", (MEM_OFFSET+(i*4)), system->memory[MEM_OFFSET+i], system->memory[MEM_OFFSET+i+1]);
//    if (memcmp(*(buffer+i), zero, 1) == 0) printf("\n Haha");
  }

  return OK;
}

/*
 * Given the file path to a ROM file, and a pointer to the system state, load
 * the ROM file into memory.
 *
 * For CHIP8, this should occur at a specific offset. TODO link to docs.
 */
int
load_rom_file(char * file_path, chip8_t * system) {
  printf("Trying to load %s\n", file_path);

  // Open the file in binary mode
  FILE *romPtr = fopen(file_path, "rb");
  if (romPtr == NULL) {
    printf(FILE_NOT_FOUND);
    exit(1);
  }

  // Seek to the end of the tile
  fseek(romPtr, 0, SEEK_END);

  // Get the current byte offset in the file
  system->current_rom_size = ftell(romPtr);

  // Jump back to the beginning of the file
  printf("File is %ld bytes long\n", system->current_rom_size);

  // Rewind pointer back to start of file
  rewind(romPtr);

  // Read ROM into memory starting at 0x200 offset
  fread(system->memory+MEM_OFFSET, system->current_rom_size, 1, romPtr);

  // Close file pointer
  fclose(romPtr);

  return(OK);
}

int
cls(chip8_t* system) {
  for(int i = 0; i < 32; i++)
    system->frame_buffer[i] = 0;
  return OK;
}

/*
 * 00E0 - CLS
Clear the display.

00EE - RET
Return from a subroutine.

The interpreter sets the program counter to the address at the top of the stack, then subtracts 1 from the stack pointer.

1nnn - JP addr
Jump to location nnn.

The interpreter sets the program counter to nnn.

2nnn - CALL addr
Call subroutine at nnn.

The interpreter increments the stack pointer, then puts the current PC on the top of the stack. The PC is then set to nnn.
 */