CC = clang
CFLAGS = -Wextra -Wall -std=c99 -pedantic -ggdb -O3 -fPIC

OBJECTS = exc_container.o exc_string.o
HEADERS = exc_container.h exc_string.h exc_test.h

A_OBJECT = exc.a
S_OBJECT = exc.so

build: exc.so exc.a

archive: build
	tar -cvf exc.tar.gz $(HEADERS) $(A_OBJECT) $(S_OBJECT)

$(S_OBJECT): $(OBJECTS)
	$(CC) $(CFLAGS) -shared $^ -o $@

$(A_OBJECT): $(OBJECTS)
	ar r $@ $^

%.o: %.c %.h
	$(CC) -c $(CFLAGS) $<

clean:
	rm -vf *.o *.so *.a *.tar.gz
