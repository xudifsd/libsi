
prefix = /usr/local
objects = malloc.o tree.o usage.o env.o stack.o utils.o builtin.o setup.o
header = malloc.h tree.h usage.h env.h stack.h utils.h interpret.h builtin.h types.h setup.h
CFLAGS = -c -g -Wall
LIB = -lgc -lm

all: libsi

debug: t_tree t_stack t_main

t_tree: t_tree.o $(objects)
	gcc -o t_tree t_tree.o malloc.o tree.o $(LIB)

t_stack: t_stack.o interpret.o $(objects)
	gcc -o t_stack t_stack.o interpret.o $(objects) $(LIB)

t_main: t_main.o interpret.o $(objects)
	gcc -o t_main t_main.o interpret.o $(objects) $(LIB)

t_tree.o: t_tree.c $(header)
	gcc $(CFLAGS) t_tree.c

t_stack.o: t_stack.c $(header)
	gcc $(CFLAGS) t_stack.c

t_main.o: t_main.c $(header)
	gcc $(CFLAGS) t_main.c


libsi: interpret.o $(objects) $(header)
	gcc -shared -fPIC -o libsi.so interpret.o $(objects)

interpret.o: interpret.c $(header)
	gcc $(CFLAGS) interpret.c

interpret.c: interpret.l interpret.h
	flex -o interpret.c interpret.l

malloc.o: malloc.c $(header)
	gcc $(CFLAGS) malloc.c

tree.o: tree.c $(header)
	gcc $(CFLAGS) tree.c

usage.o: usage.c $(header)
	gcc $(CFLAGS) usage.c

env.o: env.c $(header)
	gcc $(CFLAGS) env.c

stack.o: stack.c $(header)
	gcc $(CFLAGS) stack.c

utils.o: utils.c $(header)
	gcc $(CFLAGS) utils.c

builtin.o: builtin.c $(header)
	gcc $(CFLAGS) builtin.c

setup.o: setup.c $(header)
	gcc $(CFLAGS) setup.c

install: all
	cp libsi.so $(prefix)/lib/
	-mkdir -p $(prefix)/include/si/
	cp u_types.h $(prefix)/include/si/types.h
	cp u_setup.h $(prefix)/include/si/setup.h
	cp u_builtin.h $(prefix)/include/si/builtin.h
	cp u_interpret.h $(prefix)/include/si/interpret.h
	cp u_malloc.h $(prefix)/include/si/malloc.h
	cp u_env.h $(prefix)/include/si/env.h
	-mkdir -p $(prefix)/share/si/
	cp setup.scm $(prefix)/share/si/

uninstall:
	-rm $(prefix)/lib/libsi.so
	-rm -rf $(prefix)/include/si/
	-rm -rf $(prefix)/share/si/

demo:
	make -C si/

clean:
	-rm -f *.o
	-rm -f interpret.c t_tree t_stack t_main libsi.so
	-make -C si/ clean
