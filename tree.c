#include "tree.h"
#include "malloc.h"

static inline struct tree_node *grandparent(struct tree_node *n) {
	return n->parent->parent;
}

static inline struct tree_node *uncle(struct tree_node *n) {
	if (n->parent == grandparent(n)->left)
		return grandparent(n)->right;
	else
		return grandparent(n)->left;
}

static inline void rotate_left(struct tree_node *p) {
	struct tree_node *c = p->right;
	p->right = c->left;
	if (p->right) {
		p->right->parent = p;
		p->right->pparent = &p->right;
	}
	c->left = p;
	c->parent = p->parent;
	p->parent = c;
	c->pparent = p->pparent;
	*(c->pparent) = c;
	p->pparent = &c->left;
}

static inline void rotate_right(struct tree_node *p) {
	struct tree_node *c = p->left;
	p->left = c->right;
	if (p->left) {
		p->left->parent = p;
		p->left->pparent = &p->left;
	}
	c->right = p;
	c->parent = p->parent;
	p->parent = c;
	c->pparent = p->pparent;
	*(c->pparent) = c;
	p->pparent = &c->right;
}

static void insert_case5(struct tree_node *n) {
	n->parent->color = BLACK;
	grandparent(n)->color = RED;
	if (n == n->parent->left && n->parent == grandparent(n)->left)
		rotate_right(grandparent(n));
	else
		rotate_left(grandparent(n));
}

static void insert_case4(struct tree_node *n) {
	if (n == n->parent->right && n->parent == grandparent(n)->left) {
		rotate_left(n->parent);
		n = n->left;
	} else if (n == n->parent->left && n->parent == grandparent(n)->right) {
		rotate_right(n->parent);
		n = n->right;
	}
	insert_case5(n);
}

static void insert_case1(struct tree_node *n);
/* avoid compile warning */

static void insert_case3(struct tree_node *n) {
	if (uncle(n) != NULL && uncle(n)->color == RED) {
		n->parent->color = BLACK;
		uncle(n)->color = BLACK;
		grandparent(n)->color = RED;
		insert_case1(grandparent(n));
	} else
		insert_case4(n);
}

static void insert_case2(struct tree_node *n) {
	if (n->parent->color == BLACK)
		return;
	else
		insert_case3(n);
}

static void insert_case1(struct tree_node *n) {
	if (n->parent == NULL)
		n->color = BLACK;
	else
		insert_case2(n);
}

/* return 1 for found, 0 for none */
int tree_find(struct tree_node *root, const char *str, struct exp **rtn) {
	struct tree_node *p = root;
	int cmp;
	while (p) {
		cmp = strcmp(str, p->sym->sym);
		if (cmp > 0)
			p = p->right;
		else if (cmp < 0)
			p = p->left;
		else {
			*rtn = p->value;
			return 1;
		}
	}
	return 0;
}

/* *
 * return value: 0 if have inserted successfully, 1 if *failed* at insert
 * if `replace' is
 * 0: do not permit replacement
 * 1: must replace, do not alloc new node, this is for set_in_env
 */
int tree_insert(struct tree_node **root, struct symbol *sym, struct exp *e, int replace) {
	struct tree_node **cur;
	struct tree_node *p;
	struct tree_node *parent;
	int cmp;
	for (cur = root, parent = NULL; *cur;) {
		p = *cur;
		cmp = strcmp(sym->sym, p->sym->sym);
		if (cmp > 0) {
			parent = p;
			cur = &p->right;
		} else if (cmp < 0) {
			parent = p;
			cur = &p->left;
		} else {
			if (replace) {
				/* overwrite */
				p->value = e;
				return 0;
			}
			return 1;
		}
	}
	if (replace)
		return 1;
	*cur = alloc_tree_node(parent, cur, sym, e);
	insert_case1(*cur);
	return 0;
}

/* code below is for debugging */
typedef void (*handler_fn)(struct tree_node *);

static void traverse(struct tree_node *p, handler_fn handler) {
	handler(p);
	if (p->left)
		traverse(p->left, handler);
	if (p->right)
		traverse(p->right, handler);
}

static void to_dot(struct tree_node *cur) {
	if (cur->left != NULL)
		fprintf(stderr, "\t\"%s\" -> \"%s\"[headport=n, tailport=sw]\n", cur->sym->sym, cur->left->sym->sym);

	if (cur->parent != NULL)
		fprintf(stderr, "\t\"%s\" -> \"%s\"[headport=s, tailport=n]\n", cur->sym->sym, cur->parent->sym->sym);

	fprintf(stderr, "\t\"%s\"[color=%s]\n", cur->sym->sym, (cur->color == RED)?"red":"black");

	if (cur->right != NULL)
		fprintf(stderr, "\t\"%s\" -> \"%s\"[headport=n, tailport=se]\n", cur->sym->sym, cur->right->sym->sym);
}

void dump(struct tree_node *root) {
	fprintf(stderr, "digraph heaptree{\n");
	traverse(root, to_dot);
	fprintf(stderr, "}\n");
}
