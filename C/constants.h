//
// Created by Ross Wilson on 09/07/2023.
//

#ifndef C_ERRORS_H
#define C_ERRORS_H

const int NUM_REGISTERS = 16;
const int SIZE_STACK = 64;
const int SIZE_RAM = 4096;
const char FILE_NOT_FOUND[] = "Could not load the specified ROM file!\n Exiting.\n";

const int OK = 0;
const int NOT_OK = 1;

const int MEM_OFFSET = 0x200;

#endif //C_ERRORS_H
