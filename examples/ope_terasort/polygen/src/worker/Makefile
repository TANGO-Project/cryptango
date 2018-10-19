INCLUDES := $(INCLUDES) -I. -I.. -I/home/scsjd/.local/incldue -I/usr/include

CXX := g++ -g
CPPFLAGS := $(CPPFLAGS) $(INCLUDES)
CXXFLAGS := $(CFLAGS) -std=c++11

LIB = ../../lib

SRCS = PolyACDEncrypter.cpp PolygenMapTask.cpp
OBJS = PolyACDEncrypter.o PolygenMapTask.o

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
