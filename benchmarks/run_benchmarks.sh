#!/bin/bash

# Exit immediately if a command exits with a non-zero status.
set -e

# Assemble all benchmark files
echo "Assembling benchmarks..."
for bench_file in *.asm; do
    ../build/assembler "$bench_file" "${bench_file%.asm}.bin"
done

# Run benchmarks
echo ""
echo "Running benchmarks..."
echo "-------------------"

for bench_file in *.bin; do
    echo "Benchmark: $bench_file"
    time ../build/bvm "$bench_file"
    echo "-------------------"
done

# Clean up
# rm -f *.bin
