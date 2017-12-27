TEMPLATEROOT = ../../template/STM32-Template

# compilation flags for gdb

CFLAGS  = -O0 -g
ASFLAGS = -g 

# object files

OBJS=  $(STARTUP) main.o
OBJS+= stm32f10x_gpio.o stm32f10x_rcc.o
OBJS+= misc.o
OBJS+= stm32f10x_tim.o

# include common make file

include $(TEMPLATEROOT)/Makefile.common


