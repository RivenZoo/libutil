src := $(wildcard *.c)
obj := $(src:.c=.o)

.PHONY: test_main clean

%.d: %.c
	@set -e; rm -f $@;
	@$(CC) -M $(CPPFLAGS) $< > $@;

test_main: test_main.o file_info.o
	gcc $^ -g -Wall -o $@

clean:
	-rm -rf *.o *.d test

include $(src:.c=.d)