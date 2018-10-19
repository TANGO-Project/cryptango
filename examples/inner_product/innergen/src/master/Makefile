INCLUDES := $(INCLUDES) -I.. -I/usr/include -I$(HOME)/.local/include
LDFLAGS := $(LDFLAGS) -L$(HOME)/.local/lib -L/usr/lib -L/usr/lib/x86_64-linux-gnu

CXX := g++ -g
CPPFLAGS := $(CPPFLAGS) $(INCLUDES)
CXXFLAGS := $(CFLAGS) -std=gnu++11

LIB = ../../lib

SRCS = 
OBJS = 

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
