typedef struct node { 
    int lineNo; 
    int parentNodeSymbolID; 
    int curr; 
    int isLeaf; 
    union lex { 
        char* lexemeStr; 
        int numVal; 
        float rVal; 
    } lex; 
    
} node; 

typedef struct treeNode {
    struct treeNode *next;
    struct treeNode *child;
    node elem; 
    int num_child;
    int sib_index;
    int visit;
}tree_n;