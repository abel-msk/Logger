
СХХ=g++
CXXFLAGS= -g -std=c++11  -D__DEBUG_X64_GCC___
#CXXFLAGS= -g 

BUILDDIR=build
LDFLAGS=-lm -lstdc++
#LDFLAGS=-lm 



SRC := $(shell ls -1 ./*.cpp)
OBJ=$(addprefix $(BUILDDIR)/,$(notdir $(SRC:%.cpp=%.o)))
INCLUDE_EX+= -I./ 

# 
#  Replace path with home directory for libraray BinTreeStore
#  https://github.com/abel-msk/BinTreeStore.git
#
BINTREE_LIB=../../BinTreeStore

# BINTREE_SRC=$(shell ls -1 $(BINTREE_LIB)/src/*.cpp)
# BINTREE_OBJ=$(addprefix $(BUILDDIR)/,$(notdir $(BINTREE_SRC:%.cpp=%.o)))
INCLUDE_EX+= -I $(BINTREE_LIB)/src

LIB_DIR = ../src
LIB_SRC := $(shell ls -1 $(LIB_DIR)/*.cpp)
LIB_OBJ=$(addprefix $(BUILDDIR)/,$(notdir $(LIB_SRC:%.cpp=%.o)))
INCLUDE_EX+= -I ../src/

CXXFLAGS+=$(INCLUDE_EX)

$(BUILDDIR)/%.o: $(BINTREE_LIB)/%.cpp
	$(CXX)   $(CXXFLAGS) -I$(dir $<) -c $<  -o $@

$(BUILDDIR)/%.o: $(LIB_DIR)/%.cpp
	$(CXX)   $(CXXFLAGS) -I$(dir $<) -c $<  -o $@

$(BUILDDIR)/%.o: ./%.cpp
	$(CXX)   $(CXXFLAGS) -I$(dir $<) -c $<  -o $@	
#

all:  $(BUILDDIR) $(LIB_OBJ) $(OBJ) 
	$(СХХ) $(LDFLAGS)  $(INCLUDE_EX) $(LDLIB) -o test $(OBJ)  $(LIB_OBJ)

clean: 
	rm -f $(BUILDDIR)/*.o
	rm -f ./*.o
	rm ./test

$(BUILDDIR):
	mkdir $(BUILDDIR)
