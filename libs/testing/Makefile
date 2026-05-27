SRCS := functions.c testing_t.c shell.c
OBJS := $(SRCS:.c=.o)

%.o: %.c
	@clang -g -o testing.o -c testing.c \
		-DCISASTOR_LIBTESTING_PATH=\"$(abspath libtesting.a)\"
	@clang -g -o testing.o -c testing.c \
		-DCISASTOR_LIBTESTING_PATH=\"$(abspath libtesting.a)\" \
		-DCISASTOR_TESTING_ENTRY_OBJ=\"$(abspath testing.o)\"
	@clang -g -c $< -o $@

libtesting.a: $(OBJS)
	@ar rcs $@ $^

build: libtesting.a

clean:
	@rm *.o *.a 2>/dev/null || true
	@rm functions_test testing a.out 2>/dev/null || true

cli: libtesting.a
	@clang -o testing *.o

