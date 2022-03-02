typedef struct node {
    struct node *next;
    struct node *child;
    void ** elem;
    int num_child;
    int sib_index;
    int visit;
}tree_n;

// typedef struct TREENODE
// {
// 	struct TREENODE *parent;
// 	struct TREENODE *sibling;
//   struct TREENODE *leftmost_child;
//   struct TREENODE *rightmost_child;

//   struct TREENODE *node_inh;
//   struct TREENODE *node_syn;
//   st_wrapper *scope_sym_tab;
//   type *encl_fun_type_ptr;
//   symbol sym;
//   TOKEN token;
//   int num_child;
//   bool visited;
//   void *extra_args;
//   struct{
//     struct{
//       char *true_label;
//       char *false_label;
//     }boolean;
//     char *next_label;
//     char *cnstrct_code_begin;
//   }label;
//   char *addr;
//   struct{
//     int start;
//     int end;
//   }line_nums;
// } tree_node;
