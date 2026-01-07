CXX = g++

CXXFLAGS = -Wall -std=c++17

SRCDIR = src
BUILDDIR = build
TESTDIR = test
TARGET = bvm

all: $(BUILDDIR)/$(TARGET)

$(BUILDDIR)/$(TARGET): $(SRCDIR)/main.cpp $(SRCDIR)/stack.cpp $(SRCDIR)/stack.hpp
	mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) $(SRCDIR)/main.cpp $(SRCDIR)/stack.cpp -o $@

test: test_stack

test_stack: $(TESTDIR)/test_stack.cpp $(SRCDIR)/stack.cpp $(SRCDIR)/stack.hpp
	mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) $(TESTDIR)/test_stack.cpp $(SRCDIR)/stack.cpp -o $(BUILDDIR)/test_stack
	$(BUILDDIR)/test_stack

clean:
	rm -rf $(BUILDDIR)

.PHONY: all test clean
