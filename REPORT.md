# Technical Report: Bytecode Virtual Machine

## 1. VM Architecture

The virtual machine is designed as a stack-based architecture. This means that all arithmetic and logical operations retrieve their operands from a main `register_stack` and push their results back onto it. This simplifies the instruction format as operands do not need to be specified explicitly.

### Components
- **Program Counter (PC):** An `unsigned long` that acts as an index into the `program_memory`, pointing to the next instruction to be executed.
- **Program Memory:** A `long` array that stores the bytecode loaded from a `.bin` file. Each instruction and operand occupies one `long` slot.
- **Register Stack:** A LIFO stack implemented as a `long` array. It is used for all operations, including arithmetic, function arguments, and local variables.
- **Call Stack:** A separate stack to store return addresses for function calls, enabling nested function calls.
- **Data Memory:** A general-purpose `long` array for storing and loading values using the `STORE` and `LOAD` instructions.

### Instruction Dispatch
The VM uses a standard `while` loop that runs as long as the `HALT` instruction has not been executed. Inside the loop, it fetches the next `long` from `program_memory` at the `pc` address. This `long` is cast to an `Opcode` enum, and a `switch` statement dispatches to the correct logic for that instruction. This is a simple and direct dispatch method.

## 2. Assembler Design

The assembler is a crucial part of the toolchain, converting human-readable assembly language into the `long`-based bytecode that the VM executes. It is implemented using Flex (for lexical analysis) and Bison (for parsing).

### Two-Pass Design
To handle forward references (i.e., using a label before it is defined), the assembler is designed as a **two-pass assembler**:

1.  **First Pass:** The parser runs through the entire source file. Its only goal is to find all label definitions (e.g., `my_label:`). It stores each label and its address (the value of the program counter `pc` at that point) in a symbol table. Instructions that use labels as operands (like `JMP`, `JZ`, `JNZ`, `CALL`) are given a placeholder operand of `0`.
2.  **Second Pass:** The file pointer and line counter are reset, and the parser runs again. This time, when it encounters an instruction that uses a label, it looks up the label in the symbol table to get its address. It then emits the correct `long` opcode and `long` address into the bytecode buffer.

This design allows for flexible use of labels for control flow without imposing declaration-before-use restrictions.

### Grammar
The grammar is defined in `parser.y` and is line-oriented. It recognizes instructions (e.g., `PUSH 10`), label definitions (`loop_start:`), comments, and newlines. The grammar rules ensure that opcodes and operands are correctly parsed and passed to the `emit_long` function, which writes them to the bytecode buffer.

## 3. Call/Return Mechanism

The VM supports basic function calls and returns using the `CALL` and `RET` instructions.

-   **`CALL addr`:** Pushes the current program counter (`pc`, which points to the instruction *after* the `CALL`) onto the `call_stack` and then unconditionally jumps to the specified `addr`.
-   **`RET`:** Pops the return address from the `call_stack` back into the `pc`, resuming execution where the caller left off.

### Calling Convention
We opted for a simple, manual "callee-cleans" calling convention.

-   **Caller:** Pushes arguments onto the register stack before making a `CALL`.
-   **Callee:** Is responsible for its own stack management. It must pop its arguments and any local variables it used, and then push its single return value (if any) before executing `RET`.

This approach avoids the complexity of implementing full stack frames with a frame pointer in the VM, trading automatic stack management for programmer diligence. It is a valid and efficient strategy for simpler VMs.

## 4. Limitations & Possible Enhancements

-   **Error Handling:** The assembler and VM have basic error handling (e.g., "Label not found", "Stack Underflow"), but it could be made more robust with more specific error messages and recovery mechanisms.
-   **Strict `long`-based format:** The current architecture uses `long`s for everything, including opcodes that only need a single byte. This makes the bytecode files larger than necessary. A more compact format could be used, where the VM reads operands of different sizes (1, 4, or 8 bytes) depending on the opcode.
-   **No Local Scopes:** Without true call frames, there is no concept of local scopes. All values are on the global `register_stack` or in the global `data_memory`, which can make writing complex, modular code more difficult.
-   **Assembler Syntax:** The assembler is quite strict (e.g., requiring labels on their own lines). It could be enhanced to be more flexible.

## 5. Testing Strategy

The project employs a multi-layered testing strategy to ensure correctness and robustness.

-   **C++ Unit Tests (`make test`):** These tests, located in the `test/` directory, verify the core functionality of individual components like `Stack`, `Memory`, and `Opcode` handling in isolation.
-   **Assembler Test Suite (`make -C Assembler test`):** The assembler has its own dedicated test suite. It assembles a series of `.asm` files and performs a binary `diff` against `.bin.expected` files to guarantee that the generated bytecode is exactly correct.
-   **End-to-End Pipeline Tests (`make pipeline_test`):** This is the most comprehensive suite. It runs a series of assembly programs from the `pipeline_tests/` directory through the entire toolchain: assembly and then execution on the VM. The script automatically verifies the final state of the stack against expected values, confirming that the VM's instruction implementations are correct.
-   **Benchmarks (`make benchmark`):** A suite of performance tests to measure the VM's execution speed on various tasks.

## 6. Benchmarks / Performance Analysis

To evaluate the performance of the VM, a comprehensive suite of benchmarks was created, testing basic operations at scale and recursive algorithms.

### 6.1. Basic Operations Performance

We measured the execution time of common operations (`ADD`, `MUL`, `PUSH/POP`, `LOAD/STORE`) in loops ranging from 10 to 1,000,000 iterations. The results represent the **median of 5 runs**.

| Operations (n) | ADD (ms) | MUL (ms) | PUSH/POP (ms) | LOAD/STORE (ms) |
|----------------|----------|----------|---------------|-----------------|
| 10             | 3.02     | 3.28     | 2.85          | 3.34            |
| 100            | 2.91     | 3.41     | 2.97          | 2.71            |
| 1,000          | 2.70     | 2.90     | 2.66          | 2.83            |
| 10,000         | 3.95     | 4.21     | 3.36          | 4.36            |
| 100,000        | 14.38    | 16.73    | 12.26         | 15.86           |
| 1,000,000      | 107.31   | 105.78   | 81.80         | 117.55          |

**Note on Scaling:** The log-log scale is used below to visualize performance over five orders of magnitude. The linear scaling $O(n)$ is represented by the near-constant slope of 1 for $n \ge 10,000$.

![Operations Comparison](benchmarks/plotting/ops_comparison.png)

### 6.2. Recursive Algorithms Performance

We also tested the VM's performance with recursive implementations of Fibonacci and Factorial (median of 5 runs).

| n (Fib) | Fibonacci (ms) | n (Fact) | Factorial (ms) |
|---------|----------------|----------|----------------|
| 10      | 2.70           | 10       | 2.86           |
| 20      | 5.76           | 20       | 2.85           |
| 24      | 23.77          | 24       | 2.95           |
| 28      | 147.70         | 28       | 3.36           |
| 32      | 988.61         | 32       | 2.94           |
| -       | -              | 40       | 2.77           |

#### Fibonacci Performance
The semi-log plot clearly shows the exponential growth characteristic of the recursive Fibonacci algorithm.

![Fibonacci Performance](benchmarks/plotting/fib_performance.png)

#### Factorial Performance
The linear scaling of the recursive factorial is evident, remaining nearly constant for small values of $n$ due to minimal overhead compared to Fibonacci.

![Factorial Performance](benchmarks/plotting/fact_performance.png)

### Analysis

- **Operation Scaling:** The VM shows linear scaling with the number of operations, as expected. Dispatch overhead is consistent across different instruction types.
- **Recursion Overhead:** The recursive Fibonacci benchmark highlights the exponential growth in function calls. The jump from `n=20` to `n=25` shows a significant increase in execution time, demonstrating the overhead of `CALL` and `RET` operations at scale.
- **Factorial:** Recursive factorial, being `O(n)`, remains extremely fast even for `n=25`, as it only involves 25 function calls.

