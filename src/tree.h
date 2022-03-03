#include "treeADTDef.h"

node createEl(int lineNo, int parentSymbolID, int symbolID, int isLeafNode); 
tree_n * add_node(node el);
tree_n * add_sibling(tree_n * sib, tree_n * parent, node el);
// void add_sibling(tree_n * sib,tree_n * parent, tree_n * new_sib);
tree_n * add_child(tree_n * parent, node el );
// void add_child(tree_n * parent,tree_n * new_child );
// tree_n* remove_node(tree_n* parent, tree_n* prev_sib, tree_n* node);


// tree_node *get_nth_child(tree_node *root, int n);