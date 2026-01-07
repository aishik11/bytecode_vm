#ifndef MEMORY_H
#define MEMORY_H

#define MEM_SIZE 1024 * 20

class Memory {
public:
  Memory();
  ~Memory();
  void load(long array[], long size);
  void reset();
  void store(long val);
  long get(unsigned long address);

private:
  long mem[MEM_SIZE];
};

#endif // !MEMORY_H
