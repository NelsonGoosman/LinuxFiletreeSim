#include "utils.h"

char** getpathitems(char* path){

    // caution: strtok modifies path. Also array must be freed at some point
    char *path_cpy = strdup(path);
    char **path_items = malloc(100 * sizeof(char*));
    char* token = strtok(path_cpy, "/");
    int idx = 0;
    while (token != NULL){
        path_items[idx] = (char*)malloc(strlen(token) + 1);
        strcpy(path_items[idx], token);
        token = strtok(NULL, "/");
        idx++;
    }
    path_items[idx] = NULL;
    free(path_cpy);
    return path_items;
}



int isvalidpath(char* path){
    if (path == NULL || strlen(path) == 0) return 0;
    for (int i = 0; i < strlen(path); i++){
        char ch = path[i];
        // if char is not number or is space or not forward slash, not valid
        if (!isalnum(ch) && ch != '/' && ch != '-' && ch != '.'){
            printf("Not valid path! The path must only contain numbers, letters, and foreward slashes, no spaces are allowed.\n");
            return 0;
        }
    }

    //check to make sure path is either all directories or all directories ending in a file
    char** path_items = getpathitems(path);
    int i = 0;
    while (path_items[i]){
        if (isfile(path_items[i]) && path_items[i+1] != NULL) return 0;
        i++;
    }
    freestrarr(path_items);
    return 1;
}

int isabsolutepath(char* path){
    return path != NULL && path[0] == '/';
}

int isfile(char* str){
    for (int i = 0; i < strlen(str); i++){
        if (str[i] == '.') return 1;
    }
    return 0;
}

void freestrarr(char** arr){
    if (arr == NULL) return;
    int i = 0;
    while (arr[i] != NULL) { 
        free(arr[i]);
        i++;
    }
    free(arr);
}

int find_command(char *user_command, char** cmd_list){
	int i = 0;
	while(cmd_list[i]){
		if (strcmp(user_command, cmd_list[i]) == 0)
		return i;
		i++;
	}
	return -1;
}

void list_cmds(char** cmd_list){
	int i = 0;
	printf("The available commands are: ");
	while(cmd_list[i]){
		printf((cmd_list[i+1] != 0) ? "%s, " : "%s\n", cmd_list[i]);
		i++;
	}
}

int getftypefrompath(char* path){
    char** path_items = getpathitems(path);
    char last_path_item[64] = "";
    int i = 0;
    while(path_items[i]){
        if (path_items[i+1] == NULL){
            strcpy(last_path_item,path_items[i]);
        }
        i++;
    }
    freestrarr(path_items);
    return isfile(last_path_item);
}

NODE* findnode(NODE* start, char* path){
    // finds the file or directory specified in the path
    char** path_items = getpathitems(path);
    NODE* current = start -> child;
    int i = 0;
    while (path_items[i]){
        while (current != NULL && strcmp(current->name, path_items[i]) != 0){
            current = current -> sibling;
        }
        if (current == NULL){ //match not found
            freestrarr(path_items);
            return NULL;
        }else if (strcmp(current->name, path_items[i]) == 0 && path_items[i+1] == NULL){ //match found at end of path 
            freestrarr(path_items);
            return current;
        }else{ //match found mid path, go one directory deeper
            current = current -> child;
            i++;
        }
    }
    freestrarr(path_items);
    return NULL; // catch all null return
}

NODE* findprevnode(NODE* start, char* path){
    //find the file or directory before the file specified by the path for deleting purposes
    char** path_items = getpathitems(path);
    NODE* current = start -> child;
    NODE* prev = start;
    int i = 0;
    while (path_items[i]){
        while (current != NULL && strcmp(current->name, path_items[i]) != 0){
            prev = current;
            current = current -> sibling;
        }
        if (current == NULL){ //match not found
            freestrarr(path_items);
            return NULL;
        }else if (strcmp(current->name, path_items[i]) == 0 && path_items[i+1] == NULL){ //match found at end of path 
            freestrarr(path_items);
            return prev;
        }else{ //match found mid path, go one directory deeper
            prev = current;
            current = current -> child;
            i++;
        }
    }
    freestrarr(path_items);
    return NULL; // catch all null return
}


void remove_whitespace(char* str) {
    char* p = str;
    while (isspace((unsigned char)*p)) {
        p++;
    }
    if (p != str) {
        while (*p) {
            *str++ = *p++;
        }
        *str = '\0'; 
    }
}
