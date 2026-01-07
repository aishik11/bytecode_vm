#include "stack.hpp"
#include <stdexcept>

void Stack::push(long val) {
  if (is_full())
    throw std::runtime_error("Stack Overflow");
  mem[ind++] = val;
}

long Stack::pop() {
  if (is_empty())
    throw std::runtime_error("Stack Underflow");
  return mem[ind--];
}

bool Stack::is_full() { return ind == STACK_SIZE - 1; }

bool Stack::is_empty() { return ind == 0; }

#ifdef TEST_STACK

int main() { return 0; }

#endif // TEST_STACK
