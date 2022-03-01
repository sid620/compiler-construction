#include "treeADTDef.h"

tree_n * new_node();
tree_n * add_sibling(tree_n * sib, void *el);
void add_sibling(tree_n * sib,tree_n * parent, tree_n * new_sib);
tree_n * add_child(tree_n * parent,void * el );
void add_child(tree_n * parent,tree_n * new_child );
tree_n* remove_node(tree_n* parent, tree_n* prev_sib,tree_n* node);


// tree_node *get_nth_child(tree_node *root, int n);