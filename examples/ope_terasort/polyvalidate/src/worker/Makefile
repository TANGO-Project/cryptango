INCLUDES := $(INCLUDES) -I. -I.. -I/usr/include -I/usr/include/NTL -I/usr/include/jsoncpp
LDLIBS := $(LDLIBS) -lntl -lgmp
LDFLAGS := $(LDFLAGS) -L/usr/lib -L/usr/lib/x86_64-linux-gnu

CXX := g++ -g
CPPFLAGS := $(CPPFLAGS) $(INCLUDES)
CXXFLAGS := $(CFLAGS) -std=gnu++11

LIB = ../../lib

SRCS = PolyACDDecrypter.cpp PolyCiphertext.cpp ValidateMapTask.cpp
OBJS = PolyACDDecrypter.o PolyCiphertext.o ValidateMapTask.o

NAME = worker
DLIB = lib$(NAME).so
SLIB = lib$(NAME).a

#LIBS = $(LIB)/$(DLIB) $(LIB)/$(SLIB)
LIBS = $(LIB)/$(SLIB)

all:	$(LIBS)

$(LIB)/$(SLIB): $(OBJS)

$(LIB)/$(DLIB): $(OBJS)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -shared $(LDLIBS) $(LDFLAGS) $(MOBJS) -o $@ 

%.a:
	@echo Archive $@
	@mkdir -p $(dir $@)
	@rm -f $@
	@ar rvs $@ $^

$(OBJS):%.o:	%.cpp
	$(CXX) -DCOMPSS_WORKER -c $(CFLAGS) $(CXXFLAGS) $(CPPFLAGS) -o $@ $<

clean:
	rm -fr $(OBJS) $(LIBS)
