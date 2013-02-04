#include "tree.h"
#include "malloc.h"
#include <assert.h>

static inline struct tree_node *grandparent(struct tree_node *n) {
	return n->parent->parent;
}

static inline struct tree_node *uncle(struct tree_node *n) {
	if (n->parent == grandparent(n)->left)
		return grandparent(n)->right;
	else
		return grandparent(n)->left;
}

static inline struct tree_node *rotate_left(struct tree_node *p) {
	struct tree_node *n = p->right;
	p->right = n->left;
	n->left = p;
	n->parent = p->parent;
	p->parent = n;
	return n;
}

static inline struct tree_node *rotate_right(struct tree_node *g) {
	struct tree_node *p = g->left;
	p->parent = g->parent;
	g->parent = p;
	g->left = p->right;
	p->right = g;
	return p;
}

static void insert_case5(struct tree_node *n, struct tree_root *root) {
	n->parent->color = BLACK;
	grandparent(n)->color = RED;
	if (n == n->parent->left && n->parent == grandparent(n)->left) {
		if (grandparent(n)->parent) {
			if (grandparent(n)->parent->left == grandparent(n))
				grandparent(n)->parent->left = rotate_right(grandparent(n));
			else {
				assert(grandparent(n)->parent->right == grandparent(n));
				grandparent(n)->parent->right = rotate_right(grandparent(n));
			}
		} else
			root->node = rotate_right(grandparent(n));
	} else {
		if (grandparent(n)->parent) {
			if (grandparent(n)->parent->left == grandparent(n))
				grandparent(n)->parent->left = rotate_left(grandparent(n));
			else {
				assert(grandparent(n)->parent->right == grandparent(n));
				grandparent(n)->parent->right = rotate_left(grandparent(n));
			}
		} else
			root->node = rotate_left(grandparent(n));
	}
}

static void insert_case4(struct tree_node *n, struct tree_root *root) {
	if (n == n->parent->right && n->parent == grandparent(n)->left) {
		grandparent(n)->left = rotate_left(n->parent);
		n = n->left;
	} else if (n == n->parent->left && n->parent == grandparent(n)->right) {
		grandparent(n)->right = rotate_right(n->parent);
		n = n->right;
	}
	insert_case5(n, root);
}

static void insert_case1(struct tree_node *n, struct tree_root *root);
/* avoid compile warning */

static void insert_case3(struct tree_node *n, struct tree_root *root) {
	if (uncle(n) != NULL && uncle(n)->color == RED) {
		n->parent->color = BLACK;
		uncle(n)->color = BLACK;
		grandparent(n)->color = RED;
		insert_case1(grandparent(n), root);
	}
	else
		insert_case4(n, root);
}

static void insert_case2(struct tree_node *n, struct tree_root *root) {
	if (n->parent->color == BLACK)
		return;
	else
		insert_case3(n, root);
}

static void insert_case1(struct tree_node *n, struct tree_root *root) {
	if (n->parent == NULL)
		n->color = BLACK;
	else
		insert_case2(n, root);
}

struct exp *tree_find(struct tree_root *root, const char *str) {
	struct tree_node *p = root->node;
	int cmp;
	while (p) {
		cmp = strcmp(str, p->sym->sym);
		if (cmp > 0)
			p = p->right;
		else if (cmp < 0)
			p = p->left;
		else
			return p->value;
	}
	return NULL;
}

void tree_insert(struct tree_root *root, struct symbol *sym, struct exp *e) {
	struct tree_node *p = root->node;
	int cmp;
	if (!p) {
		/* insert case 1 */
		root->node = alloc_tree_node(NULL, sym, e);
		root->node->color = BLACK;
		return;
	}
	for (;;) {
		cmp = strcmp(sym->sym, p->sym->sym);
		if (cmp > 0) {
			if (p->right)
				p = p->right;
			else {
				p->right = alloc_tree_node(p, sym, e);
				insert_case2(p->right, root);
				return;
			}
		} else if (cmp < 0) {
			if (p->left)
				p = p->left;
			else {
				p->left = alloc_tree_node(p, sym, e);
				insert_case2(p->left, root);
				return;
			}
		} else {
			/* overwrite */
			p->value = e;
			return;
		}
	}
}
