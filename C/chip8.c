#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

union CHIP8 {
  uint8_t v_register[16];
  uint16_t index_register;
  uint8_t stack[64];
  uint8_t stack_pointer;
  uint8_t delay_timer;
  uint8_t sound_timer;
  uint64_t frame_buffer[32]; // TODO is this the best idea?
  uint8_t memory[4096];
};

int main() {

  union CHIP8 *chip8 = malloc(sizeof(*chip8));
  FILE *romPtr = fopen("test.ch8", "rb"); // Open the file in binary mode
  uint8_t *zero ;

  uint8_t *buffer;
  char opcode[4];
  long filelen;

  fseek(romPtr, 0, SEEK_END);          // Jump to the end of the file
  filelen = ftell(romPtr);             // Get the current byte offset in the file
  printf("File is %ld bytes long\n", filelen);
  rewind(romPtr);                      // Jump back to the beginning of the file

  buffer = (uint8_t *)malloc(filelen * sizeof(uint8_t)); // Enough memory for the file
  fread(buffer, filelen, 1, romPtr); // Read in the entire file
  for(int i = 0; i < filelen; i += 2) {
    printf("%02x%02x\n", buffer[i], buffer[i+1]);
    if (memcmp(*(buffer+i), zero, 1) == 0) printf("\n Haha");
  }
    fclose(romPtr); // Close the file

  return 0;
}
