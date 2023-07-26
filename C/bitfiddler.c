#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "constants.h"
#include "log.c/src/log.h"

/*
  Get nibbles (4 bits)
*/
uint8_t get_first_nibble(uint16_t* instruction) {
  uint8_t nibble = (*instruction >> 12);
  return nibble;
}

uint8_t get_second_nibble(uint16_t* instruction) {
  uint8_t nibble = ((*instruction & MASK_0F00) >> 8);
  return nibble;
}

uint8_t get_third_nibble(uint16_t* instruction) {
  uint8_t nibble = ((*instruction & MASK_00F0) >> 4);
  return nibble;
}

uint8_t get_fourth_nibble(uint16_t* instruction) {
  return *instruction & MASK_000F;
}

/*
  Get bytes (8 bits)
*/

uint8_t get_first_byte(uint16_t* instruction) {
  uint8_t byte = (*instruction >> 16);
  return byte;
}

uint8_t get_second_byte(uint16_t* instruction) {
  uint8_t byte = (*instruction & MASK_00FF);
  return byte;
}

/*
  Get byte and half (12 bits)
*/

uint16_t get_last_12_bits(uint16_t* instruction) {
  uint16_t byte = (*instruction & MASK_0FFF);
  return byte;
}