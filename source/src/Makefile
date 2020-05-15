ifeq ($(BVRS_RISCV),1)
	# Some defaults to get going
	PREFIX = ../../build/target
	BVRS_OS ?= linux
	ARCH_STR = riscv64-unknown-${BVRS_OS}-gnu
	CC=${ARCH_STR}-gcc
	LD=${ARCH_STR}-ld
	AR=${ARCH_STR}-ar
else
	PREFIX = ../../build/host
endif

ORT_PREFIX = ../../build/host/bin/ort
OBJS		  = db.o backend.o
CFLAGS += -I $(PREFIX)/include -g -Werror
LFLAGS += -L $(PREFIX)/lib -l:libsqlbox.a -l:libsqlite3.a -l:libkcgi.a -l:libkcgijson.a -l:libz.a
LFLAGS += -ldl -lpthread -lcrypt -lresolv -lm
LFLAGS_TEST = -lcurl -l:libkcgiregress.a

TESTS = test0 test1 test2 test3 test4

all: bvrs

clean:
	rm -f db.h
	rm -f db.c
	rm -f $(OBJS)
	rm -f bvrs 

distclean: clean
	rm -f config.log config.h Makefile.configure

%.o: %.c db.h
	$(CC) -c -o $@ $(CFLAGS) $<

db.h: bvrs.ort
	$(ORT_PREFIX)-c-header -jv bvrs.ort > db.h

db.c: bvrs.ort
	$(ORT_PREFIX)-c-source -Idjv -jv bvrs.ort > db.c

bvrs: main.o $(OBJS) db.h
	$(CC) -o $@ main.o $(OBJS) $(LFLAGS)

bvrs.sql: bvrs.ort
	$(ORT_PREFIX)-sql bvrs.ort > bvrs.sql

test%: $(OBJS) bvrs.sql
	mkdir -p out
	rm -f out/testdb.db
	sqlite3 out/testdb.db < bvrs.sql
	if [ -f tests/$@.sql ]; then sqlite3 out/testdb.db < tests/$@.sql; fi
	$(CC)  $(OBJS) tests/$@.c -I $(PWD) $(CFLAGS) $(LFLAGS) $(LFLAGS_TEST) -o out/$@
	./out/$@ out/testdb.db

cgi_test%: bvrs bvrs.sql
	mkdir -p out
	rm -f out/$@.db
	sqlite3 out/$@.db < tests/$@.sql
	$(CC)  $(OBJS) tests/$@.c -I $(PWD) $(CFLAGS) $(LFLAGS) $(LFLAGS_TEST) -o out/$@
	./out/$@ out/$@.db

alltests: $(TESTS)
