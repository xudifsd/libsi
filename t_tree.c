#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "tree.h"
#include "malloc.h"

int main(int argc, char *argv[]) {
	/* run with `./t_tree 2> out.dot && dot -Tpng -o t.png out.dot` */
	struct tree_node *root = NULL;
	int i;
	struct symbol *sym[26];
	struct number *num[26];
	char template[] = "a";

	for (i = 0; i < 26; i++) {
		template[0] += i;
		sym[i] = alloc_symbol(template);
		num[i] = alloc_long(100l + i);
		tree_insert(&root, sym[i], (struct exp *)num[i], 0);
		template[0] = 'a';
	}

	dump(root);

	assert(((struct number *)tree_find(root, "c"))->l_value == 100 + 'c' - 'a');
	assert(((struct number *)tree_find(root, "z"))->l_value == 100 + 'z' - 'a');
	assert(((struct number *)tree_find(root, "d"))->l_value == 100 + 'd' - 'a');
	assert(((struct number *)tree_find(root, "j"))->l_value == 100 + 'j' - 'a');
	assert(((struct number *)tree_find(root, "m"))->l_value == 100 + 'm' - 'a');
	exit(0);
}
