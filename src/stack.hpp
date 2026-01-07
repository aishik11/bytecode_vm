#ifndef STACK_H

#include <vector>

#define STACK_H
#define STACK_SIZE 1024 * 10

class Stack {
public:
  Stack();
  ~Stack() = default;
  void push(long val);
  long pop();
  long peek();
  void dup();
  bool is_empty();
  bool is_full();
  std::vector<long> getElements() const;

private:
  long mem[STACK_SIZE];
  unsigned long ind = 0;
};

#endif // !STACK_H
