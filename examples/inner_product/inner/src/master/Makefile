INCLUDES := $(INCLUDES) -I. -I.. -I/usr/include -I/usr/local/cuda/include -I/home/scsjd/.local/include

CXX := nvcc -g
CPPFLAGS := $(CPPFLAGS) $(INCLUDES)
CXXFLAGS := $(CFLAGS) -std=c++11

LIB = ../../lib

SRCS = HE1CiphertextCPU.cpp
OBJS = HE1CiphertextCPU.o

NAME = master
SLIB = lib$(NAME).a

LIBS = $(LIB)/$(SLIB)

all:	$(LIBS)

$(LIB)/$(SLIB): $(OBJS)

%.a:
	@echo Archive $@
	@mkdir -p $(dir $@)
	@rm -f $@
	@ar rvs $@ $^

$(OBJS):%.o:	%.cpp
	$(CXX) -DCOMPSS_MASTER -c $(CFLAGS) $(CXXFLAGS) $(CPPFLAGS) -o $@ $<

clean:
	rm -fr $(OBJS) $(LIBS)
