MAKE = make

CXX = g++
CXXFLAGS = -std=c++14 -O3 -Wall -Wextra -Werror -fno-exceptions

INCLUDE = $(CURDIR)/src/include
OBJ = build/process.o build/timer.o


all: $(OBJ)
	$(MAKE) -C $(CURDIR)/tests

build/process.o: prepare src/process.cc
	$(CXX) $(CXXFLAGS) -I$(INCLUDE) -c -o build/process.o src/process.cc

build/timer.o: prepare src/timer.cc
	$(CXX) $(CXXFLAGS) -I$(INCLUDE) -c -o build/timer.o src/timer.cc

prepare:
	@mkdir -p build

clean:
	@rm -rf build/*.o build/tests

distclean:
	@rm -rf build

test:
	$(MAKE) test -C $(CURDIR)/tests

.PHONY: all prepare clean distclean test
