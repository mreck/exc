CC = clang
CFLAGS = -Wextra -Wall -std=c99 -pedantic -ggdb -O3 -fPIC
OBJCTS = exc_container.o exc_string.o

all: exc.so exc.a

exc.so: $(OBJCTS)
	$(CC) $(CFLAGS) -shared $^ -o $@

exc.a: $(OBJCTS)
	ar r $@ $^

%.o: %.c %.h
	$(CC) -c $(CFLAGS) $<

clean:
	rm -f *.o *.so *.a
