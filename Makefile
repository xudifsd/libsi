
prefix = /usr/local/
objects = malloc.o tree.o usage.o env.o stack.o math.o utils.o eval.o builtin.o apply.o setup.o
header = malloc.h tree.h usage.h env.h stack.h math.h utils.h eval.h interpret.h builtin.h types.h apply.h setup.h
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


libsi: $(objects) $(header)
	gcc -shared -fPIC -o libsi.so $(objects)

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

math.o: math.c $(header)
	gcc $(CFLAGS) math.c

utils.o: utils.c $(header)
	gcc $(CFLAGS) utils.c

eval.o: eval.c $(header)
	gcc $(CFLAGS) eval.c

builtin.o: builtin.c $(header)
	gcc $(CFLAGS) builtin.c

apply.o: apply.c $(header)
	gcc $(CFLAGS) apply.c

setup.o: setup.c $(header)
	gcc $(CFLAGS) setup.c

clean:
	-rm *.o
	-rm interpret.c t_tree t_stack t_main libsi
