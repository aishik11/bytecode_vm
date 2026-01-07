CXX = g++

CXXFLAGS = -Wall -std=c++17

SRCDIR = src
BUILDDIR = build
TESTDIR = test
TARGET = bvm

all: $(BUILDDIR)/$(TARGET)

$(BUILDDIR)/$(TARGET): $(SRCDIR)/main.cpp
	mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) $< -o $@

test:
	# Add commands to compile and run your tests here

clean:
	rm -rf $(BUILDDIR)

.PHONY: all test clean
