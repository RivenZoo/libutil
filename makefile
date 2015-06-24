CFLAGS:=-g -Wall
INCLUDE:=-I./src

src := $(wildcard src/*.c)
obj := $(src:%.c=%.o)
dep := $(src:src%.c=build%.d)

.PHONY: libutil test_main clean

build/%.d: src/%.c
	@test -d ./build || mkdir -p ./build
	@set -e; rm -f $@;
	@$(CC) -M $(CPPFLAGS) $< > $@;

libutil: $(obj)
	ar -cr $@.a $(obj)

test_main: test_main.c libutil.a
	$(CC) $(CFLAGS) $(INCLUDE) $< -L./ -lutil -o $@

clean:
	-rm -rf $(obj) $(dep) libutil.a test_main

include $(dep)