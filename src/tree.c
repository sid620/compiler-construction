#include "tree.h"
#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

node createEl(int lineNo, int parentSymbolID, int symbolID, int isLeafNode) { 
    node new; 
    new.lineNo = lineNo; 
    new.parentNodeSymbolID = parentSymbolID; 
    new.curr = symbolID; 
    new.isLeaf = isLeafNode; 

    if (isLeafNode == -1) { 
        new.lex.lexemeStr = "____"; 
    } 
    return new; 
}

tree_n add_node(node el)
{
    tree_n new_node;

    new_node.next = NULL;
    new_node.child = NULL; 
    new_node.num_child=0;
    new_node.visit=0;
    new_node.sib_index=0; 
    new_node.elem.curr = el.curr; 
    new_node.elem.isLeaf = el.isLeaf; 
    new_node.elem.lex = el.lex; 
    new_node.elem.lineNo = el.lineNo; 
    new_node.elem.parentNodeSymbolID = el.parentNodeSymbolID; 

    printf("%d \n", new_node.elem.curr);
    return new_node;
}

tree_n add_child(tree_n* parent, node el)
{ 
    if (parent == NULL) return *parent;
    if (parent -> child){
        tree_n *temp= (tree_n *)malloc(sizeof(tree_n ));
        temp=parent->child;
        tree_n *sib= (tree_n *)malloc(sizeof(tree_n ));
        *sib= add_node(el);
        parent->child=sib; 
        sib->next=temp;
        parent->num_child++;
        return *sib;
    }
    else{
        parent->num_child++;
        parent->child = (tree_n *)malloc(sizeof(tree_n));
        *(parent->child) = add_node(el);
        return *(parent->child);
    }
        
}

//Function to add child override with different parameters
// void add_child(tree_n *parent, tree_n *new_child) {
//     if (parent == NULL||new_child==NULL)
//         return NULL;

//     else
//         parent->child=new_child;

// }

tree_n add_sibling(tree_n* sib, tree_n* parent, node el)
{
    if( sib == NULL) return *sib;
    while(sib->next) sib = sib->next;
    parent->num_child++;
    sib->next = (tree_n *)malloc(sizeof(tree_n));
    *(sib->next) = add_node(el);
    return *(sib->next);

}


// tree_n* remove_node(tree_n* parent, tree_n* prev_sib,tree_n* node)
// {   
//     if(node==parent->child){
//         parent->child=node->next;
//         parent->num_child--;
//     }
//     else if(prev_sib != NULL)
//         prev_sib->next=node->next;

//     free(node);
// }


//   if (prev != NULL)
//     return prev->sibling;
//   else
//     return parent->leftmost_child;
    
// tree_n *node__by_index(tree_n *root, int index){
// 	if((root == NULL) || (index > root->num_child))
// 		return NULL;
	
// 	tree_n *temp = root->child;

// 	for(int i = 1; i < index; i++){		// loop tells how many child u have to go ahead
// 		temp = temp->next;
// 	}
// 	return temp;
// }



// //Function to add child override with different parameters
// void add_child(tree_n *parent, tree_n *child) {

//   if (parent->rightmost_child == NULL) {
//     parent->leftmost_child = child;
//     parent->rightmost_child = child;
//   } else {
//     parent->rightmost_child->sibling = child;
//     parent->rightmost_child = child;
//   }
//   parent->num_child++;
//   child->parent = parent;
//   child->sibling = NULL;
//   if(child->sym.is_terminal == false){
//     if(parent->line_nums.start == 0)
//       parent->line_nums.start = child->line_nums.start;
//     if(child->line_nums.end != 0)
//       parent->line_nums.end = child->line_nums.end;
//   }
// }
