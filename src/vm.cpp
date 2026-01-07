#include "vm.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

VM::VM() : pc(0), verbose(false) {}

VM::~VM() {}

void VM::setVerbose(bool v) { verbose = v; }

void VM::load(const std::string &filename) {
  std::ifstream file(filename, std::ios::binary);
  if (!file.is_open())
    throw std::runtime_error("VM Load Error: Could not open file " + filename);

  std::vector<unsigned char> bytecode((std::istreambuf_iterator<char>(file)),
                                      std::istreambuf_iterator<char>());
  file.close();

  if (bytecode.empty()) {
    throw std::runtime_error(
        "VM Load Error: File is empty or could not be read.");
  }

  if (bytecode.size() > MEM_SIZE)
    throw std::runtime_error(
        "VM Load Error: Bytecode size exceeds memory capacity.");

  long val_array[bytecode.size()];
  for (size_t i = 0; i < bytecode.size(); ++i) {
    val_array[i] = static_cast<long>(bytecode[i]);
  }
  ram.load(val_array, bytecode.size());
  pc = 0;
  std::cout << "Loaded " << bytecode.size() << " bytes from " << filename
            << std::endl;
}

