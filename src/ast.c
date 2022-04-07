#pragma once
#include "ast.h"
#include<stdio.h>
#include<stdlib.h>
#include "astDef.h"
#include "lexerDef.h"
astNode *mknode(node elem){
    astNode *new = (astNode *)malloc(sizeof(astNode));
    new->next=NULL;
    new->child=NULL;
    new->numChildren=0;
    new->elem=(node *)malloc(sizeof(node));
    new->elem->curr = elem.curr;
    new->elem->isLeaf = elem.isLeaf;
    new->elem->lineNo = elem.lineNo;
    new->elem->parentNodeSymbolID = elem.parentNodeSymbolID;
    new->elem->ruleNumber = elem.ruleNumber;
    return new;
}
void addChild(astNode *parent, astNode *child){
    if(parent->child==NULL){
        parent->child = (astNode *)malloc(sizeof(astNode));
        parent->child = child;
    }
    else{
        astNode *temp;
        temp = parent->child;
        while(temp->next!=NULL)temp = temp->next;
        temp->next  = child;
        parent->numChildren++;
    }
}
void insert(astNode *curr, astNode *sibling){
    astNode *temp = curr->next;
    curr->next = sibling;
    sibling->next = temp;
}
bool isUseful(int tokenID){
    switch(tokenID){
        case TK_FUNID: {
            return true;
        } 
        case TK_INPUT:{
            return true;
        }
        case TK_OUTPUT:{
            return true;
        }
        case TK_ID:{
            return true;
        }
        case TK_INT:{
            return true;
        }
        case TK_REAL:{
            return true;
        }
        case TK_RECORD:{
            return true;
        }
        case TK_UNION:{
            return true;
        }
        case TK_RUID:{
            return true;
        }
        case TK_GLOBAL:{
            return true;
        }
        case TK_IF:{
            return true;
        }
        case TK_THEN:{
            return true;
        }
        case TK_PLUS:{
            return true;
        }
        case TK_MUL:{
            return true;
        }
        case TK_DIV:{
            return true;
        }
        case TK_MINUS:{
            return true;
        }
        default:{
            return false;
        }
    }
}
astNode *findChild(astNode *node, int position){
    astNode * temp = node;
    int j = 0;
    while(j<position){
        temp = temp->next;
        j++;
    }
    return temp;
}
void constructAst(astNode *node, treeN *root){
    if(!root)return;
    performAction(root->elem.ruleNumber,node,root);
    int j = 0;
    for(int i = 0;i < root->numChild;i++){
        if(root->elem.ruleNumber!=0 || (root->elem.isLeaf && isUseful(root->elem.curr))){
            constructAst(findChild(node,j),root->children[i]);
            j++;
        }
    }
}
void performAction(int ruleNumber, astNode *node, treeN *root){
    switch (ruleNumber){
    case 1:{
        //<program> ===> <otherFunctions> <mainFunction>
        // make  node for <otherFunctions> and <mainFunction>
        astNode *node1 = mknode(root->children[0]->elem);
        astNode *node2 = mknode(root->children[1]->elem);
        addChild(node, node1);
        addChild(node, node2);
        break;
    }
    case 2:{
        //<mainFunction> ===> TK_MAIN <stmts> TK_END
        // make node for <stmts>
        astNode *node1 = mknode(root->children[1]->elem);
        node = node1;
        break;
    }
    case 3:{
        //<otherFunctions> ===> <function> <otherFunctions1>
        // make node for <function> and <otherFunctions1>
        astNode *node1 = mknode(root->children[0]->elem);
        astNode *node2 = mknode(root->children[1]->elem);
        // insert
        node = node1;
        insert(node, node2);
        break;
    }
    case 4:{
        //<otherFunctions> ===> ∈
        // otherFunctions --> epsilon
        // can't assign null else next will be inaccessible
        node = mknode(root->children[0]->elem);
        break;
    }
    case 5:{
        //<function> ===> TK_FUNID <input_par> <output_par> TK_SEM <stmts> TK_END
        // create node for FUNID, input_par, output_par, and stmts
        astNode *node1 = mknode(root->children[0]->elem);
        astNode *node2 = mknode(root->children[1]->elem);
        astNode *node3 = mknode(root->children[2]->elem);
        astNode *node4 = mknode(root->children[4]->elem);
        // Add children
        addChild(node, node1);
        addChild(node, node2);
        addChild(node, node3);
        addChild(node, node4);
        break;
        
    }
    case 6: {
        // <input_par> ===> TK_INPUT TK_PARAMETER TK_LIST TK_SQL <parameter_list> TK_SQR
        // mknode for TK_INPUT and <parameter_list>
        astNode *node1 = mknode(root->children[0]->elem);
        astNode *node2 = mknode(root->children[4]->elem);
        node = node1;
        insert(node, node2);
        break;
    }
    case 7: {
        //<output_par> ===> TK_OUTPUT TK_PARAMETER TK_LIST TK_SQL <parameter_list> TK_SQR
        astNode *node1 = mknode(root->children[0]->elem);
        astNode *node2 = mknode(root->children[4]->elem);
        node = node1;
        insert(node, node2);
        break;
    }
    case 8:{
        //<parameter_list> ===> <dataType> TK_ID <remaining_list>
        astNode *node1 = mknode(root->children[0]->elem);
        astNode *node2 = mknode(root->children[1]->elem);
        astNode *node3 = mknode(root->children[2]->elem);
        addChild(node,node1);
        addChild(node,node2);
        addChild(node,node3);
        break;
    }
    case 9:{
        //<dataType> ===> <primitiveDatatype> 
        astNode *node1 = mknode(root->children[0]->elem);
        node = node1;
        break; 
    }
    case 10:{
        //<dataType> ===> <constructedDatatype>
        astNode *node1 = mknode(root->children[0]->elem);
        node = node1;
        break;
    }
    case 11:{
        //<primitiveDatatype> ===> TK_INT
        astNode *node1 = mknode(root->children[0]->elem);
        node = node1;
        break;
    }
    case 12:{
        //<primitiveDatatype> ===> TK_REAL
        astNode *node1 = mknode(root->children[0]->elem);
        node = node1;
        break;
    }
    case 13:{
        //<constructedDatatype> ===> TK_RECORD TK_RUID
        astNode *node1 = mknode(root->children[0]->elem);
        astNode *node2 = mknode(root->children[1]->elem);
        addChild(node, node1);
        addChild(node, node2);
        break;
    }
    case 14:{
        //<constructedDatatype> ===> TK_UNION TK_RUID
        astNode *node1 = mknode(root->children[0]->elem);
        astNode *node2 = mknode(root->children[1]->elem);
        addChild(node, node1);
        addChild(node, node2);
        break;
    }
    case 15:{
        //<constructedDatatype> ===> TK_RUID
        astNode *node1 = mknode(root->children[0]->elem);
        node = node1;
        break;
    }
    case 16:{
        //<remaining_list> ===> TK_COMMA <parameter_list>
        astNode *node1 = mknode(root->children[0]->elem);
        node = node1;
        break;
    }
    case 17:{
        //<remaining_list> ===> ∈
        // can't assign null else next will be inaccessible
        node = mknode(root->children[0]->elem);
        break;
    }
    case 18:{
        //<stmts> ===> <typeDefinitions> <declarations> <otherStmts> <returnStmt>
        astNode *node1 = mknode(root->children[0]->elem);
        astNode *node2 = mknode(root->children[1]->elem);
        astNode *node3 = mknode(root->children[2]->elem);
        astNode *node4 = mknode(root->children[3]->elem);
        node = node1;
        insert(node, node2);
        insert(node2, node3);
        insert(node3, node4);
        break;
    }
    case 19:{
        //<typeDefinitions> ===> <actualOrRedefined> <typeDefinitions>
        astNode *node1 = mknode(root->children[0]->elem);
        astNode *node2 = mknode(root->children[1]->elem);
        node = node1;
        insert(node, node2);
        break;
    }
    case 20:{
        //<typeDefinitions> ===> ∈
        // can't assign null else next will be inaccessible
        node = mknode(root->children[0]->elem);
        break;
    }
    case 21:{
        //<actualOrRedefined> ===> <typeDefinition>
        astNode *node1 = mknode(root->children[0]->elem);
        node = node1;
        break;
    }
    case 22:{
        //<actualOrRedefined> ===> <definetypestmt>
        astNode *node1 = mknode(root->children[1]->elem);
        node = node1;
        break;
    }
    case 23:{
        //<typeDefinition> ===> TK_RECORD TK_RUID <fieldDefinitions> TK_ENDRECORD
        astNode *node1 = mknode(root->children[0]->elem);
        astNode *node2 = mknode(root->children[1]->elem);
        astNode *node3 = mknode(root->children[2]->elem);
        node = node1;
        addChild(node, node2);
        addChild(node, node3);
        break;
    }
    case 24:{
        //<typeDefinition> ===> TK_UNION TK_RUID <fieldDefinitions> TK_ENDUNION
        astNode *node1 = mknode(root->children[0]->elem);
        astNode *node2 = mknode(root->children[1]->elem);
        astNode *node3 = mknode(root->children[2]->elem);
        node = node1;
        addChild(node, node2);
        addChild(node, node3);
        break;        
    }
    case 25:{
        //<fieldDefinitions> ===> <fieldDefinition> <fieldDefinition1> <moreFields>
        astNode *node1 = mknode(root->children[0]->elem);
        astNode *node2 = mknode(root->children[1]->elem);
        astNode *node3 = mknode(root->children[2]->elem);
        node = node1;
        insert(node, node2);
        insert(node2, node3);
        break;
    }
    case 26:{
        //<fieldDefinition> ===> TK_TYPE <fieldType> TK_COLON TK_FIELDID TK_SEM
        astNode *node1 = mknode(root->children[1]->elem);
        astNode *node2 = mknode(root->children[3]->elem);
        addChild(node, node1);
        addChild(node, node2);
        break;
    }
    case 27:{
        // <fieldType> ===> <primitiveDatatype>
        astNode *node1 = mknode(root->children[0]->elem);
        node = node1;
        break;
    }
    case 28:{
        // <fieldType> ===> TK_RUID
        astNode *node1 = mknode(root->children[0]->elem);
        node = node1;
        break;
    }
    case 29:{
        //<moreFields> ===> <fieldDefinition> <moreFields1>
        astNode *node1 = mknode(root->children[0]->elem);
        astNode *node2 = mknode(root->children[1]->elem);
        node = node1;
        insert(node, node2);
        break;
    }
    case 30:{
        // <moreFields> ===> ∈
        // can't assign null else next will be inaccessible
        node = mknode(root->children[0]->elem);
        break;
    }
    case 31:{
        // <definetypestmt> ===> TK_DEFINETYPE <A> TK_RUID TK_AS TK_RUID
        astNode *node1 = mknode(root->children[0]->elem);
        astNode *node2 = mknode(root->children[1]->elem);
        astNode *node3 = mknode(root->children[2]->elem);
        astNode *node4 = mknode(root->children[3]->elem);
        astNode *node5 = mknode(root->children[4]->elem);
        node = node1;
        addChild(node,node2);
        addChild(node,node3);
        addChild(node,node4);
        addChild(node,node5);
        break;    
    }
    case 32:{
        // <declarations> ===> <declaration> <declarations1>
        astNode *node1 = mknode(root->children[0]->elem);
        astNode *node2 = mknode(root->children[1]->elem);
        node = node1;
        insert(node, node2);
        break;
    }
    case 33:{
        // <declarations> ===> ∈
        // can't assign null else next will be inaccessible
        node = mknode(root->children[0]->elem);
        break;
    }
    case 34:{
        // <declaration> ===> TK_TYPE <dataType> TK_COLON TK_ID <global_or_not> TK_SEM
        astNode *node1 = mknode(root->children[1]->elem);
        astNode *node2 = mknode(root->children[3]->elem);
        astNode *node3 = mknode(root->children[4]->elem);
        addChild(node,node1);
        addChild(node,node2);
        addChild(node,node3);
        break;        
    }
    case 35:{
        // <global_or_not> ===> TK_COLON TK_GLOBAL
        astNode *node1 = mknode(root->children[1]->elem);
        node = node1;
        break;
    }
    case 36:{
        // <global_or_not> ===> ∈
        // can't assign null else next will be inaccessible
        node = mknode(root->children[0]->elem);
        break;
    }
    case 37:{
        // <otherStmts> ===> <stmt> <otherStmts1>
        astNode *node1 = mknode(root->children[0]->elem);
        astNode *node2 = mknode(root->children[1]->elem);
        node = node1;
        insert(node,node2);
        break;
    }
    case 38:{
        // <otherStmts> ===> ∈
        // can't assign null else next will be inaccessible
        node = mknode(root->children[0]->elem);
        break;
    }
    case 39:{
        // <stmt> ===> <assignmentStmt>
        node  = mknode(root->children[0]->elem);
        break;
    }
    case 40:{
        // <stmt> ===> <iterativeStmt>
        node = mknode(root->children[0]->elem);
        break;
    }
    case 41:{
        // <stmt> ===> <conditionalStmt>
        node = mknode(root->children[0]->elem);
        break;
    }
    case 42:{
        // <stmt> ===> <ioStmt>
        node = mknode(root->children[0]->elem);
        break;
    }
    case 43:{
        // <stmt> ===> <funCallStmt>
        node = mknode(root->children[0]->elem);
        break;
    }
    case 44:{
        // <assignmentStmt> ===> <singleOrRecId> TK_ASSIGNOP <arithmeticExpression> TK_SEM
        node = mknode(root->children[0]->elem);
        astNode *node1 = mknode(root->children[0]->elem);
        astNode *node2 = mknode(root->children[1]->elem);
        astNode *node3 = mknode(root->children[2]->elem);
        addChild(node,node1);
        addChild(node,node2);
        addChild(node,node3);
        break;
    }
    case 45:{
        // <singleOrRecId> ===> TK_ID <option_single_constructed>
        astNode *node1 = mknode(root->children[0]->elem);
        astNode *node2 = mknode(root->children[1]->elem);
        addChild(node,node1);
        addChild(node,node2);
        break;
    }
    case 46:{
        // <option_single_constructed> ===> <oneExpansion> <moreExpansions>
        astNode *node1 = mknode(root->children[0]->elem);
        astNode *node2 = mknode(root->children[1]->elem);
        node = node1;
        insert(node,node2);
        break;
    }
    case 47:{
        // <option_single_constructed> ===> ∈
        // can't assign null else next will be inaccessible
        node = mknode(root->children[0]->elem);
        break;
    }
    case 48:{
        // <oneExpansion> ===> TK_DOT TK_FIELDID
        node = mknode(root->children[1]->elem);
        break;
    }
    case 49:{
        // <moreExpansions> ===> <oneExpansion> <moreExpansions1>
        astNode *node1 = mknode(root->children[0]->elem);
        astNode *node2 = mknode(root->children[1]->elem);
        node = node1;
        insert(node, node2);
        break;
    }
    case 50:{
        // <moreExpansions> ===> ∈
        // can't assign null else next will be inaccessible
        node = mknode(root->children[0]->elem);
        break;
    }
    case 51:{
        // <funCallStmt> ===> <outputParameters> TK_CALL TK_FUNID TK_WITH TK_PARAMETERS <inputParameters> TK_SEM
        astNode *node1 = mknode(root->children[0]->elem);
        astNode *node2 = mknode(root->children[1]->elem);
        astNode *node3 = mknode(root->children[2]->elem);
        astNode *node4 = mknode(root->children[4]->elem);
        astNode *node5 = mknode(root->children[5]->elem);
        addChild(node,node1);
        addChild(node,node2);
        addChild(node,node3);
        addChild(node,node4);
        addChild(node,node5);
        break;
    }
    case 52:{
        // <outputParameters> ===> TK_SQL <idList> TK_SQR TK_ASSIGNOP
        astNode *node1 = mknode(root->children[1]->elem);
        addChild(node,node1);
        break;
    }
    case 53:{
        // <outputParameters> ===> ∈
        // can't assign null else next will be inaccessible
        node = mknode(root->children[0]->elem);
        break;
    }
    case 54:{
        // <inputParameters> ===> TK_SQL <idList> TK_SQR
        astNode *node1 = mknode(root->children[1]->elem);
        node = node1;
        break;
    }
    case 55:{
        // <iterativeStmt> ===> TK_WHILE TK_OP <booleanExpression> TK_CL <stmt> <otherStmts> TK_ENDWHILE
        astNode *node1 = mknode(root->children[2]->elem);
        astNode *node2 = mknode(root->children[4]->elem);
        astNode *node3 = mknode(root->children[5]->elem);
        addChild(node,node1);
        addChild(node,node2);
        addChild(node,node3);
        break;
    }
    case 56:{
        astNode *node1 = mknode(root->children[0]->elem);
        astNode *node2 = mknode(root->children[2]->elem);
        astNode *node3 = mknode(root->children[4]->elem);
        astNode *node4 = mknode(root->children[5]->elem);
        astNode *node5 = mknode(root->children[6]->elem);
        astNode *node6 = mknode(root->children[7]->elem);
        addChild(node, node1);
        addChild(node, node2);
        addChild(node, node3);
        addChild(node, node4);
        addChild(node, node5);
        addChild(node, node6);
        break;
    }
    case 57:{
        // <elsePart> ===> TK_ELSE <stmt> <otherStmts> TK_ENDIF
        astNode *node1 = mknode(root->children[1]->elem);
        astNode *node2 = mknode(root->children[2]->elem);
        addChild(node, node1);
        addChild(node, node2);
        break;
    }
    case 58:{
        //<elsePart> ===> TK_ENDIF
        astNode *node1 = mknode(root->children[0]->elem);
        node = node1;
        break;
    }
    case 59:{
        // <ioStmt> ===> TK_READ TK_OP <var> TK_CL TK_SEM
        astNode *node1 = mknode(root->children[0]->elem);
        astNode *node2 = mknode(root->children[2]->elem);
        addChild(node, node1);
        addChild(node, node2);
        break;
    }
    case 60:{
        // <ioStmt> ===> TK_WRITE TK_OP <var> TK_CL TK_SEM
        astNode *node1 = mknode(root->children[0]->elem);
        astNode *node2 = mknode(root->children[2]->elem);
        addChild(node, node1);
        addChild(node, node2);
        break;
    }
    case 61:{
        // <arithmeticExpression> ===> <term> <expPrime>
        astNode *node1 = mknode(root->children[0]->elem);
        astNode *node2 = mknode(root->children[1]->elem);
        node = node1;
        insert(node, node2);
        break;
    }
    case 62:{
        // <expPrime> ===> <lowPrecedenceOperators> <term> <expPrime>
        astNode *node1 = mknode(root->children[0]->elem);
        astNode *node2 = mknode(root->children[1]->elem);
        astNode *node3 = mknode(root->children[2]->elem);
        node = node1;
        insert(node, node2);
        insert(node2, node3);
        break;
    }
    case 63:{
        // <expPrime> ===> ∈
        // can't assign null else next will be inaccessible
        node = mknode(root->children[0]->elem);
        break;
    }
    case 64:{
        // <term> ===> <factor> <termPrime>
        astNode *node1 = mknode(root->children[0]->elem);
        astNode *node2 = mknode(root->children[1]->elem);
        node = node1;
        insert(node, node2);
        break;
    }
    case 65:{
        // <termPrime> ===> <highPrecedenceOperators> <factor> <termPrime>
        astNode *node1 = mknode(root->children[0]->elem);
        astNode *node2 = mknode(root->children[1]->elem);
        astNode *node3 = mknode(root->children[2]->elem);
        node = node1;
        insert(node, node2);
        insert(node2, node3);
        break;
    }
    case 66:{
        // <termPrime> ===> ∈
        // can't assign null else next will be inaccessible
        node = mknode(root->children[0]->elem);
        break;
    }
    case 67:{
        // <factor> ===> TK_OP <arithmeticExpression> TK_CL
        node = mknode(root->children[1]->elem);
        break;
    }
    case 68:{
        // <factor> ===> <var>
        node = mknode(root->children[0]->elem);
        break;
    }
    case 69:{
        // <highPrecedenceOperators> ===> TK_MUL
        node = mknode(root->children[0]->elem);
        break;
    }
    case 70:{
        // <highPrecedenceOperators> ===> TK_DIV 
        node = mknode(root->children[0]->elem);
        break;
    }
    case 71:{
        // <lowPrecedenceOperators> ===> TK_PLUS
        node = mknode(root->children[0]->elem);
        break;
    }
    case 72:{
        // <lowPrecedenceOperators> ===> TK_MINUS
        node = mknode(root->children[0]->elem);
        break;
    }
    case 73:{
        // <booleanExpression> ===> TK_OP <booleanExpression1> TK_CL <logicalOp> TK_OP <booleanExpression2> TK_CL
        astNode *node1 = mknode(root->children[1]->elem);
        astNode *node2 = mknode(root->children[3]->elem);
        astNode *node3 = mknode(root->children[5]->elem);
        addChild(node, node1);
        addChild(node, node2);
        addChild(node, node3);
        break;
    }
    case 74:{
        // <booleanExpression> ===> <var> <relationalOp> <var1>
        astNode *node1 = mknode(root->children[0]->elem);
        astNode *node2 = mknode(root->children[1]->elem);
        astNode *node3 = mknode(root->children[2]->elem);
        node = node1;
        insert(node, node2);
        insert(node2, node3);
        break;
    }
    case 75:{
        // <booleanExpression> ===> TK_NOT TK_OP <booleanExpression1> TK_CL
        node = mknode(root->children[0]->elem);
        astNode *node1 = mknode(root->children[2]->elem);
        insert(node, node1);
        break;
    }
    case 76:{
        // <var> ===> <singleOrRecId>
        node = mknode(root->children[0]->elem);
        break;
    }
    case 77:{
        // <var> ===> TK_NUM
        node = mknode(root->children[0]->elem);
        break;
    }
    case 78:{
        // <var> ===> TK_RNUM
        node = mknode(root->children[0]->elem);
        break;
    }
    case 79:{
        // <logicalOp> ===> TK_AND
        node = mknode(root->children[0]->elem);
        break;
    }
    case 80:{
        // <logicalOp> ===> TK_OR
        node = mknode(root->children[0]->elem);
        break;
    }
    case 81:{
        // <relationalOp> ===> TK_LT
        node = mknode(root->children[0]->elem);
        break;
    }
    case 82:{
        // <relationalOp> ===> TK_LE
        node = mknode(root->children[0]->elem);
        break; 
    }
    case 83:{
        // <relationalOp> ===> TK_EQ
        node = mknode(root->children[0]->elem);
        break;
    }
    case 84:{
        // <relationalOp> ===> TK_GT
        node = mknode(root->children[0]->elem);
        break;
    }
    case 85:{
        // <relationalOp> ===> TK_GE
        node = mknode(root->children[0]->elem);
        break;
    }
    case 86:{
        // <relationalOp> ===> TK_NE
        node = mknode(root->children[0]->elem);
        break;
    }
    case 87:{
        // <returnStmt> ===> TK_RETURN <optionalReturn> TK_SEM
        astNode *node1 = mknode(root->children[0]->elem);
        astNode *node2 = mknode(root->children[1]->elem);
        addChild(node, node1);
        addChild(node, node2);
        break;
    }
    case 88:{
        // <optionalReturn> ===> TK_SQL <idList> TK_SQR
        astNode *node1 = mknode(root->children[1]->elem);
        node1->next=node->next;
        node = node1;
        break;
    }
    case 89:{
        // <optionalReturn> ===> ∈
        // can't assign null else next will be inaccessible
        node = mknode(root->children[0]->elem);
        break;
    }
    case 90:{
        // <idList> ===> TK_ID <more_ids>
        astNode *node1 = mknode(root->children[0]->elem);
        astNode *node2 = mknode(root->children[1]->elem);
        node = node1;
        insert(node, node2);
        break;
    }
    case 91:{
        // <more_ids> ===> TK_COMMA <idList>
        node = mknode(root->children[1]->elem);
        break;
    }
    case 92:{
        // <more_ids> ===> ∈
        // can't assign null else next will be inaccessible
        node = mknode(root->children[0]->elem);
        return;
    }
    case 93:{
        // <A> ===> TK_RECORD
        node = mknode(root->children[0]->elem);
        break;
    }
    case 94:{
        // <A> ===> TK_UNION
        node = mknode(root->children[0]->elem);
        break;
    }
    default:
        break;
    }
}