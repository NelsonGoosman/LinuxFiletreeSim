#ifndef FILETREE
#define FILETREE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/*
For each node, childPtr points to the oldest child, and siblingPtr points to the oldest sibling. Each node
also has a parentPtr pointing to its parent node. The name and type fields are used to record the name of
the node and corresponding type (either DIR or FILE). For the root node, both parentPtr and siblingPtr
point to itself.
*/

typedef struct node {
	char  name[64];   
	char  type;
	struct node *child, *sibling, *parent;
} NODE;




//Command functions 
void mkdir(NODE* root_dir, char* path); 
void rmdir(NODE* root_dir, char* path); 
void ls(NODE* root, char* path); 
void cd(NODE** root_dir, char* path, char* cwd_str); 
void pwd(char* cwd_str); 
void creat(NODE* root_dir, char* path);
void rm(NODE* root_dir, char* path);
void reload(NODE** root, NODE** cwd, char* fname);
void quit(NODE* root, NODE* cwd); 

//Tree functions
int initialize(NODE** root, NODE** cwd); // init tree
NODE* make_node(const char* name, char type); // makes a new node
void tree_destructor(NODE* root); // deletes the tree recursively
void save_tree(NODE* root, char* fname); //saves to file
void save_helper(NODE* root, FILE* file, char* prev_path);

#endif