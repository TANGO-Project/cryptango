INCLUDES := $(INCLUDES) -Iinclude -I$(XMP_HOME)/src/include -I$(CUDA_HOME)/include
LDLIBS := $(LDLIBS) -lcryptopp -lntl -lboost_iostreams -lboost_serialization -lgmp -lgf2x -lcudart -lxmp
LDFLAGS := $(LDFLAGS) -L/usr/lib -L/usr/lib/x86_64-linux-gnu -L$(CUDA_HOME)/lib64 -L$(XMP_HOME)

CXX := g++
CPPFLAGS := $(CPPFLAGS) $(INCLUDES) -D__ISAAC64
CFLAGS := $(CFLAGS) -Wall -g -pipe -O2 -fPIC
CXXFLAGS := $(CFLAGS) -std=gnu++11
#CXXFLAGS := $(CFLAGS) -std=c++11 -stdlib=libc++ 

BIN := lib
OBJ := obj
SRC := src

NAME := cryptango

DLIB := lib$(NAME).so
SLIB := lib$(NAME).a

LIBS := $(BIN)/$(SLIB) $(BIN)/$(DLIB)

SRCS := $(shell find src -type f \( -name '*.cpp' \) )
		
OBJS := $(patsubst $(SRC)/%.cpp,$(OBJ)/%.o,$(SRCS))

all: $(LIBS)

define cc-command
@mkdir -p $(dir $@); 
@echo Compile $<; $(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<
endef

$(OBJS):$(OBJ)/%.o:	$(SRC)/%.cpp | print-opts
	$(cc-command)

%.a:
	@echo Archive $@
	@mkdir -p $(dir $@)
	@rm -f $@
	@ar rcs $@ $^
	
$(BIN)/$(SLIB): $(OBJS) Makefile

$(BIN)/$(DLIB): $(OBJS) Makefile
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -shared $(LDLIBS) $(LDFLAGS) $(OBJS) -o $@ 

clean:
	rm -rf $(OBJ) $(LIBS)

print-opts:
	@echo Building with \"$(CXX) $(CPPFLAGS) $(CXXFLAGS)\"
	
.PHONY: all mains clean print-opts
