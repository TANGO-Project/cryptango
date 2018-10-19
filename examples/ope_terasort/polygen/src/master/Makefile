INCLUDES := $(INCLUDES) -I. -I.. -I/home/scsjd/.local/include -I/usr/include 


CXX := g++ -g
CPPFLAGS := $(CPPFLAGS) $(INCLUDES)
CXXFLAGS := $(CFLAGS) -std=c++11

LIB = ../../lib

SRCS = PolyACDEncrypter.cpp
OBJS = PolyACDEncrypter.o

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
