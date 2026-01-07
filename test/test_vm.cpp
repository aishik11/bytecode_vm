#include "../src/vm.hpp"
#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>

// Helper function to create a bytecode file
void create_bytecode_file(const std::string &filename,
                          const std::vector<unsigned char> &bytecode) {
  std::ofstream file(filename, std::ios::binary);
  if (!file.is_open()) {
    throw std::runtime_error("Failed to create bytecode file: " + filename);
  }
  for (unsigned char byte : bytecode) {
    file.put(static_cast<char>(byte));
  }
  file.close();
}

void test_vm_push_add_halt() {
  std::cout << "Running test_vm_push_add_halt..." << std::endl;
  std::string test_file = "test_push_add_halt.bin";
  create_bytecode_file(test_file, {0x01, 0x05, 0x01, 0x03, 0x10, 0xFF}); // PUSH 5, PUSH 3, ADD, HALT

  VM vm;
  vm.setVerbose(false); // Disable verbose for tests unless specifically needed
  vm.load(test_file);
  vm.run();

  assert(vm.register_stack.pop() == 8 && "PUSH 5, PUSH 3, ADD did not result in 8");
  assert(vm.register_stack.is_empty() && "Register stack not empty after test");
  std::cout << "test_vm_push_add_halt passed" << std::endl;
}

void test_vm_store_load_halt() {
  std::cout << "Running test_vm_store_load_halt..." << std::endl;
  std::string test_file = "test_store_load_halt.bin";
  // PUSH 10, STORE 0, LOAD 0, HALT
  create_bytecode_file(test_file, {0x01, 0x0A, 0x30, 0x00, 0x31, 0x00, 0xFF}); 

  VM vm;
  vm.setVerbose(false);
  vm.load(test_file);
  vm.run();

  assert(vm.register_stack.pop() == 10 && "STORE 10 at 0, LOAD from 0 did not result in 10");
  assert(vm.register_stack.is_empty() && "Register stack not empty after test");
  std::cout << "test_vm_store_load_halt passed" << std::endl;
}


int main() {
  test_vm_push_add_halt();
  test_vm_store_load_halt();
  return 0;
}
