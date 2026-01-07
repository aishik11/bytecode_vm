#ifndef STACK_H

#define STACK_H
#define STACK_SIZE 1024 * 10
typedef unsigned long size_t;

class Stack {
public:
  Stack() = default;
  ~Stack() = default;
  void push(long val);
  long pop();

private:
  long mem[STACK_SIZE];
  size_t ind;
  bool is_empty();
  bool is_full();
};

#endif // !STACK_H
