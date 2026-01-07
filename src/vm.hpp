#ifndef VM_H
#define VM_H

#include "memory.hpp"
#include "stack.hpp"
#include <string>

class VM {
public:
  VM();
  ~VM();

  Stack call_stack;
  Stack register_stack;
  Memory program_memory;
  Memory data_memory;
  unsigned long pc;
  bool verbose;

  void load(const std::string &filename);
  void run();
  void setVerbose(bool v);

private:
};

#endif // !VM_H
