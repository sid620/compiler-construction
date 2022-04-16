// #pragma once
#include "ast.h"
#include<stdio.h>
#include<stdlib.h>

/*
    Creates a new node for the AST
*/
astNode *mknode(node elem, grammar G){
    astNode *new = (astNode *)malloc(sizeof(astNode));
    new->child  = (astNode *)malloc(sizeof(astNode));
    new->next   = (astNode *)malloc(sizeof(astNode));
    new->next=NULL;
    new->child=NULL;
    // new->numChildren=0;
    new->elem=(node *)malloc(sizeof(node));
    new->elem->curr = elem.curr;
    new->elem->isLeaf = elem.isLeaf;
    new->elem->lineNo = elem.lineNo;
    new->elem->parentNodeSymbolID = elem.parentNodeSymbolID;
    new->elem->ruleNumber = elem.ruleNumber;
    if(elem.isLeaf){
        if(strcmp(G.terminals[elem.curr],"TK_NUM")==0)
            new->elem->lex.numVal = elem.lex.numVal;
        else if(strcmp(G.terminals[elem.curr],"TK_RNUM")==0)
            new->elem->lex.rVal = elem.lex.rVal;
        else 
            new->elem->lex.lexemeStr = elem.lex.lexemeStr;
    }
    else
        new->elem->lex.lexemeStr = elem.lex.lexemeStr;
    return new;
}

/*
    Adds the node:child to the node:parent at the end of the linked list
*/
void addChildAST(astNode *parent, astNode *child){
    if(parent->child==NULL){

        parent->child = (astNode *)malloc(sizeof(astNode));
        parent->child = child;
    }
    else{
        astNode *temp;
        temp = parent->child;
        while(temp->next!=NULL)temp = temp->next;
        temp->next  = (astNode *)malloc(sizeof(astNode));
        temp->next = child;
        // parent->numChildren++;
    }
}

/*
    Adds the node:sibling after node:curr in the linked list of AST level
*/
void insertAST(astNode *curr, astNode *sibling){
    // astNode *temp = curr->next;
    sibling->next = curr->next;
    curr->next = sibling;
    // sibling->next = temp;
}

/*
    Checks if the terminal is to be kept in the AST as a node  
*/
bool isUseful(int tokenID){
    switch(tokenID-1){
        case TK_FIELDID:
        case TK_DEFINETYPE:
        case TK_AS:
        case TK_ASSIGNOP:
        // case TK_CALL:
        // case TK_PARAMETERS:
        case TK_ENDIF:
        case TK_READ:
        case TK_WRITE:
        case TK_NUM:
        case TK_RNUM:
        case TK_AND:
        case TK_OR:
        case TK_LT:
        case TK_LE:
        case TK_EQ:
        case TK_GT:
        case TK_GE:
        case TK_NE:
        case TK_RETURN:
        case TK_FUNID: 
        // case TK_INPUT:
        // case TK_OUTPUT:
        case TK_ID:
        case TK_INT:
        case TK_REAL:
        case TK_RECORD:
        case TK_UNION:
        case TK_RUID:
        case TK_GLOBAL:
        case TK_IF:
        case TK_THEN:
        case TK_PLUS:
        case TK_MUL:
        case TK_DIV:
        case TK_MINUS:
        case TK_NOT:
            return true;
            break;
        default:{
            return false;
        }
    }
}

astNode *findChild(astNode *node, int position, bool insertUsed, int insertCount){
    
    astNode * temp = insertUsed? node:node->child;
    int j = 0;
    while(j<position+insertCount){
        if(temp==NULL)printf("Halt\n");
        temp = temp->next;
        j++;
    }
    return temp;
}

/*
    Constructs the AST by recursively traversing the Parse Tree 
*/
void constructAst(astNode *node, treeN *root, grammar G, int *insertPrev, astNode *mainRoot){
    // printf("Here %s\n",G.nonTerminals[node->elem->curr]);
    bool *insertUsed = (bool *)malloc(sizeof(bool));
    int *insertCount = (int *)malloc(sizeof(int));
    *insertCount=0;
    *insertUsed = false;
    if(!root)return;
    if(root->elem.isLeaf){
        // printf("Rule %d Leaf %s\n",node->elem->ruleNumber,G.terminals[root->elem.curr]);
        return;
    }
    printf("constructing node rule:%d NT: %s, number of children in parse tree: %d\n",root->elem.ruleNumber,root->elem.isLeaf?G.terminals[root->elem.curr]:G.nonTerminals[root->elem.curr],root->numChild);
    performAction(root->elem.ruleNumber,node,root,G,insertUsed,insertPrev);
    int j = 0;
    for(int i = root->numChild-1;i >= 0;i--){
        // printf("Parent name %s child name %s rule number %d ast number %d curr number %d\n",root->children[i]->elem.isLeaf?G.terminals[root->children[i]->elem.curr]:G.nonTerminals[root->children[i]->elem.curr],root->children[i]->elem.isLeaf?G.terminals[root->children[i]->elem.curr]:G.nonTerminals[root->children[i]->elem.curr],node->elem->ruleNumber,j,root->children[i]->elem.curr);
        if(root->children[i]->elem.ruleNumber!=0 || (root->children[i]->elem.isLeaf && isUseful(root->children[i]->elem.curr))){
            if(!root->children[i]->elem.isLeaf){
                astNode *temp = findChild(node,j,*insertUsed,*insertCount);
                // printf("Inside \n");
                // if(temp==NULL)
                // printf("Hi %d %s rule number %d\n",j,G.nonTerminals[node->elem->curr],node->elem->ruleNumber);
                // printf("hello\n");
                // printf("before construct Parent name: %s Child number %d child name: %s\n",temp->elem->isLeaf?G.terminals[temp->elem->curr]:G.nonTerminals[temp->elem->curr],j,root->children[i]->elem.isLeaf?G.terminals[root->children[i]->elem.curr]:G.nonTerminals[root->children[i]->elem.curr]);
                // int x = *insertCount;
                constructAst(temp,root->children[i],G,insertCount,mainRoot);
                // if(*insertCount>x)*insertPrev+=(*insertCount-x);
                // printf("Parent name: %s Child rule number %d rule number child belongs to %d child name: %s\n",temp->elem->isLeaf?G.terminals[temp->elem->curr]:G.nonTerminals[temp->elem->curr],temp->elem->ruleNumber,node->elem->ruleNumber,root->children[i]->elem.isLeaf?G.terminals[root->children[i]->elem.curr]:G.nonTerminals[root->children[i]->elem.curr]);
            }
            else{
                astNode *temp = findChild(node,j,*insertUsed,*insertCount);
                printf("Leaf %s in parse tree, belongs to rule %d\n",G.terminals[temp->elem->curr],node->elem->ruleNumber);
            }
            j++;
        }
        else{
            printf("Child %s not in abstract syntax tree. Rule number child belongs to: %d lineNo: %d\n",G.terminals[root->children[i]->elem.curr],root->elem.ruleNumber,root->children[i]->elem.lineNo);
        }
        
    }
    if(*insertUsed)*insertPrev+=*insertCount;
    // printf("\n\n**************************************************************************************\n\n");
    // printf("First level of mainRoot offSet value: %d\n",*insertPrev);
    // astNode *curr = mainRoot->child;
    // while(curr!=NULL){
    //     printf("isLeaf: %d curr: %d name:%s Line:%d\n",curr->elem->isLeaf,curr->elem->curr,curr->elem->isLeaf?G.terminals[curr->elem->curr]:G.nonTerminals[curr->elem->curr],curr->elem->lineNo);
    //     curr = curr->next;
    // }
    // printf("Over here\n");
    // printf("\n\n**************************************************************************************\n\n");
    // printf("Recurse back to parent of %s\n",node->elem->isLeaf?G.terminals[node->elem->curr]:G.nonTerminals[node->elem->curr]);
}

/*
    Applies the appropriate semantic rules and creates the necessary nodes for the corresponding rule
*/
void performAction(int ruleNumber, astNode *node, treeN *root, grammar G, bool *insertUsed, int *insertCount){
    switch (ruleNumber){
    case 1:{
        //<program> ===> <otherFunctions> <mainFunction>
        // make  node for <otherFunctions> and <mainFunction>
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        astNode *node2 = mknode(root->children[root->numChild-1-1]->elem,G);
        addChildAST(node, node1);
        addChildAST(node, node2);
        break;
    }
    case 2:{
        //<mainFunction> ===> TK_MAIN <stmts> TK_END
        // make node for <stmts>
        // printf("Rule 2 has BT\n");
        astNode *node1 = mknode(root->children[root->numChild-1-1]->elem,G);
        // Old rule:
        // node1->next = node->next;
        // *node = *node1;
        // *insertUsed = true;

        // new rule
        addChildAST(node, node1);
        // printf("Rule 2 has no BT\n");
        break;
    }
    case 3:{
        //<otherFunctions> ===> <function> <otherFunctions1>
        // make node for <function> and <otherFunctions1>
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        astNode *node2 = mknode(root->children[root->numChild-1-1]->elem,G);
        //old rule
        // printf("next of rule 3 is %s\n",G.nonTerminals[node->elem->curr]);
        // Change 3: ignore otherFunctions
        node1->next = node->next;
        *node = *node1;
        // printf("Rule 3 next %s\n",G.nonTerminals[node->elem->curr]);
        // if(node1->next==NULL)printf("next of rule 3 is null\n");
        insertAST(node, node2);
        *insertUsed = true;
        (*insertCount)++;

        //new rule
        // addChildAST(node, node1);
        // addChildAST(node, node2);
        break;
    }
    case 4:{
        //<otherFunctions> ===> ∈
        // otherFunctions --> epsilon
        // can't assign null else next will be inaccessible
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        node1->next = node->next;
        *node = *node1;
        *insertUsed = true;
        break;
    }
    case 5:{
        //<function> ===> TK_FUNID <input_par> <output_par> TK_SEM <stmts> TK_END
        // create node for FUNID, input_par, output_par, and stmts
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        astNode *node2 = mknode(root->children[root->numChild-1-1]->elem,G);
        astNode *node3 = mknode(root->children[root->numChild-1-2]->elem,G);
        astNode *node4 = mknode(root->children[root->numChild-1-4]->elem,G);
        // printf("Rule 5 BT %s\n",G.nonTerminals[node->elem->curr]);
        // Add children

        // if(node==NULL)printf("Yes\n");
        addChildAST(node, node1);
        addChildAST(node, node2);
        addChildAST(node, node3);
        addChildAST(node, node4);
        // printf("Rule 5 no BT\n");
        break;
        
    }
    case 6: {
        // <input_par> ===> TK_INPUT TK_PARAMETER TK_LIST TK_SQL <parameter_list> TK_SQR
        // mknode for TK_INPUT and <parameter_list>
        // Change 5: ignore TK_INPUT, only parameter_list is child of input_par
        // astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        astNode *node2 = mknode(root->children[root->numChild-1-4]->elem,G);
        // old rule
        // node1->next = node->next;
        // *node = *node1;
        // insertAST(node, node2);
        // *insertUsed = true;
        // (*insertCount)++;

        //new rule
        // addChildAST(node, node1);
        addChildAST(node, node2);
        // printf("No BT in rule 6 %s\n",G.nonTerminals[node2->elem->curr]);
        break;
    }
    case 7: {
        //<output_par> ===> TK_OUTPUT TK_PARAMETER TK_LIST TK_SQL <parameter_list> TK_SQR
        // Change 5: ignore TK_OUTPUT only parameter_list is child of output_par
        // astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        astNode *node2 = mknode(root->children[root->numChild-1-4]->elem,G);
        //old rule
        // node1->next = node->next;
        // *node = *node1;
        // insertAST(node, node2);
        // (*insertCount)++;
        // *insertUsed = true;

        //new rule
        // addChildAST(node, node1);
        addChildAST(node, node2);
        break;
    }
    case 8:{
        // <output_par>===>∈
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        node1->next = node->next;
        *insertUsed = true;
        break;
    }
    case 9:{
        //<parameter_list> ===> <dataType> TK_ID <remaining_list>
        // printf("Rule 9 has BT\n");
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        astNode *node2 = mknode(root->children[root->numChild-1-1]->elem,G);
        astNode *node3 = mknode(root->children[root->numChild-1-2]->elem,G);
        //Change 6: ignore parameter_lis
        // addChildAST(node,node1);
        // addChildAST(node,node2);
        // addChildAST(node,node3);
        node1->next = node->next;
        *node = *node1;
        insertAST(node, node2);
        insertAST(node2, node3);
        *insertUsed = true;
        *insertCount+=2;
        // printf("Rule 9 has no BT\n");
        break;
    }
    case 10:{
        //<dataType> ===> <primitiveDatatype> 
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        node1->next = node->next;
        *node = *node1;
        *insertUsed = true;
        break; 
    }
    case 11:{
        //<dataType> ===> <constructedDatatype>
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        node1->next = node->next;
        *node = *node1;
        *insertUsed = true;

        break;
    }
    case 12:{
        //<primitiveDatatype> ===> TK_INT
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        node1->next = node->next;
        *node = *node1;
        *insertUsed = true;

        break;
    }
    case 13:{
        //<primitiveDatatype> ===> TK_REAL
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);

        node1->next = node->next;
        *node = *node1;
        *insertUsed = true;

        break;
    }
    case 14:{
        //<constructedDatatype> ===> TK_RECORD TK_RUID
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        astNode *node2 = mknode(root->children[root->numChild-1-1]->elem,G);
        // addChildAST(node, node1);
        // addChildAST(node, node2);
        node1->next = node->next;
        *node = *node1;
        insertAST(node, node2);
        *insertUsed = true;
        (*insertCount)++;
        break;
    }
    case 15:{
        //<constructedDatatype> ===> TK_UNION TK_RUID
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        astNode *node2 = mknode(root->children[root->numChild-1-1]->elem,G);
        // addChildAST(node, node1);
        // addChildAST(node, node2);
        node1->next = node->next;
        *node = *node1;
        insertAST(node, node2);
        *insertUsed = true;
        (*insertCount)++;
        break;
    }
    case 16:{
        //<constructedDatatype> ===> TK_RUID
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        node1->next = node->next;
        *node = *node1;
        *insertUsed = true;
        break;
    }
    case 17:{
        //<remaining_list> ===> TK_COMMA <parameter_list>
        // printf("Rule 17 has BT\n");
        astNode *node1 = mknode(root->children[root->numChild-1-1]->elem,G);
        node1->next = node->next;
        *node = *node1;
        *insertUsed = true;

        // printf("Rule 17 has no BT\n");
        break;
    }
    case 18:{
        //<remaining_list> ===> ∈
        // can't assign null else next will be inaccessible
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        node1->next = node->next;
        *node = *node1;
        *insertUsed = true;
        break;
    }
    case 19:{
        /* Change 1: ignore stmts, substitutes stmts with these four nodes*/
        //<stmts> ===> <typeDefinitions> <declarations> <otherStmts> <returnStmt>
        // printf("Rule 19 has BT\n");
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        astNode *node2 = mknode(root->children[root->numChild-1-1]->elem,G);
        astNode *node3 = mknode(root->children[root->numChild-1-2]->elem,G);
        astNode *node4 = mknode(root->children[root->numChild-1-3]->elem,G);
        // old rule
        node1->next = node->next;
        *node = *node1;
        insertAST(node, node2);
        insertAST(node2, node3);
        insertAST(node3, node4);
        *insertUsed = true;
        *insertCount+=3;

        // new rule: adds a node corresponding to stmts
        // addChildAST(node, node1);
        // addChildAST(node, node2);
        // addChildAST(node, node3);
        // addChildAST(node, node4);
        
        // printf("Rule 19 has no BT\n");
        break;
    }
    case 20:{
        //<typeDefinitions> ===> <actualOrRedefined> <typeDefinitions1>
        // printf("Rule 20 has BT\n");
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        astNode *node2 = mknode(root->children[root->numChild-1-1]->elem,G);
        // old rule
        // node1->next = node->next;
        // *node = *node1;
        // insertAST(node, node2);
        // *insertUsed = true;
        // (*insertCount)++;
        // new rule
        addChildAST(node, node1);
        addChildAST(node, node2);
        // printf("Rule 20 has no BT\n");
        break;
    }
    case 21:{
        //<typeDefinitions> ===> ∈
        // can't assign null else next will be inaccessible
        // printf("Rule 21 has BT\n");
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        node1->next = node->next;
        *node = *node1;
        *insertUsed = true;
        // printf("Rule 21 has no BT\n");
        break;
    }
    case 22:{
        //<actualOrRedefined> ===> <typeDefinition>
        // printf("Rule 22 has BT\n");
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        node1->next = node->next;
        *node = *node1;
        *insertUsed = true;
        // printf("Rule 22 has no BT\n");
        break;
    }
    case 23:{
        //<actualOrRedefined> ===> <definetypestmt>
        // printf("Rule 23 has BT\n");
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        node1->next = node->next;
        *node = *node1;
        *insertUsed = true;
        // printf("Rule 23 has no BT\n");
        break;
    }
    case 24:{
        //<typeDefinition> ===> TK_RECORD TK_RUID <fieldDefinitions> TK_ENDRECORD
        // Rule changed here, else exceptions would be needed
        // printf("Rule 24 has no BT\n");
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        astNode *node2 = mknode(root->children[root->numChild-1-1]->elem,G);
        astNode *node3 = mknode(root->children[root->numChild-1-2]->elem,G);
        // node1->next = node->next;
        // *node = *node1;
        addChildAST(node, node1);
        addChildAST(node, node2);
        addChildAST(node, node3);
        // printf("Rule 24 has no BT\n");
        break;
    }
    case 25:{
        //<typeDefinition> ===> TK_UNION TK_RUID <fieldDefinitions> TK_ENDUNION
        // Rule changed here, else exceptions would be needed
        // printf("Rule 25 has BT\n");
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        astNode *node2 = mknode(root->children[root->numChild-1-1]->elem,G);
        astNode *node3 = mknode(root->children[root->numChild-1-2]->elem,G);
        // node1->next = node->next;
        // *node = *node1;
        addChildAST(node, node1);
        addChildAST(node, node2);
        addChildAST(node, node3);
        // printf("Rule 25 has no BT\n");
        break;        
    }
    case 26:{
        //<fieldDefinitions> ===> <fieldDefinition> <fieldDefinition1> <moreFields>
        // printf("Rule 26 has BT\n");
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        astNode *node2 = mknode(root->children[root->numChild-1-1]->elem,G);
        astNode *node3 = mknode(root->children[root->numChild-1-2]->elem,G);
        // old rule
        // node1->next = node->next;
        // *node = *node1;
        // insertAST(node, node2);
        // insertAST(node2, node3);
        // *insertUsed = true;
        // *insertCount+=2;
        // new rule
        addChildAST(node, node1);
        addChildAST(node, node2);
        addChildAST(node, node3);
        // printf("Rule 26 has no BT\n");
        break;
    }
    case 27:{
        //<fieldDefinition> ===> TK_TYPE <fieldType> TK_COLON TK_FIELDID TK_SEM
        // printf("Rule 27 has BT\n");
        astNode *node1 = mknode(root->children[root->numChild-1-1]->elem,G);
        astNode *node2 = mknode(root->children[root->numChild-1-3]->elem,G);
        addChildAST(node, node1);
        addChildAST(node, node2);
        // printf("Rule 27 has no BT\n");
        break;
    }
    case 28:{
        // <fieldType> ===> <primitiveDatatype>
        // printf("Rule 28 has BT\n");
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        node1->next = node->next;
        *node = *node1;
        *insertUsed = true;
        // printf("Rule 28 has no BT\n");
        break;
    }
    case 29:{
        // <fieldType> ===> TK_RUID
        // printf("Rule 29 has BT\n");
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        node1->next = node->next;
        *node = *node1;
        *insertUsed = true;
        // printf("Rule 29 has no BT\n");
        break;
    }
    case 30:{
        //<moreFields> ===> <fieldDefinition> <moreFields1>
        // printf("Rule 30 has BT\n");
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        astNode *node2 = mknode(root->children[root->numChild-1-1]->elem,G);
        // old rule
        // node1->next = node->next;
        // *node = *node1;
        // insertAST(node, node2);
        // *insertUsed = true;
        // (*insertCount)++;
        // new rule
        addChildAST(node, node1);
        addChildAST(node, node2);
        // printf("Rule 30 has no BT\n");
        break;
    }
    case 31:{
        // <moreFields> ===> ∈
        // can't assign null else next will be inaccessible
        // printf("Rule 31 has BT\n");
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        node1->next = node->next;
        *node = *node1;
        *insertUsed = true;
        // printf("Rule 31 has no BT\n");
        break;
    }
    case 32:{
        // <definetypestmt> ===> TK_DEFINETYPE <A> TK_RUID TK_AS TK_RUID
        // printf("Rule 32 has BT\n");
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        astNode *node2 = mknode(root->children[root->numChild-1-1]->elem,G);
        astNode *node3 = mknode(root->children[root->numChild-1-2]->elem,G);
        astNode *node4 = mknode(root->children[root->numChild-1-3]->elem,G);
        astNode *node5 = mknode(root->children[root->numChild-1-4]->elem,G);
        addChildAST(node,node1);
        addChildAST(node,node2);
        addChildAST(node,node3);
        addChildAST(node,node4);
        addChildAST(node,node5);
        // printf("Rule 32 has no BT\n");
        break;    
    }
    case 33:{
        // <declarations> ===> <declaration> <declarations1>
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        astNode *node2 = mknode(root->children[root->numChild-1-1]->elem,G);
        // old rule
        // Change 4: ignore declarations
        node1->next = node->next;
        *node = *node1;
        insertAST(node, node2);
        *insertUsed = true;
        (*insertCount)++;
        // new rule
        // addChildAST(node, node1);
        // addChildAST(node, node2);
        // printf("Testing stuff: \n");
        // astNode *temp = node;
        
        // while(temp!=NULL){
        //     printf("%s \n",G.nonTerminals[temp->elem->curr]);
        //     temp = temp->next;
        // }
        break;
    }
    case 34:{
        // <declarations> ===> ∈
        // can't assign null else next will be inaccessible
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        node1->next = node->next;
        *node = *node1;
        *insertUsed = true;
        break;
    }
    case 35:{
        // <declaration> ===> TK_TYPE <dataType> TK_COLON TK_ID <global_or_not> TK_SEM
        astNode *node1 = mknode(root->children[root->numChild-1-1]->elem,G);
        astNode *node2 = mknode(root->children[root->numChild-1-3]->elem,G);
        astNode *node3 = mknode(root->children[root->numChild-1-4]->elem,G);
        addChildAST(node,node1);
        addChildAST(node,node2);
        addChildAST(node,node3);
        break;        
    }
    case 36:{
        // <global_or_not> ===> TK_COLON TK_GLOBAL
        astNode *node1 = mknode(root->children[root->numChild-1-1]->elem,G);
        node1->next = node->next;
        *node = *node1;
        *insertUsed = true;
        break;
    }
    case 37:{
        // <global_or_not> ===> ∈
        // can't assign null else next will be inaccessible
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        node1->next = node->next;
        *node = *node1;
        *insertUsed = true;
        break;
    }
    case 38:{
        // <otherStmts> ===> <stmt> <otherStmts1>
        // printf("Rule 38 has BT\n");
        // Change 2: ignore otherStmts
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        astNode *node2 = mknode(root->children[root->numChild-1-1]->elem,G);
        // old rule
        node1->next = node->next;
        *node = *node1;
        insertAST(node,node2);
        *insertUsed = true;
        (*insertCount)++;
        // new rule
        // addChildAST(node, node1);
        // addChildAST(node, node2);
        // printf("Rule 38 has no BT\n");
        break;
    }
    case 39:{
        // <otherStmts> ===> ∈
        // can't assign null else next will be inaccessible
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        node1->next = node->next;
        *node = *node1;
        *insertUsed = true;
        // printf("Testing 39\n");
        // printf("%s\n",G.terminals[node->elem->curr]);
        break;
    }
    case 40:{
        // <stmt> ===> <assignmentStmt>
        astNode *node1  = mknode(root->children[root->numChild-1-0]->elem,G);
        node1->next = node->next;
        *node = *node1;
        *insertUsed = true;
        break;
    }
    case 41:{
        // <stmt> ===> <iterativeStmt>
        // printf("Rule 41 has BT\n");
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        node1->next = node->next;
        *node = *node1;
        *insertUsed = true;
        // printf("Rule 41 has no BT\n");
        break;
    }
    case 42:{
        // <stmt> ===> <conditionalStmt>
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        node1->next = node->next;
        *node = *node1;
        *insertUsed = true;
        break;
    }
    case 43:{
        // <stmt> ===> <ioStmt>
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        node1->next = node->next;
        *node = *node1;
        *insertUsed = true;
        break;
    }
    case 44:{
        // <stmt> ===> <funCallStmt>
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        node1->next = node->next;
        *node = *node1;
        *insertUsed = true;
        break;
    }
    case 45:{
        // <assignmentStmt> ===> <singleOrRecId> TK_ASSIGNOP <arithmeticExpression> TK_SEM
        // node = mknode(root->children[root->numChild-1-0]->elem,G);
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        astNode *node2 = mknode(root->children[root->numChild-1-1]->elem,G);
        astNode *node3 = mknode(root->children[root->numChild-1-2]->elem,G);
        addChildAST(node,node1);
        addChildAST(node,node2);
        addChildAST(node,node3);
        break;
    }
    case 46:{
        // <singleOrRecId> ===> TK_ID <option_single_constructed>
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        astNode *node2 = mknode(root->children[root->numChild-1-1]->elem,G);
        // addChildAST(node,node1);
        // addChildAST(node,node2);
        node1->next = node->next;
        *node = *node1;
        insertAST(node,node2);
        *insertUsed = true;
        (*insertCount)++;
        break;
    }
    case 47:{
        // <option_single_constructed> ===> <oneExpansion> <moreExpansions>
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        astNode *node2 = mknode(root->children[root->numChild-1-1]->elem,G);
        // old rule
        node1->next = node->next;
        *node = *node1;
        insertAST(node,node2);
        *insertUsed = true;
        (*insertCount)++;
        // new rule
        // addChildAST(node, node1);
        // addChildAST(node, node2);
        break;
    }
    case 48:{
        // <option_single_constructed> ===> ∈
        // can't assign null else next will be inaccessible
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        node1->next = node->next;
        *node = *node1;
        *insertUsed = true;
        break;
    }
    case 49:{
        // <oneExpansion> ===> TK_DOT TK_FIELDID
        astNode *node1 = mknode(root->children[root->numChild-1-1]->elem,G);
        node1->next = node->next;
        *node = *node1;
        *insertUsed = true;
        break;
    }
    case 50:{
        // <moreExpansions> ===> <oneExpansion> <moreExpansions1>
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        astNode *node2 = mknode(root->children[root->numChild-1-1]->elem,G);
        // old rule
        node1->next = node->next;
        *node = *node1;
        insertAST(node, node2);
        (*insertCount)++;
        *insertUsed = true;
        // new rule
        // addChildAST(node, node1);
        // addChildAST(node, node2);
        break;
    }
    case 51:{
        // <moreExpansions> ===> ∈
        // can't assign null else next will be inaccessible
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        node1->next = node->next;
        *node = *node1;
        *insertUsed = true;
        break;
    }
    case 52:{
        // <funCallStmt> ===> <outputParameters> TK_CALL TK_FUNID TK_WITH TK_PARAMETERS <inputParameters> TK_SEM
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        // astNode *node2 = mknode(root->children[root->numChild-1-1]->elem,G);
        astNode *node3 = mknode(root->children[root->numChild-1-2]->elem,G);
        // astNode *node4 = mknode(root->children[root->numChild-1-4]->elem,G);
        astNode *node5 = mknode(root->children[root->numChild-1-5]->elem,G);
        addChildAST(node,node1);
        // addChildAST(node,node2);
        addChildAST(node,node3);
        // addChildAST(node,node4);
        addChildAST(node,node5);
        break;
    }
    case 53:{
        // <outputParameters> ===> TK_SQL <idList> TK_SQR TK_ASSIGNOP
        astNode *node1 = mknode(root->children[root->numChild-1-1]->elem,G);
        astNode *node2 = mknode(root->children[root->numChild-1-3]->elem,G);
        addChildAST(node,node1);
        addChildAST(node,node2);
        // node1->next = node->next;
        // *node = *node1;
        // insertAST(node,node2);
        // *insertUsed = true;
        // (*insertCount)++;
        break;
    }
    case 54:{
        // <outputParameters> ===> ∈
        // can't assign null else next will be inaccessible
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        // node1->next = node->next;
        // *node = *node1;
        // *insertUsed = true;
        addChildAST(node, node1);
        break;
    }
    case 55:{
        // <inputParameters> ===> TK_SQL <idList> TK_SQR
        astNode *node1 = mknode(root->children[root->numChild-1-1]->elem,G);
        // node1->next = node->next;
        // *node = *node1;
        // *insertUsed = true;
        addChildAST(node, node1);
        break;
    }
    case 56:{
        // <iterativeStmt> ===> TK_WHILE TK_OP <booleanExpression> TK_CL <stmt> <otherStmts> TK_ENDWHILE
        astNode *node1 = mknode(root->children[root->numChild-1-2]->elem,G);
        astNode *node2 = mknode(root->children[root->numChild-1-4]->elem,G);
        astNode *node3 = mknode(root->children[root->numChild-1-5]->elem,G);
        addChildAST(node,node1);
        addChildAST(node,node2);
        addChildAST(node,node3);
        break;
    }
    case 57:{

        // <conditionalStmt> ===> TK_IF TK_OP <booleanExpression> TK_CL TK_THEN <stmt> <otherStmts> <elsePart>

        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        astNode *node2 = mknode(root->children[root->numChild-1-2]->elem,G);
        astNode *node3 = mknode(root->children[root->numChild-1-4]->elem,G);
        astNode *node4 = mknode(root->children[root->numChild-1-5]->elem,G);
        astNode *node5 = mknode(root->children[root->numChild-1-6]->elem,G);
        astNode *node6 = mknode(root->children[root->numChild-1-7]->elem,G);
        addChildAST(node, node1);
        addChildAST(node, node2);
        addChildAST(node, node3);
        addChildAST(node, node4);
        addChildAST(node, node5);
        addChildAST(node, node6);
        break;
    }
    case 58:{
        // <elsePart> ===> TK_ELSE <stmt> <otherStmts> TK_ENDIF
        astNode *node1 = mknode(root->children[root->numChild-1-1]->elem,G);
        astNode *node2 = mknode(root->children[root->numChild-1-2]->elem,G);
        astNode *node3 = mknode(root->children[root->numChild-1-3]->elem,G);
        addChildAST(node, node1);
        addChildAST(node, node2);
        addChildAST(node, node3);
        break;
    }
    case 59:{
        //<elsePart> ===> TK_ENDIF
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        node1->next = node->next;
        *node = *node1;
        *insertUsed = true;
        break;
    }
    case 60:{
        // <ioStmt> ===> TK_READ TK_OP <var> TK_CL TK_SEM
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        astNode *node2 = mknode(root->children[root->numChild-1-2]->elem,G);
        addChildAST(node, node1);
        addChildAST(node, node2);
        break;
    }
    case 61:{
        // <ioStmt> ===> TK_WRITE TK_OP <var> TK_CL TK_SEM
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        astNode *node2 = mknode(root->children[root->numChild-1-2]->elem,G);
        addChildAST(node, node1);
        addChildAST(node, node2);
        break;
    }
    case 62:{
        // <arithmeticExpression> ===> <term> <expPrime>
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        astNode *node2 = mknode(root->children[root->numChild-1-1]->elem,G);
        // old rule
        // node1->next = node->next;
        // *node = *node1;
        // insertAST(node, node2);
        // *insertUsed = true;
        // (*insertCount)++;
        // new rule
        addChildAST(node, node1);
        addChildAST(node, node2);
        break;
    }
    case 63:{
        // <expPrime> ===> <lowPrecedenceOperators> <term> <expPrime>
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        astNode *node2 = mknode(root->children[root->numChild-1-1]->elem,G);
        astNode *node3 = mknode(root->children[root->numChild-1-2]->elem,G);
        node1->next = node->next;
        *node = *node1;
        insertAST(node, node2);
        insertAST(node2, node3);
        *insertCount+=2;
        *insertUsed = true;
        // addChildAST(node, node1);
        // addChildAST(node, node2);
        // addChildAST(node, node3);
        break;
    }
    case 64:{
        // <expPrime> ===> ∈
        // can't assign null else next will be inaccessible
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        node1->next = node->next;
        *node = *node1;
        *insertUsed = true;
        break;
    }
    case 65:{
        // <term> ===> <factor> <termPrime>
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        astNode *node2 = mknode(root->children[root->numChild-1-1]->elem,G);
        // old rule
        node1->next = node->next;
        *node = *node1;
        insertAST(node, node2);
        *insertUsed = true;
        (*insertCount)++;
        // new rule
        // addChildAST(node, node1);
        // addChildAST(node, node2);
        break;
    }
    case 66:{
        // <termPrime> ===> <highPrecedenceOperators> <factor> <termPrime>
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        astNode *node2 = mknode(root->children[root->numChild-1-1]->elem,G);
        astNode *node3 = mknode(root->children[root->numChild-1-2]->elem,G);
        // old rule
        node1->next = node->next;
        *node = *node1;
        insertAST(node, node2);
        insertAST(node2, node3);
        *insertUsed = true;
        *insertCount+=2;
        // new rule
        // addChildAST(node, node1);
        // addChildAST(node, node2);
        // addChildAST(node, node3);
        break;
    }
    case 67:{
        // <termPrime> ===> ∈
        // can't assign null else next will be inaccessible
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        node1->next = node->next;
        *node = *node1;
        *insertUsed = true;
        break;
    }
    case 68:{
        // <factor> ===> TK_OP <arithmeticExpression> TK_CL
        astNode *node1 = mknode(root->children[root->numChild-1-1]->elem,G);
        node1->next = node->next;
        *node = *node1;
        *insertUsed = true;
        break;
    }
    case 69:{
        // <factor> ===> <var>
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        node1->next = node->next;
        *node = *node1;
        *insertUsed = true;
        break;
    }
    case 70:{
        // <highPrecedenceOperators> ===> TK_MUL
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        node1->next = node->next;
        *node = *node1;
        *insertUsed = true;
        break;
    }
    case 71:{
        // <highPrecedenceOperators> ===> TK_DIV 
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        node1->next = node->next;
        *node = *node1;
        *insertUsed = true;
        break;
    }
    case 72:{
        // <lowPrecedenceOperators> ===> TK_PLUS
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        node1->next = node->next;
        *node = *node1;
        *insertUsed = true;
        break;
    }
    case 73:{
        // <lowPrecedenceOperators> ===> TK_MINUS
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        node1->next = node->next;
        *node = *node1;
        *insertUsed = true;
        break;
    }
    case 74:{
        // <booleanExpression> ===> TK_OP <booleanExpression1> TK_CL <logicalOp> TK_OP <booleanExpression2> TK_CL
        astNode *node1 = mknode(root->children[root->numChild-1-1]->elem,G);
        astNode *node2 = mknode(root->children[root->numChild-1-3]->elem,G);
        astNode *node3 = mknode(root->children[root->numChild-1-5]->elem,G);
        addChildAST(node, node1);
        addChildAST(node, node2);
        addChildAST(node, node3);
        break;
    }
    case 75:{
        // <booleanExpression> ===> <var> <relationalOp> <var1>
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        astNode *node2 = mknode(root->children[root->numChild-1-1]->elem,G);
        astNode *node3 = mknode(root->children[root->numChild-1-2]->elem,G);
        // old rule
        // node1->next = node->next;
        // *node = *node1;
        // insertAST(node, node2);
        // insertAST(node2, node3);
        // *insertUsed = true;
        // *insertCount+=2;
        // new rule
        addChildAST(node, node1);
        addChildAST(node, node2);
        addChildAST(node, node3);
        break;
    }
    case 76:{
        // <booleanExpression> ===> TK_NOT TK_OP <booleanExpression1> TK_CL
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        astNode *node2 = mknode(root->children[root->numChild-1-2]->elem,G);
        // node1->next = node->next;
        // *node = *node1;
        // insertAST(node, node2);
        // *insertUsed = true;
        // (*insertCount)++;
        addChildAST(node, node1);
        addChildAST(node, node2);
        break;
    }
    case 77:{
        // <var> ===> <singleOrRecId>
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        node1->next = node->next;
        *node = *node1;
        *insertUsed = true;
        break;
    }
    case 78:{
        // <var> ===> TK_NUM
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        node1->next = node->next;
        *node = *node1;
        *insertUsed = true;
        break;
    }
    case 79:{
        // <var> ===> TK_RNUM
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        node1->next = node->next;
        *node = *node1;
        *insertUsed = true;
        break;
    }
    case 80:{
        // <logicalOp> ===> TK_AND
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        node1->next = node->next;
        *node = *node1;
        *insertUsed = true;
        break;
    }
    case 81:{
        // <logicalOp> ===> TK_OR
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        node1->next = node->next;
        *node = *node1;
        *insertUsed = true;
        break;
    }
    case 82:{
        // <relationalOp> ===> TK_LT
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        node1->next = node->next;
        *node = *node1;
        *insertUsed = true;
        break;
    }
    case 83:{
        // <relationalOp> ===> TK_LE
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        node1->next = node->next;
        *node = *node1;
        *insertUsed = true;
        break; 
    }
    case 84:{
        // <relationalOp> ===> TK_EQ
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        node1->next = node->next;
        *node = *node1;
        *insertUsed = true;
        break;
    }
    case 85:{
        // <relationalOp> ===> TK_GT
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        node1->next = node->next;
        *node = *node1;
        *insertUsed = true;
        break;
    }
    case 86:{
        // <relationalOp> ===> TK_GE
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        node1->next = node->next;
        *node = *node1;
        *insertUsed = true;
        break;
    }
    case 87:{
        // <relationalOp> ===> TK_NE
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        node1->next = node->next;
        *node = *node1;
        *insertUsed = true;
        break;
    }
    case 88:{
        // <returnStmt> ===> TK_RETURN <optionalReturn> TK_SEM
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        astNode *node2 = mknode(root->children[root->numChild-1-1]->elem,G);
        addChildAST(node, node1);
        addChildAST(node, node2);
        break;
    }
    case 89:{
        // <optionalReturn> ===> TK_SQL <idList> TK_SQR
        astNode *node1 = mknode(root->children[root->numChild-1-1]->elem,G);
        // node1->next = node->next;
        // *node = *node1;
        // *insertUsed = true;
        addChildAST(node, node1);
        break;
    }
    case 90:{
        // <optionalReturn> ===> ∈
        // can't assign null else next will be inaccessible
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        // node1->next = node->next;
        // *node = *node1;
        // *insertUsed = true;
        addChildAST(node, node1);
        break;
    }
    case 91:{
        // <idList> ===> TK_ID <more_ids>
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        astNode *node2 = mknode(root->children[root->numChild-1-1]->elem,G);
        // old rule
        node1->next = node->next;
        *node = *node1;
        insertAST(node, node2);
        *insertUsed = true;
        (*insertCount)++;
        // addChildAST(node, node1);
        // addChildAST(node, node2);
        break;
    }
    case 92:{
        // <more_ids> ===> TK_COMMA <idList>
        astNode *node1 = mknode(root->children[root->numChild-1-1]->elem,G);
        node1->next = node->next;
        *node = *node1;
        *insertUsed = true;
        break;
    }
    case 93:{
        // <more_ids> ===> ∈
        // can't assign null else next will be inaccessible
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        node1->next = node->next;
        *node = *node1;
        *insertUsed = true;
        break;
    }
    case 94:{
        // <A> ===> TK_RECORD
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        node1->next = node->next;
        *node = *node1;
        *insertUsed = true;
        break;
    }
    case 95:{
        // <A> ===> TK_UNION
        astNode *node1 = mknode(root->children[root->numChild-1-0]->elem,G);
        node1->next = node->next;
        *node = *node1;
        *insertUsed = true;
        break;
    }
    default:
        break;
    }
}

/*
    Traverses the AST inorder and prints the nodes
*/
void printAST(astNode *root, grammar G, int *count){
    if(root==NULL)return;
    if(root->child==NULL){
        if(root->elem->isLeaf && strcmp(G.terminals[root->elem->curr],"TK_RNUM")==0)
            printf("AST\tLeaf\t%0.2f\tLine No\t%d\n",root->elem->lex.rVal,root->elem->lineNo);
        else if(root->elem->isLeaf && strcmp(G.terminals[root->elem->curr],"TK_NUM")==0)
            printf("AST\tLeaf\t%d\tLine No\t%d\n",root->elem->lex.numVal,root->elem->lineNo);
        else {
            if(root->elem->isLeaf)
                printf("AST\tnode\t%s\n",G.terminals[root->elem->curr]);
            else
                printf("AST\tnode\t%s\n",G.nonTerminals[root->elem->curr]);
        }
        (*count)++;
        return;
    }
    astNode *temp = root->child;
    if(temp->next==NULL){
        printAST(temp,G,count);
        if(root->elem->isLeaf && strcmp(G.terminals[root->elem->curr],"TK_RNUM")==0)
            printf("AST\tLeaf\t%0.2f\tLine No\t%d\n",root->elem->lex.rVal,root->elem->lineNo);
        else if(root->elem->isLeaf && strcmp(G.terminals[root->elem->curr],"TK_NUM")==0)
            printf("AST\tLeaf\t%d\tLine No\t%d\n",root->elem->lex.numVal,root->elem->lineNo);
        else {
            if(root->elem->isLeaf)
                printf("AST\tnode\t%s\n",G.terminals[root->elem->curr]);
            else
                printf("AST\tnode\t%s\n",G.nonTerminals[root->elem->curr]);
        }
        (*count)++;
        return;
    }
    else{
        while(temp->next->next!=NULL){
            printAST(temp,G,count);
            temp = temp->next;
        }
        printAST(temp,G,count);
        if(root->elem->isLeaf && strcmp(G.terminals[root->elem->curr],"TK_RNUM")==0)
            printf("AST\tLeaf\t%0.2f\tLine No\t%d\n",root->elem->lex.rVal,root->elem->lineNo);
        else if(root->elem->isLeaf && strcmp(G.terminals[root->elem->curr],"TK_NUM")==0)
            printf("AST\tLeaf\t%d\tLine No\t%d\n",root->elem->lex.numVal,root->elem->lineNo);
        else {
            if(root->elem->isLeaf)
                printf("AST\tnode\t%s\n",G.terminals[root->elem->curr]);
            else
                printf("AST\tnode\t%s\n",G.nonTerminals[root->elem->curr]);
        }
        printAST(temp->next,G,count);
        (*count)++;
        return;
    }
}

/* 
int main(){
    char* file; 
    file = "grammar.txt"; 
    grammar C; 
    C = readGrammar(file); 
    printf("Grammar read and saved \n"); 
    printf("%d %d %d %d \n", C.totalNumRules, C.allRules[36].numOrs, C.allRules[31].epsilon, C.allRules[28].epsilon); 
    
    // printf("'%s' %d %d \n", C.nonTerminals[23], C.allRules[23].numOrs, C.allRules[23].epsilon); 
    // printf("%d %d '%s' '%s' \n", C.allRules[23].RHS[0].symbols[0].symbol, C.allRules[23].RHS[1].symbols[0].symbol, C.nonTerminals[C.allRules[23].RHS[0].symbols[0].symbol], C.terminals[C.allRules[23].RHS[1].symbols[0].symbol]); 

    // prettyPrintGrammar(C);
    
    C.ff = ComputeFirstAndFollowSets(C); 
    printf("First and Follow computed \n"); 
    printf("\n***** \n"); 
    // int n = 48; 
    // printf("C.ff[n].numFirst %d  C.ff[n].numFollow %d \n", C.ff[n].numFirst[0], C.ff[n].numFollow); 
    // for (int i = 0; i < C.ff[n].numFollow; i++) { 
    //     printf("%d '%s' \n", C.ff[n].follow[i], C.terminals[C.ff[n].follow[i]]); 
    // } 
    
    printf("\n***** \n"); 

    // int trial = 10; 
    // // printf("%d %d %d %d %d %d \n", C.allRules[23].numOrs, C.ff[24].numFirst[0], C.ff[24].numFirst[1], C.ff[24].numFirst[2], C.ff[24].numFirst[3], C.ff[24].numFirst[4]); 
    // printf("C.allRules[trial].numOrs %d ", C.allRules[trial].numOrs); 
    // for (int i = 0; i < C.allRules[trial].numOrs; i++) { 
    //     printf("C.ff[trial].numFirst[i] %d ", C.ff[trial].numFirst[i]); 
    // } 

    // printRule(C,trial,-1);
    
    parseTable* T = intializeParseTable(C.numNonTerminals,C.numTerminals);
    createParseTable(C,C.ff,T);
    // printParseTable(C,T);
    printf("Parse Table created \n"); 

    // printf("%d %d %d '%s' '%s' \n", C.allRules[2].numOrs, C.ff[2].numFirst[0], C.ff[2].numFirst[1], C.terminals[C.ff[2].first[0][0]], C.terminals[C.ff[2].first[1][0]]); 
    // printf("%d %d %d \n", C.allRules[0].numOrs, C.allRules[0].RHS[0].numSyms, C.allRules[0].RHS[0].symbols[1].type); 
    // printf("%d %d %d %d %d %d '%s' '%s' '%s' '%s' \n", C.ff[23].numFirst, C.ff[23].numFollow, C.ff[23].follow[0], C.ff[23].follow[1], C.ff[23].follow[2], C.ff[23].follow[3], C.terminals[C.ff[23].follow[0]], C.terminals[C.ff[23].follow[1]], C.terminals[C.ff[23].follow[2]], C.terminals[C.ff[23].follow[3]]); 

    char* testCaseFile = "./stage 2 (typechecking testcases for expressions)/p3.txt"; 
    // FILE *fp = fopen("./testcases_stage1/t2.txt","r"); 
    // initialize();
    // fp = getStream(fp, 0);
    // tokenInfo currToken = getNextToken(fp); 
    // printf("%d '%s' \n", findIndex(C.terminals, C.numTerminals, enumToStringP[currToken.tkn_name]), enumToStringP[currToken.tkn_name]); 
    treeN rootNode; 
    rootNode = parseInputSourceCode(testCaseFile, C, T); 
    // printf("%u %d %d %d %d %d \n", &rootNode, rootNode.elem.curr, rootNode.numChild, rootNode.elem.lineNo, rootNode.elem.isLeaf, rootNode.elem.parentNodeSymbolID); 
    // printf("%u %u \n", rootNode.children[0], rootNode.children[1]); 
    // printf("%u %u \n", rootNode.children[1]->children[1], rootNode.children[1]->children[1]->children[5]); 
    // printf("%d '%s' %d '%s' \n", rootNode.children[1]->children[1], C.nonTerminals[rootNode.children[1]->children[1]->elem.curr], rootNode.children[1]->children[1]->children[5], C.terminals[rootNode.children[1]->children[1]->children[5]->elem.curr]); 
    int *count1 = (int *)malloc(sizeof(int));
    *count1 = 0;
    printParseTree(&rootNode,"op.txt",C,count1);
    int *insertPrev = (int *)malloc(sizeof(int));
    *insertPrev = 0;
    int *count = (int *)malloc(sizeof(int));
    *count = 0;
    astNode *astroot = mknode(rootNode.elem,C);
    constructAst(astroot, &rootNode,C,insertPrev,astroot);
    printf("Parse Tree Number of nodes = %d. Allocated Memory = %lu bytes\n",*count1,sizeof(treeN)*(*count1));
    printf("*************************************************************************************************\n\n");
    printf("Printing Abstract Syntax Tree in Inorder Traversal\n");
    printAST(astroot,C, count);
    printf("AST Number of nodes = %d. Allocated Memory = %lu bytes\n\n",*count,sizeof(astNode)*(*count));
    float p1 = sizeof(treeN)*(*count1);
    float p2 = sizeof(astNode)*(*count);
    float compressionPercentage = ((p1 - p2)/p1)*100;
    printf("Compression percentage %f\n",compressionPercentage);
    printf("*************************************************************************************************\n\n");
    printf("Level 1 printing\n");
    printf("Root : isLeaf: %d curr: %d name: %s Line: %d \n",astroot->elem->isLeaf,astroot->elem->curr,astroot->elem->isLeaf?C.terminals[astroot->elem->curr]:C.nonTerminals[astroot->elem->curr],astroot->elem->lineNo);
    astNode *curr = astroot->child;
    while(curr!=NULL){
        printf("isLeaf: %d curr: %d name:%s Line:%d\n",curr->elem->isLeaf,curr->elem->curr,curr->elem->isLeaf?C.terminals[curr->elem->curr]:C.nonTerminals[curr->elem->curr],curr->elem->lineNo);
        curr = curr->next;
    }
    // printf("isLeaf: %d curr: %d name: %s\n",astroot->child->elem->isLeaf,astroot->child->elem->curr,astroot->child->elem->isLeaf?C.terminals[astroot->child->elem->curr]:C.nonTerminals[astroot->child->elem->curr]);
    // printf("rule number %d LHS %s RHS %s\n",getRuleNumber(52,1,C),C.nonTerminals[C.allRules[52].LHS],C.terminals[C.allRules[52].RHS[1].symbols[0].symbol]);
}

*/