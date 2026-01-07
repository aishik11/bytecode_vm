CXX = g++

CXXFLAGS = -Wall -std=c++17

SRCDIR = src
BUILDDIR = build
TESTDIR = test
TARGET = bvm

all: $(BUILDDIR)/$(TARGET)

$(BUILDDIR)/$(TARGET): $(SRCDIR)/main.cpp $(SRCDIR)/stack.cpp $(SRCDIR)/stack.hpp $(SRCDIR)/memory.cpp $(SRCDIR)/memory.hpp $(SRCDIR)/vm.cpp $(SRCDIR)/vm.hpp $(SRCDIR)/op_codes.cpp $(SRCDIR)/op_codes.hpp
	mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) $(SRCDIR)/main.cpp $(SRCDIR)/stack.cpp $(SRCDIR)/memory.cpp $(SRCDIR)/vm.cpp $(SRCDIR)/op_codes.cpp -o $@

test: test_stack test_memory test_opcodes test_vm

test_stack: $(TESTDIR)/test_stack.cpp $(SRCDIR)/stack.cpp $(SRCDIR)/stack.hpp
	mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) $(TESTDIR)/test_stack.cpp $(SRCDIR)/stack.cpp -o $(BUILDDIR)/test_stack
	$(BUILDDIR)/test_stack

test_memory: $(TESTDIR)/test_memory.cpp $(SRCDIR)/memory.cpp $(SRCDIR)/memory.hpp
	mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) $(TESTDIR)/test_memory.cpp $(SRCDIR)/memory.cpp -o $(BUILDDIR)/test_memory
	$(BUILDDIR)/test_memory

test_opcodes: $(TESTDIR)/test_opcodes.cpp $(SRCDIR)/op_codes.cpp $(SRCDIR)/op_codes.hpp
	mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) $(TESTDIR)/test_opcodes.cpp $(SRCDIR)/op_codes.cpp -o $(BUILDDIR)/test_opcodes
	$(BUILDDIR)/test_opcodes

test_vm: $(TESTDIR)/test_vm.cpp $(SRCDIR)/vm.cpp $(SRCDIR)/vm.hpp $(SRCDIR)/memory.cpp $(SRCDIR)/memory.hpp $(SRCDIR)/stack.cpp $(SRCDIR)/stack.hpp $(SRCDIR)/op_codes.cpp $(SRCDIR)/op_codes.hpp
	mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) $(TESTDIR)/test_vm.cpp $(SRCDIR)/vm.cpp $(SRCDIR)/memory.cpp $(SRCDIR)/stack.cpp $(SRCDIR)/op_codes.cpp -o $(BUILDDIR)/test_vm
	$(BUILDDIR)/test_vm

clean: clean_assembler
	rm -rf $(BUILDDIR)

clean_assembler:
	-$(MAKE) -C Assembler clean # Clean the assembler's generated files
	rm -f $(BUILDDIR)/assembler

assembler:
	mkdir -p $(BUILDDIR)
	$(MAKE) -C Assembler
	cp Assembler/bin/assembler $(BUILDDIR)/assembler

.PHONY: all test clean assembler clean_assembler pipeline_test

pipeline_test: all assembler
	cd pipeline_tests && ./run_pipeline_tests.sh


