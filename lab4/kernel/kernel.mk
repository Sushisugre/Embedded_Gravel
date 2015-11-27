KERNEL = $(KDIR)/kernel
KSTART = $(KDIR)/start.o

# All core kernel objects go here.  Add objects here if you need to.
KOBJS := assert.o main.o enter_user_mode.o math.o memcheck.o raise.o ctype.o hexdump.o device.o swi_handler.o c_swi_handler.o irq_handler.o time_driver.o 
#KOBJS := assert.o main.o time_driver.o raise.o call_user.o read.o write.o exit.o time.o sleep.o swi_handler.o c_swi_handler.o irq_handler.o c_irq_handler.o
KOBJS := $(KOBJS:%=$(KDIR)/%)

-include $(KDIR)/arm/kernel.mk
-include $(KDIR)/syscall/kernel.mk
-include $(KDIR)/sched/kernel.mk
-include $(KDIR)/lock/kernel.mk

ALL_OBJS += $(KOBJS) $(KSTART)
ALL_CLOBBERS += $(KERNEL) $(KERNEL).bin

# Put everything needed by the kernel into the final binary.
# KOBJS contains core kernel objects.
# AOBJS contains objects that are ARM dependent.
# UOBJS contains objects that are U-boot dependent.

$(KERNEL): $(KSTART) $(KOBJS) $(UOBJS)
	@echo LD $(notdir $@)
	@$(LD) -static $(LDFLAGS) -o $@ $^ $(LIBGCC)

