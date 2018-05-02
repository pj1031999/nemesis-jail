TOPDIR = $(realpath ../..)

CXX = g++
CXXFLAGS = -w

SRC = $(shell find -iname '*.cc' -type f)
DST = $(addprefix $(TOPDIR)/build/tests/, $(SRC:.cc=))

all: prepare $(DST)

$(TOPDIR)/build/tests/%:%.cc
	$(CXX) $(CXXFLAGS) -o $@ $<

prepare:
	@mkdir -p $(TOPDIR)/build/tests

.PHONY: all prepare
