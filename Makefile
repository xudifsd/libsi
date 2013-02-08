
prefix = /usr/local/
objects = malloc.o tree.o usage.o env.o stack.o math.o
header = malloc.h tree.h usage.h env.h stack.h math.h
CFLAGS = -c -g -Wall

all:

debug: t_tree

t_tree: t_tree.o malloc.o tree.o
	gcc -o t_tree t_tree.o malloc.o tree.o -lgc

t_tree.o: t_tree.c $(header)
	gcc $(CFLAGS) t_tree.c

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

clean:
	-find . -type f -regex ".*\.o" | xargs rm
	-rm t_tree
