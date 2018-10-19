RM := rm -rf
LIBS := -lgmp -lgf2x -lntl -ljsoncpp -lcryptmr
CPP_SRCS := main.cpp 
OBJS := main.o 

all: innervalidate

# Tool invocations
innervalidate: $(OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	g++ -L/usr/lib -L/usr/lib/x86_64-linux-gnu -L/home/scsjd/.local/lib -o innervalidate $(OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Each subdirectory must supply rules for building sources it contributes
%.o: %.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I$(HOME)/.local/include -O0 -g3 -Wall -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS) innervalidate
	-@echo ' '

.PHONY: all clean