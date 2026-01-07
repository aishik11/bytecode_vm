#ifndef OPCODE_H
#define OPCODE_H

#include <string>

typedef enum Opcode {
  // Blank
  NOP = 0x00,
  // Data
  PUSH,
  POP,
  DUP,
  // Arithmetic
  ADD = 0x10,
  SUB,
  MUL,
  DIV,
  CMP,
  // Bitwise
  AND = 0x15,
  OR,
  XOR,
  NOT,
  SHL,
  SHR,
  // Control
  JMP = 0x20,
  JZ,
  JNZ,
  // Memory
  STORE = 0x30,
  LOAD,
  CALL,
  RET,
  // Halt
  HALT = 0xFF,
} Opcode;

Opcode byteToOpcode(unsigned char byte);
std::string opcodeToString(Opcode opcode);

#endif // !OPCODE_H
