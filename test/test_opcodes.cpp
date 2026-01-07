#include "../src/op_codes.hpp"
#include <cassert>
#include <iostream>
#include <vector>
#include <map>

void test_byteToOpcode() {
  // Test valid opcodes
  assert(byteToOpcode(0x00) == NOP && "byteToOpcode NOP failed");
  assert(byteToOpcode(0x01) == PUSH && "byteToOpcode PUSH failed");
  assert(byteToOpcode(0x10) == ADD && "byteToOpcode ADD failed");
  assert(byteToOpcode(0x20) == JMP && "byteToOpcode JMP failed");
  assert(byteToOpcode(0x30) == STORE && "byteToOpcode STORE failed");
  assert(byteToOpcode(0xFF) == HALT && "byteToOpcode HALT failed");

  // Test an unknown opcode (should throw an exception)
  bool caught_exception = false;
  try {
    byteToOpcode(0x05); // An undefined opcode
  } catch (const std::runtime_error& e) {
    caught_exception = true;
  }
  assert(caught_exception && "byteToOpcode did not throw for unknown opcode");
  
  std::cout << "test_byteToOpcode passed" << std::endl;
}

void test_opcodeToString() {
  // Test known opcodes
  assert(opcodeToString(NOP) == "NOP" && "opcodeToString NOP failed");
  assert(opcodeToString(PUSH) == "PUSH" && "opcodeToString PUSH failed");
  assert(opcodeToString(ADD) == "ADD" && "opcodeToString ADD failed");
  assert(opcodeToString(JMP) == "JMP" && "opcodeToString JMP failed");
  assert(opcodeToString(STORE) == "STORE" && "opcodeToString STORE failed");
  assert(opcodeToString(HALT) == "HALT" && "opcodeToString HALT failed");

  std::cout << "test_opcodeToString passed" << std::endl;
}

int main() {
  test_byteToOpcode();
  test_opcodeToString();
  return 0;
}
