# defaults, if no environment variables set

BVRS_RISCV ?= 0
BVRS_LLVM ?= 0
BVRS_OS ?= linux

ifeq ($(BVRS_RISCV),1)
	PREFIX ?= ../../build/target
	BVRS_OS ?= linux
	ARCH_STR ?= riscv64-unknown-${BVRS_OS}
	ifeq ($(BVRS_LLVM),0)
		# for GCC, ome defaults to get going
		ARCH_STR = riscv64-unknown-${BVRS_OS}-gnu
		CC ?= ${ARCH_STR}-gcc
		LD ?= ${ARCH_STR}-ld
		AR ?= ${ARCH_STR}-ar
	endif
else
	PREFIX ?= ../../build/host
endif

ORT_PREFIX ?= ../../build/host/bin/ort
OBJS		  = db.o backend.o
CFLAGS += -DDEBUG
CFLAGS += -I $(PREFIX)/include -g -Werror
CFLAGS_TEST = 
LDFLAGS += -L $(PREFIX)/lib -l:libsqlbox.a -l:libsqlite3.a -l:libkcgi.a -l:libkcgijson.a -l:libz.a
LDFLAGS += -ldl -lpthread -lcrypt -lm
LDFLAGS_TEST += -lcurl -l:libkcgiregress.a

ifeq ($(BVRS_OS),freebsd)
	LDFLAGS += -lmd
	ifeq ($(BVRS_RISCV),0) # native FreeBSD needs extra include/lib path
		LDFLAGS_TEST += -L/usr/local/lib
		CFLAGS_TEST += -I/usr/local/include
	endif
else # a platform with -lresolv, we assume
	LDFLAGS += -lresolv
endif

TESTS = tests/test0 tests/test1 tests/test2 tests/test3 tests/test4 \
		tests/test5 tests/test6 tests/test7 tests/test8 \
		tests/cgi_test0 tests/cgi_test1 tests/cgi_test2 tests/cgi_test3 \
		tests/cgi_test4 tests/cgi_test5 tests/cgi_test6 tests/cgi_test7

all: bvrs create_official

clean:
	rm -f db.h
	rm -f db.c
	rm -f $(OBJS)
	rm -f bvrs 

distclean: clean
	rm -f config.log config.h Makefile.configure

.DELETE_ON_ERROR:

%.o: %.c db.h
	$(CC) -c -o $@ $(CFLAGS) $<

db.h: bvrs.ort
	$(ORT_PREFIX)-c-header -jv bvrs.ort > db.h

db.c: bvrs.ort
	$(ORT_PREFIX)-c-source -Idjv -jv bvrs.ort > db.c

create_official: create_official.o $(OBJS) db.h
	$(CC) -o $@ create_official.o $(OBJS) $(LDFLAGS)

bvrs: main.o $(OBJS) db.h
	$(CC) -o $@ main.o $(OBJS) $(LDFLAGS)

bvrs.sql: bvrs.ort
	$(ORT_PREFIX)-sql bvrs.ort > bvrs.sql

tests/%: bvrs bvrs.sql
	@mkdir -p out
	@rm -f out/$(@F).db
	@sqlite3 out/$(@F).db < bvrs.sql
	@if [ -f tests/$(@F).sql ]; then sqlite3 out/$(@F).db < tests/$(@F).sql; fi
	@$(CC)	$(OBJS) tests/$(@F).c -I $(PWD) $(CFLAGS) $(CFLAGS_TEST) $(LDFLAGS) $(LDFLAGS_TEST) -o out/$(@F)
	@./out/$(@F) out/$(@F).db

# cgi_test%: bvrs bvrs.sql
#	mkdir -p out
#	rm -f out/$@.db
#	sqlite3 out/testdb.db < bvrs.sql
#	if [ -f tests/$@.sql ]; then sqlite3 out/testdb.db < tests/$@.sql; fi
#	$(CC)  $(OBJS) tests/$@.c -I $(PWD) $(CFLAGS) $(LDFLAGS) $(LDFLAGS_TEST) -o out/$@
#	./out/$@ out/$@.db

alltests: $(TESTS)
