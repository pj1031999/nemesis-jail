TOPDIR = $(realpath ..)

CXX = g++
CXXFLAGS = -std=c++14 -O3 -Wall -Wextra

LDFLAGS = -pthread -lseccomp

INCLUDE = $(TOPDIR)/src/include

all: $(TOPDIR)/build/handler 
$(TOPDIR)/build/handler: $(TOPDIR)/build/handler.o $(TOPDIR)/build/process.o $(TOPDIR)/build/timer.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -I$(INCLUDE) -o $(TOPDIR)/build/handler $(TOPDIR)/build/handler.o $(TOPDIR)/build/process.o $(TOPDIR)/build/timer.o

$(TOPDIR)/build/handler_check: $(TOPDIR)/build/handler_check.o $(TOPDIR)/build/process.o $(TOPDIR)/build/timer.o
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -I$(INCLUDE) -o $(TOPDIR)/build/handler_check $(TOPDIR)/build/handler_check.o $(TOPDIR)/build/process.o $(TOPDIR)/build/timer.o

$(TOPDIR)/build/handler.o:
	$(CXX) $(CXXFLAGS) -I$(INCLUDE) -c -o $(TOPDIR)/build/handler.o $(CURDIR)/handler.cc

$(TOPDIR)/build/handler_check.o:
	$(CXX) $(CXXFLAGS) -I$(INCLUDE) -c -o $(TOPDIR)/build/handler_check.o $(CURDIR)/handler_check.cc

test: $(TOPDIR)/build/handler_check
	$(MAKE) -C $(CURDIR)/return_state
	$(MAKE) -C $(CURDIR)/tle
	$(MAKE) -C $(CURDIR)/ill
	@sudo ./test.py


.PHONY: all test
