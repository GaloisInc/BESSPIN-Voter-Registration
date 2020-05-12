ifeq ($(BVRS_RISCV),1)
	PREFIX = ../../build/target
	BVRS_OS ?= linux
	ARCH_STR = riscv64-unknown-${BVRS_OS}-gnu
	CC=${ARCH_STR}-gcc
	LD=${ARCH_STR}-ld
	AR=${ARCH_STR}-ar
else
	PREFIX = ../../build/host
endif

OBJS		  = main.o
CFLAGS += -I $(PREFIX)/include
LFLAGS += -L $(PREFIX)/lib -l:libsqlite3.a -l:libkcgi.a -l:libz.a

all: bvrs

clean:
	rm -f $(OBJS)
	rm -f bvrs 

distclean: clean
	rm -f config.log config.h Makefile.configure

%.o: %.c
	$(CC) -c -o $@ $(CFLAGS) $<

bvrs: $(OBJS)
	$(CC) -o $@ $(OBJS) $(LFLAGS)
