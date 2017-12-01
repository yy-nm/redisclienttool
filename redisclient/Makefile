
CXX = g++ -std=c++11
#CC ?= gcc -std=c11
SHARED := -fPIC --shared
LIB ?= libs
OBJ ?= obj
CFLAGS = -O2 -Wall
AR = ar rcs
RM = rm -rf

CFILENAME = command commandhelper responseparser

all: lib
.PHONY: test clean libdynamic libstatic lib

$(LIB):
	mkdir $(LIB)

$(OBJ):
	mkdir $(OBJ)

define CFILEOBJECT
  $(OBJ)/$(strip $(1)).o: $(1).cpp | $(OBJ)
	$(CXX) $(CFLAGS) -c -o $$@ $$<
endef


$(foreach v, $(CFILENAME), $(eval $(call CFILEOBJECT, $(v))))

lib: libdynamic libstatic


libdynamic: | $(LIB)
	$(CXX) $(SHARED) $(CFLAGS) -o $(LIB)/libredisclienttool.so $(foreach v, $(CFILENAME), $(v).cpp)

libstatic: $(foreach v, $(CFILENAME), $(OBJ)/$(v).o) | $(LIB) $(OBJ)
	$(AR) $(LIB)/libredisclienttool.a $(foreach v, $(CFILENAME), $(OBJ)/$(v).o)

test:
	$(CXX) $(CFLAGS) -o redisclient-test main.cpp $(foreach v, $(CFILENAME), $(v).cpp)

clean:
	$(RM) redisclient-test $(LIB)/* $(OBJ)/*
