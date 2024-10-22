#ifndef UTILS
#define UTILS

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "filetree.h"


int isvalidpath(char* path); // tests if the path is valid by checking if there are no files in the middle of the path
int isabsolutepath(char* path); // check if path starts with /
char** getpathitems(char* path); // returns an array of strings where each index is the path item. Ex /a/b/c returns [a,b,c]
int isfile(char* str); //returns true if string contains a '.'
void freestrarr(char** arr); // frees the string array allocated in get path items
int find_command(char *user_command, char** cmd_list); // checks the command against the command list, returns -1 if not found otherwise the index
void list_cmds(char** cmd_list); // prints all commands
int getftypefrompath(char* path); // gets the type (either file or directory) from the path by looking at the last path item
NODE* findnode(NODE* start, char* path); // finds a node given a path, starting from either cwd or root. Returns NULL if not found
NODE* findprevnode(NODE* start, char* path); // same as above but finds the node before the one specified by path for deletion purposes
void remove_whitespace(char* str); // removed leading whitespace from a string

#endif