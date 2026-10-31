CXX ?= g++
CXXFLAGS ?= -std=c++20 -O2 -Wall -Wextra -Wpedantic
.PHONY: test demo clean tools
rtl-witness: main.cpp contract.cpp contract.hpp
	$(CXX) $(CXXFLAGS) main.cpp contract.cpp -o $@
witness-tests: tests.cpp contract.cpp contract.hpp
	$(CXX) $(CXXFLAGS) tests.cpp contract.cpp -o $@
test: witness-tests
	./witness-tests
demo: rtl-witness
	./rtl-witness
tools:
	@for tool in yosys verilator iverilog; do command -v $$tool || true; done
clean:
	rm -f rtl-witness witness-tests
