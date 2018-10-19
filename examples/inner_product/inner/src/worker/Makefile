INCLUDES := $(INCLUDES) -I. -I.. -I/usr/include -I$(CUDA_HOME)/include -I/home/scsjd/.local/include

CXX := nvcc -g
CPPFLAGS := $(CPPFLAGS) $(INCLUDES)
CXXFLAGS := $(CFLAGS) -std=c++11

LIB = ../../lib

SRCS = HE1Array.cpp HE1CiphertextCPU.cpp InnerMapTask.cpp InnerMapTask_GPU.cpp
OBJS = HE1Array.o HE1CiphertextCPU.o InnerMapTask.o InnerMapTask_GPU.o

NAME = worker
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
	$(CXX) -DCOMPSS_WORKER -c $(CFLAGS) $(CXXFLAGS) $(CPPFLAGS) -o $@ $<

clean:
	rm -fr $(OBJS) $(LIBS)
