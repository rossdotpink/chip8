//
// Created by Ross Wilson on 09/07/2023.
//

#ifndef C_CONSTANTS_H
#define C_CONSTANTS_H

// typedef status int;

const int NUM_REGISTERS = 16;
const int SIZE_STACK = 64;
const int SIZE_RAM = 4096;
const char FILE_NOT_FOUND[] = "Could not load the specified ROM file!\n Exiting.\n";

const int OK = 0;
const int NOT_OK = 1;

const int MEM_OFFSET = 0x200;

const uint16_t MASK_000F = 0x000F;
const uint16_t MASK_00F0 = 0x00F0;
const uint16_t MASK_00FF = 0x00FF;
const uint16_t MASK_0F00 = 0x0F00;
const uint16_t MASK_0FF0 = 0x0FF0;
const uint16_t MASK_0FFF = 0x0FFF;
const uint16_t MASK_F000 = 0xF000;
const uint16_t MASK_FF00 = 0xFF00;

const uint16_t JMPS = 0x0000;
const uint16_t CLS  = 0x00E0;
const uint16_t RET  = 0x00EE;
const uint16_t MASK_JMP  = 0x1000;
const uint16_t JMP  = 0x1;
const uint16_t MASK_CALL = 0x2000;
const uint16_t CALL = 0x2;
const uint16_t MASK_SKEQ = 0x3000;
const uint16_t SKEQ = 0x3;
const uint16_t MASK_SKNE = 0x4000;
const uint16_t SKNE = 0x4;
// const SOME = 0x5000
// const SOME = 0x6000
// const SOME = 0x7000
// const SOME = 0x8000
// const SOME = 0x8001
// const SOME = 0x8002
// const SOME = 0x8003
// const SOME = 0x8004
// const SOME = 0x8005
// // const SOME = 0xSet Vx = Vx
// const SOME = 0x8006
// const SOME = 0x8007
// // const SOME = 0xSet Vx = Vy
// const SOME = 0x800E
// const SOME = 0x9000
// const SOME = 0xA000
const uint16_t MASK_LDI = 0xA000;
const uint16_t LDI = 0xA;
// const SOME = 0xB000
// const SOME = 0xC000
// const SOME = 0xD000
const uint16_t MASK_DRAW = 0xD000;
const uint16_t DRAW = 0xD;
// const SOME = 0xE09E
// const SOME = 0xE0A1
// const SOME = 0xF007
// const SOME = 0xF00A
// const SOME = 0xF015
// const SOME = 0xF018
// const SOME = 0xF01E
// const SOME = 0xF029
// const SOME = 0xF033
// const SOME = 0xF055
// const SOME = 0xF065
#endif //C_CONSTANTS_H
