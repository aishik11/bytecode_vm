#ifndef STACK_H

#define STACK_H
#define STACK_SIZE 1024 * 10

class Stack {
public:
  Stack();
  ~Stack() = default;
  void push(long val);
  long pop();
  bool is_empty();
  bool is_full();

private:
  long mem[STACK_SIZE];
  unsigned long ind = 0;
};

#endif // !STACK_H
