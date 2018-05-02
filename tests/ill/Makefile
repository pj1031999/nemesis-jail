TOPDIR = $(realpath ../..)

CXX = g++
CXXFLAGS = -w

SRC = $(shell find -iname '*.cc' -type f)
DST = $(addprefix $(TOPDIR)/build/tests/ill/, $(SRC:.cc=))

all: prepare $(DST)

$(TOPDIR)/build/tests/ill/%:%.cc
	$(CXX) $(CXXFLAGS) -o $@ $<

prepare:
	@mkdir -p $(TOPDIR)/build/tests/ill

.PHONY: all prepare
