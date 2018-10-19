INCLUDES := $(INCLUDES) -I.. -I/usr/include -I/home/scsjd/.local/include

CXX := g++ -g
CPPFLAGS := $(CPPFLAGS) $(INCLUDES)
CXXFLAGS := $(CFLAGS) -std=gnu++11

LIB = ../../lib

SRCS = OPEMapTask.cpp OPEReduceTask.cpp PolyCiphertext.cpp
OBJS = OPEMapTask.o OPEReduceTask.o PolyCiphertext.o

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
