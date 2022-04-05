#pragma once
#include "ast.h"
#include<stdio.h>
#include<stdlib.h>
#include "astDef.h"
astNode *mknode(node *elem){
    astNode *new = (astNode *)malloc(sizeof(astNode));
    new->next=NULL;
    new->child=NULL;
    new->numChildren=0;
    new->elem=(node *)malloc(sizeof(node));
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
