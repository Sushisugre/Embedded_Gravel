PROGS_SPLAT_OBJS := rot13.o
PROGS_SPLAT_OBJS := $(PROGS_SPLAT_OBJS:%=$(TDIR)/rot13/%)
ALL_OBJS += $(PROGS_SPLAT_OBJS)

$(TDIR)/bin/splat : $(TSTART) $(PROGS_SPLAT_OBJS) $(TLIBC)
