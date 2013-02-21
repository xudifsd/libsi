
prefix = /usr/local/
objects = malloc.o tree.o usage.o env.o stack.o math.o utils.o eval.o quote_stack.o
header = malloc.h tree.h usage.h env.h stack.h math.h utils.h eval.h interpret.h quote_stack.h
CFLAGS = -c -g -Wall

all:

debug: t_tree t_stack t_main

t_tree: t_tree.o $(objects)
	gcc -o t_tree t_tree.o malloc.o tree.o -lgc

t_stack: t_stack.o $(objects)
	gcc -o t_stack t_stack.o $(objects) -lgc

t_main: t_main.o interpret.o $(objects)
	gcc -o t_main t_main.o interpret.o $(objects) -lgc

t_tree.o: t_tree.c $(header)
	gcc $(CFLAGS) t_tree.c

t_stack.o: t_stack.c $(header)
	gcc $(CFLAGS) t_stack.c

t_main.o: t_main.c $(header)
	gcc $(CFLAGS) t_main.c


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

quote_stack.o: quote_stack.c $(header)
	gcc $(CFLAGS) quote_stack.c

clean:
	-find . -type f -regex ".*\.o" | xargs rm
	-rm interpret.c t_tree t_stack t_main
