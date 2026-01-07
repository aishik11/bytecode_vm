#ifndef VM_H
#define VM_H

#include "memory.hpp"
#include "op_codes.hpp"
#include "stack.hpp"

class VM {
public:
  VM();
  ~VM();

private:
  Stack call_stack;
  Stack register_stack;
  Memory ram;
};

#endif // !VM_H
