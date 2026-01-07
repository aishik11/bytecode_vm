#include "stack.hpp"
#include <stdexcept>

Stack::Stack() : ind(0) {}

void Stack::push(long val) {
  if (is_full())
    throw std::runtime_error("Stack Overflow");
  mem[ind++] = val;
}

long Stack::pop() {
  if (is_empty())
    throw std::runtime_error("Stack Underflow");
  return mem[--ind];
}

bool Stack::is_full() { return ind == STACK_SIZE; }

bool Stack::is_empty() { return ind == 0; }

void Stack::dup() {
  if (is_empty())
    throw std::runtime_error("Stack Underflow: Cannot duplicate from an empty stack.");
  long val = pop();
  push(val);
  push(val);
}

std::vector<long> Stack::getElements() const {
    std::vector<long> elements;
    for (unsigned long i = 0; i < ind; ++i) {
        elements.push_back(mem[i]);
    }
    return elements;
}
