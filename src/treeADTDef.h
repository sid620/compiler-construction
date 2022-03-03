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
    struct treeNode** children;
    node elem; 
    int numChild; 
} treeN; 
