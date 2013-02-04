#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "tree.h"
#include "malloc.h"

typedef void (*handler_fn)(struct tree_node *);

static void traverse(struct tree_node *p, handler_fn handler) {
	if (p->left)
		traverse(p->left, handler);
	handler(p);
	if (p->right)
		traverse(p->right, handler);
}

void to_dot(struct tree_node *cur) {
	if (cur->left != NULL)
		fprintf(stderr, "\t\"%s\" -> \"%s\"[headport=n, tailport=sw]\n", cur->sym->sym, cur->left->sym->sym);

	fprintf(stderr, "\t\"%s\"[color=%s]\n", cur->sym->sym, (cur->color == RED)?"red":"black");

	if (cur->right != NULL)
		fprintf(stderr, "\t\"%s\" -> \"%s\"[headport=n, tailport=sw]\n", cur->sym->sym, cur->right->sym->sym);
}

int main(int argc, char *argv[]) {
	/* run with `./t_tree 2> out.dot && dot -Tpng -o t.png out.dot` */
	struct tree_root root;
	int i;
	struct symbol *sym[26];
	struct number *num[26];
	char template[] = "a";
	memset(&root, 0, sizeof(struct tree_root));

	for (i = 0; i < 26; i++) {
		template[0] += i;
		sym[i] = alloc_symbol(template);
		num[i] = alloc_long(100l + i);
		tree_insert(&root, sym[i], (struct exp *)num[i]);
		template[0] = 'a';
	}
	fprintf(stderr, "digraph heaptree%d{\n", i);
	traverse(root.node, to_dot);
	fprintf(stderr, "}\n");
	assert(((struct number *)tree_find(&root, "c"))->l_value == 100 + 'c' - 'a');
	assert(((struct number *)tree_find(&root, "z"))->l_value == 100 + 'z' - 'a');
	assert(((struct number *)tree_find(&root, "d"))->l_value == 100 + 'd' - 'a');
	assert(((struct number *)tree_find(&root, "j"))->l_value == 100 + 'j' - 'a');
	assert(((struct number *)tree_find(&root, "m"))->l_value == 100 + 'm' - 'a');
	exit(0);
}
