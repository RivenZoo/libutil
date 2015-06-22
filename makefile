src := $(wildcard *.c)
obj := $(src:.c=.o)

.PHONY: test_main clean

%.d: %.c
	@set -e; rm -f $@;
	@$(CC) -M $(CPPFLAGS) $< > $@;

test_main: test_main.o dir_walker.o file_info.o list.o
	clang $^ -g -Wall -o $@

clean:
	-rm -rf *.o *.d test

include $(src:.c=.d)