INCLUDES := $(INCLUDES) -I.. -I/usr/include -I/home/scsjd/.local/include

CXX := g++ -g
CPPFLAGS := $(CPPFLAGS) $(INCLUDES)
CXXFLAGS := $(CFLAGS) -std=gnu++11

LIB = ../../lib

SRCS = PolyCiphertext.cpp PartitionSampler.cpp
OBJS = PolyCiphertext.o PartitionSampler.o

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
