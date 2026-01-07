#include "vm.hpp"
#include "op_codes.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

VM::VM() : pc(0), verbose(false) {}

VM::~VM() {}

void VM::setVerbose(bool v) { verbose = v; }

void VM::load(const std::string &filename) {
  FILE* file = fopen(filename.c_str(), "rb");
  if (!file) {
    throw std::runtime_error("VM Load Error: Could not open file " + filename);
  }

  fseek(file, 0, SEEK_END);
  long file_size = ftell(file);
  fseek(file, 0, SEEK_SET);

  if (file_size == 0) {
    fclose(file);
    throw std::runtime_error("VM Load Error: File is empty.");
  }
  if (file_size % sizeof(long) != 0) {
    fclose(file);
    throw std::runtime_error("VM Load Error: File size is not a multiple of sizeof(long).");
  }

  size_t num_longs = file_size / sizeof(long);
  if (num_longs > MEM_SIZE) {
    fclose(file);
    throw std::runtime_error("VM Load Error: Bytecode size exceeds memory capacity.");
  }

  long buffer[num_longs];
  fread(buffer, sizeof(long), num_longs, file);
  fclose(file);

  program_memory.load(buffer, num_longs);
  pc = 0;
  std::cout << "Loaded " << file_size << " bytes from " << filename << std::endl;
}

void VM::run() {
  if (verbose) {
    std::cout << "VM running in verbose mode..." << std::endl;
  } else {
    std::cout << "VM running..." << std::endl;
  }

  while (true) {
    if (pc >= MEM_SIZE) {
      throw std::runtime_error(
          "VM Runtime Error: Program Counter out of bounds.");
    }

    long instruction = program_memory.get(pc++);
    Opcode opcode = longToOpcode(instruction);

    if (verbose) {
      std::cout << "PC: " << pc - 1 << ", Opcode: " << opcodeToString(opcode);
    }

    long val1, val2, addr, idx, amt;
    switch (opcode) {
    case NOP:
      if (verbose)
        std::cout << " (NOP)" << std::endl;
      break;
    case PUSH:
      if (pc >= MEM_SIZE)
        throw std::runtime_error(
            "VM Runtime Error: PUSH operand out of bounds.");
      val1 = program_memory.get(pc++);
      register_stack.push(val1);
      if (verbose)
        std::cout << " " << val1 << " (PUSH " << val1 << ")" << std::endl;
      break;
    case POP:
      register_stack.pop();
      if (verbose)
        std::cout << " (POP)" << std::endl;
      break;
    case DUP:
      register_stack.dup();
      if (verbose)
        std::cout << " (DUP)" << std::endl;
      break;
    case ADD:
      val2 = register_stack.pop();
      val1 = register_stack.pop();
      register_stack.push(val1 + val2);
      if (verbose)
        std::cout << " (ADD " << val1 << ", " << val2 << ")" << std::endl;
      break;
    case SUB:
      val2 = register_stack.pop();
      val1 = register_stack.pop();
      register_stack.push(val1 - val2);
      if (verbose)
        std::cout << " (SUB " << val1 << ", " << val2 << ")" << std::endl;
      break;
    case MUL:
      val2 = register_stack.pop();
      val1 = register_stack.pop();
      register_stack.push(val1 * val2);
      if (verbose)
        std::cout << " (MUL " << val1 << ", " << val2 << ")" << std::endl;
      break;
    case DIV:
      val2 = register_stack.pop();
      val1 = register_stack.pop();
      if (val2 == 0) {
        throw std::runtime_error("VM Runtime Error: Division by zero.");
      }
      register_stack.push(val1 / val2);
      if (verbose)
        std::cout << " (DIV " << val1 << ", " << val2 << ")" << std::endl;
      break;
    case CMP:
      val2 = register_stack.pop();
      val1 = register_stack.pop();
      register_stack.push(val1 < val2 ? 1 : 0);
      if (verbose)
        std::cout << " (CMP " << val1 << ", " << val2 << ")" << std::endl;
      break;
    case AND:
      val2 = register_stack.pop();
      val1 = register_stack.pop();
      register_stack.push(val1 & val2);
      if (verbose)
        std::cout << " (AND " << val1 << ", " << val2 << ")" << std::endl;
      break;
    case OR:
      val2 = register_stack.pop();
      val1 = register_stack.pop();
      register_stack.push(val1 | val2);
      if (verbose)
        std::cout << " (OR " << val1 << ", " << val2 << ")" << std::endl;
      break;
    case XOR:
      val2 = register_stack.pop();
      val1 = register_stack.pop();
      register_stack.push(val1 ^ val2);
      if (verbose)
        std::cout << " (XOR " << val1 << ", " << val2 << ")" << std::endl;
      break;
    case NOT:
      val1 = register_stack.pop();
      register_stack.push(~val1);
      if (verbose)
        std::cout << " (NOT " << val1 << ")" << std::endl;
      break;
    case SHL:
      amt = register_stack.pop();
      val1 = register_stack.pop();
      register_stack.push(val1 << amt);
      if (verbose)
        std::cout << " (SHL " << val1 << ", " << amt << ")" << std::endl;
      break;
    case SHR:
      amt = register_stack.pop();
      val1 = register_stack.pop();
      register_stack.push(val1 >> amt);
      if (verbose)
        std::cout << " (SHR " << val1 << ", " << amt << ")" << std::endl;
      break;
    case JMP:
      if (pc >= MEM_SIZE)
        throw std::runtime_error(
            "VM Runtime Error: JMP address out of bounds.");
      addr = program_memory.get(pc++);
      pc = addr;
      if (verbose)
        std::cout << " " << addr << " (JMP to " << addr << ")" << std::endl;
      break;
    case JZ:
      if (pc >= MEM_SIZE)
        throw std::runtime_error("VM Runtime Error: JZ address out of bounds.");
      val1 = register_stack.pop();
      addr = program_memory.get(pc++);
      if (val1 == 0) {
        pc = addr;
      }
      if (verbose)
        std::cout << " " << addr << " (JZ to " << addr << " if " << val1
                  << " == 0)" << std::endl;
      break;
    case JNZ:
      if (pc >= MEM_SIZE)
        throw std::runtime_error(
            "VM Runtime Error: JNZ address out of bounds.");
      val1 = register_stack.pop();
      addr = program_memory.get(pc++);
      if (val1 != 0) {
        pc = addr;
      }
      if (verbose)
        std::cout << " " << addr << " (JNZ to " << addr << " if " << val1
                  << " != 0)" << std::endl;
      break;
    case STORE:
      if (pc >= MEM_SIZE)
        throw std::runtime_error(
            "VM Runtime Error: STORE index out of bounds.");
      val1 = register_stack.pop();
      idx = program_memory.get(pc++);
      data_memory.store(idx, val1);
      if (verbose)
        std::cout << " " << idx << " (STORE " << val1 << " at " << idx << ")"
                  << std::endl;
      break;
    case LOAD:
      if (pc >= MEM_SIZE)
        throw std::runtime_error("VM Runtime Error: LOAD index out of bounds.");
      idx = program_memory.get(pc++);
      register_stack.push(data_memory.get(idx));
      if (verbose)
        std::cout << " " << idx << " (LOAD from " << idx << ")" << std::endl;
      break;
    case CALL:
      if (pc >= MEM_SIZE)
        throw std::runtime_error(
            "VM Runtime Error: CALL address out of bounds.");
      addr = program_memory.get(pc++);
      call_stack.push(pc);
      pc = addr;
      if (verbose)
        std::cout << " " << addr << " (CALL " << addr << ")" << std::endl;
      break;
    case RET:
      pc = call_stack.pop();
      if (verbose)
        std::cout << " (RET to " << pc << ")" << std::endl;
      break;
    case HALT:
      if (verbose)
        std::cout << " (HALT)" << std::endl;
      return;
    default:
      throw std::runtime_error(
          "VM Runtime Error: Unimplemented or unknown opcode: " +
          opcodeToString(opcode));
    }
  }
}

void VM::printStack() {
  std::cout << "Stack (top to bottom):" << std::endl;
  std::vector<long> stack_elements = register_stack.getElements();
  for (int i = stack_elements.size() - 1; i >= 0; --i) {
    std::cout << "  " << stack_elements[i] << std::endl;
  }
  if (stack_elements.empty()) {
    std::cout << "  (empty)" << std::endl;
  }
}
