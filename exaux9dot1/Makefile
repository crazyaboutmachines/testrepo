TEMPLATEROOT = ../../template/STM32-Template

# compilation flags for gdb

CFLAGS  = -O0 -g
ASFLAGS = -g 
LDLIBS += -lm
LDFLAGS+= -specs=nosys.specs

# object files

OBJS=  $(STARTUP) main.o
OBJS+= stm32f10x_gpio.o stm32f10x_rcc.o stm32f10x_usart.o uart.o
OBJS+= stm32f10x_i2c.o i2c.o

# include common make file

include $(TEMPLATEROOT)/Makefile.common


