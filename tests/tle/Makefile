TOPDIR = $(realpath ../..)

CXX = g++
CXXFLAGS = -w

SRC = $(shell find -iname '*.cc' -type f)
DST = $(addprefix $(TOPDIR)/build/tests/tle/, $(SRC:.cc=))

all: prepare $(DST)

$(TOPDIR)/build/tests/tle/%:%.cc
	$(CXX) $(CXXFLAGS) -o $@ $<

prepare:
	@mkdir -p $(TOPDIR)/build/tests/tle

.PHONY: all prepare
