#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "constants.h"
#include "log.c/src/log.h"
#include "bitfiddler.c"

typedef struct _chip8_t chip8_t;
struct _chip8_t {
  useconds_t sleep_duration;
  long current_rom_size;
  uint16_t index_register;
  uint16_t memory[SIZE_RAM / 2];
  uint16_t program_counter;
  uint64_t frame_buffer[32]; // TODO is this the best idea?
  uint8_t delay_timer;
  uint8_t sound_timer;
  uint8_t stack[SIZE_STACK];
  uint8_t stack_pointer;
  uint8_t v_register[NUM_REGISTERS];
};

int load_rom_file(char *, chip8_t *);
chip8_t* init_chip8();
uint16_t fetch(chip8_t *);
void loop(chip8_t *);
void cls(chip8_t *);
void load_i(chip8_t*, uint16_t);

int main(int argc, char* argv[]) {


  chip8_t* system = init_chip8();
  log_trace("main() system = %p", system);

  load_rom_file(argv[1], system);
  
  log_trace("main() - DT         = %u!!", system->delay_timer);
  log_trace("main() - PC         = %u!!", system->program_counter);
  log_trace("main() - MEM_OFFSET = %d!!", MEM_OFFSET);

  log_trace("0x%03x\t%02x%02x", 
  (system->program_counter), 
  system->memory[MEM_OFFSET], 
  system->memory[MEM_OFFSET+1]);

  loop(system);

  // for(int i = 0; i < system->current_rom_size; i += 2) {
  //   log_trace("0x%03x\t%02x%02x", (MEM_OFFSET+(i*4)), system->memory[MEM_OFFSET+i], system->memory[MEM_OFFSET+i+1]);
  // }

  return OK;
}

/*
  Allocates memory for a chip8_t object and returns a pointer to it.
*/
chip8_t * init_chip8(void) {
  // Allocate memory for our system state object
  chip8_t * system = malloc(sizeof(chip8_t));

  log_trace("init_chip8() system = %p", system);

  if (!system) {
      log_error("Error, unable to allocate memory.");
      exit(EXIT_FAILURE);
  }

  *system = (chip8_t) { 
    .sleep_duration = 100000,
    .stack_pointer = 0x0,
    .program_counter = 512,
    .memory = {
      0xF0,0x90,0x90,0x90,0xF0,
      0x20,0x60,0x20,0x20,0x70,
      0xF0,0x10,0xF0,0x80,0xF0,
      0xF0,0x10,0xF0,0x10,0xF0,
      0x90,0x90,0xF0,0x10,0x10,
      0xF0,0x80,0xF0,0x10,0xF0,
      0xF0,0x80,0xF0,0x90,0xF0,
      0xF0,0x10,0x20,0x40,0x40,
      0xF0,0x90,0xF0,0x90,0xF0,
      0xF0,0x90,0xF0,0x10,0xF0,
      0xF0,0x90,0xF0,0x90,0x90,
      0xE0,0x90,0xE0,0x90,0xE0,
      0xF0,0x80,0x80,0x80,0xF0,
      0xE0,0x90,0x90,0x90,0xE0,
      0xF0,0x80,0xF0,0x80,0xF0,
      0xF0,0x80,0xF0,0x80,0x80
    }
  };

  return system;
}

void loop(chip8_t * system) {
  while(true) {
    usleep(system->sleep_duration);

    uint16_t instruction = fetch(system);
    log_debug("Current Instruction 0x%04X @ PC: 0x%03X \
      1st Word: 0x%04X \
      2nd Word: 0x%04X \
      3rd Word: 0x%04X \
      4th Word: 0x%04X"
      , instruction
      , system->program_counter
      , get_first_nibble(&instruction)
      , get_second_nibble(&instruction)
      , get_third_nibble(&instruction)
      , get_fourth_nibble(&instruction));
    system->program_counter++;

    if (instruction == CLS) {
      cls(system);
      log_info("Call to CLS.");
      continue;
    }

    if (instruction == RET) {
      // 00EE - RET
      // Return from a subroutine.
      //
      // The interpreter sets the program counter to the address at the top of the stack,
      system->program_counter = system->stack[system->stack_pointer];
      // then subtracts 1 from the stack pointer.
      system->stack_pointer--;
      log_info("Call to RET.");
      continue;
    }

    // if ((instruction & JMPS) == JMPS) {
    //   log_warn("JMPS is not implemented.");
    // }

    if ((instruction & MASK_JMP) == MASK_JMP) {
      log_warn("JMP is not implemented.");
      continue;
    }

    uint8_t first_nibble = get_first_nibble(&instruction);
    uint16_t last_12_bits = get_first_nibble(&instruction);
    uint8_t x = get_second_nibble(&instruction);
    uint8_t kk = get_second_byte(&instruction);

    switch (first_nibble)
    {
      case JMP:
        // 1nnn - JP addr
        // Jump to location nnn.
        //
        // The interpreter sets the program counter to nnn.
        system->program_counter = (instruction & MASK_0FFF);
        log_info("Call to JMP (1nnn).");
        continue;
      case CALL:
        // 2nnn - CALL addr
        // Call subroutine at nnn.
        //
        // The interpreter increments the stack pointer, 
        system->stack_pointer++;
        // then puts the current PC on the top of the stack. 
        system->stack[system->stack_pointer] = system->program_counter;
        // The PC is then set to nnn.
        system->program_counter = (instruction & MASK_0FFF);
        log_info("Call to CALL (2nnn).");
        continue;

      case SKEQ:
        // 3xkk - SE Vx, byte
        // Skip next instruction if Vx = kk.
        //
        // The interpreter compares register Vx to kk,

        // and if they are equal, 
        if (system->v_register[x] == kk)
          // increments the program counter by 2.
          // We will increment PC once on the next loop,
          // so we only do it once here
          system->program_counter++;
        log_info("Call to SKEQ (2nnn).");
        continue;

      case SKNE:
        // 4xkk - SNE Vx, byte
        // Skip next instruction if Vx != kk.
        //
        // The interpreter compares register Vx to kk, and if they are not equal, increments the program counter by 2.
        if (system->v_register[x] != kk)
          // increments the program counter by 2.
          // We will increment PC once on the next loop,
          // so we only do it once here
          system->program_counter++;
        log_info("Call to SKEQ (2nnn).");
        continue;
      case LDI:
        // 
        load_i(system, last_12_bits);
        log_info("Call to LDI (Annn).");
        continue;
      case DRAW:
        // Dxyn - DRW Vx, Vy, nibble
        // Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.

        // The interpreter reads n bytes from memory, starting at the address stored in I.
        // These bytes are then displayed as sprites on screen at coordinates (Vx, Vy).
        // Sprites are XORed onto the existing screen.
        // If this causes any pixels to be erased, VF is set to 1, otherwise it is set to 0.
        // If the sprite is positioned so part of it is outside the coordinates of the display,
        // it wraps around to the opposite side of the screen.
        // See instruction 8xy3 for more information on XOR, and section 2.4, 
        // Display, for more information on the Chip-8 screen and sprites.

      default:
       break;
    }

  }
}
/*
  Return the instruction pointed to by the Program Counter.
*/
uint16_t fetch(chip8_t * system) {
  uint16_t instruction = system->memory[system->program_counter];
  // Convert to little-endian format
  instruction = htons(instruction);
  // log_debug("Current Instruction %04X @ PC: 0x%03X", instruction, system->program_counter);
  // log_trace("0x%03x\t%02x%02x", (system->program_counter), system->memory[MEM_OFFSET], system->memory[MEM_OFFSET+1]);

  return instruction;
}

uint16_t parse(chip8_t * system, uint16_t instruction) {

}

/*
 * Given the file path to a ROM file, and a pointer to the system state, load
 * the ROM file into memory.
 *
 * For CHIP8, this should occur at a specific offset. TODO link to docs.
 */
int load_rom_file(char * file_path, chip8_t * system) {
  // log_trace("Trying to load %s", file_path);

  // Open the file in binary mode
  FILE *romPtr = fopen(file_path, "rb");
  if (romPtr == NULL) {
    // log_trace(FILE_NOT_FOUND);
    exit(1);
  }

  // Seek to the end of the tile
  fseek(romPtr, 0, SEEK_END);

  // Get the current byte offset in the file
  system->current_rom_size = ftell(romPtr);

  // Rewind pointer back to start of file
  rewind(romPtr);

  // Read ROM into memory starting at 0x200 offset
  fread(system->memory+MEM_OFFSET, system->current_rom_size, 1, romPtr);

  // Close file pointer
  fclose(romPtr);

  return(OK);
}

/*
  00E0 - CLS
  Clear the display.
*/
void cls(chip8_t* system) {
  for(int i = 0; i < 32; i++)
    system->frame_buffer[i] = 0;
  // return OK;
}

/*
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
/*
  Annn - LD I, addr
  Set I = nnn.

  The value of register I is set to nnn.
 */
void load_i(chip8_t* system, uint16_t address) {
  system->index_register = address;
}