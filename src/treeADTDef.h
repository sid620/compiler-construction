typedef struct node { 
    int lineNo; 
    int parentNodeSymbolID; 
    int curr; 
    int isLeaf; 
    struct lex { 
        char* lexemeStr; 
        int numVal; 
        float rVal; 
    } lex; 
    int ruleNumber;     // This variable stores the rule number used for expansion, 0 means that it is a terminal
} node; 

typedef struct treeNode {
    struct treeNode** children;
    struct node elem; 
    int numChild; 
} treeN; 
