CC      = gcc
CFLAGS  = -g -O2 -Wall -Werror

N = 10000
targets = part2_def part2_best part2_o2
cobj = main.o

all: $(targets)

.PHONY: all clean clobber

% : %.o $(cobj)
	$(CC) -g -o $@ $^

main.o: DEFINES = -DN=$(N)
part2_def.o: DEFINES = -DDEFAULT
part2_best.o:  DEFINES = -DBEST_OPTIMIZE
part2_o2.o:  DEFINES = -DOPTIMIZE2

main.o: main.c
part2_def.o: part2c.c
part2_best.o: part2c.c
part2_o2.o: part2c.c

$(targets:%=%.o) $(cobj) : %.o :
	$(CC) -c $(CFLAGS) $(DEFINES) -o $@ $<

clean:
	$(RM) $(cobj)
	$(RM) $(targets:%=%.o)

clobber: clean
	$(RM) $(targets)
