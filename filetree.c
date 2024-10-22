#include "filetree.h"
#include "utils.h"

int initialize(NODE** root, NODE** cwd) {
	*root = (NODE *)malloc(sizeof(NODE));
    if (*root == NULL) {
        printf("Could not allocate space in make_node");
        exit(1);
    }
    
    // Set the root node's properties
    strcpy((*root)->name, "/");
    (*root)->parent = *root;  // Root's parent is itself
    (*root)->sibling = 0;
    (*root)->child = 0;
    (*root)->type = 'D';
    
    *cwd = *root;  
    
    printf("Filesystem initialized!\n");
    return 0;  
}

NODE* make_node(const char* name, char type){
	// requitements: must validate that type is either D or F
	NODE* new_node = (NODE *)malloc(sizeof(NODE));
	if (new_node == NULL){
		printf("Could not allocate space in make_node");
		return NULL;
	} 
	strcpy(new_node->name, name);
	new_node->parent = 0;
	new_node->sibling = 0;
	new_node->child = 0;
	new_node->type = type;
	return new_node;
}

void mkdir(NODE* root_dir, char* path){
	int ftype = getftypefrompath(path);
	if (ftype == 1){
		printf("Cannot use create command to make a file\n");
		return;
	}
	char** path_items = getpathitems(path);
	NODE* current = root_dir, *prev = root_dir, *parent_dir = root_dir; // only need prev for the parent pointer
	int i = 0;
	while(path_items[i]){
		char ftype  = (isfile(path_items[i]) == 1) ? 'F' : 'D';
		if (current->child == NULL){ 
			NODE* new_node = make_node(path_items[i], ftype);
			current->child = new_node;
			new_node ->parent = current;
			current = new_node;
			prev = new_node;
		}else {
			//basically inserting into end of linked list
			//going down a directory, set the parent pointer
			parent_dir = current;
			current = current->child;
			while (current != NULL && strcmp(current->name, path_items[i]) != 0) {
				prev = current;
				current = current->sibling;
			}

			if (current != NULL && strcmp(current->name, path_items[i]) == 0) { // found item in path

				if (path_items[i+1] == NULL){ // if its the end of the path, path already exists

					freestrarr(path_items);
					printf("Directory %s already exists\n", path);
					return; //path already exists
				} 

				current = prev; // back up a node
				prev = current; // set prev to current
				parent_dir = current;
				current = current -> child; // go down a directory
			} else if (current == NULL) {
				// Insert the node here since the sibling does not exist
				NODE* new_node = make_node(path_items[i], ftype);
				prev->sibling = new_node;
				new_node -> parent = parent_dir;
			}
		}
		i++;
	}

	freestrarr(path_items);
}

void ls(NODE* root, char* path){
	NODE* current = NULL;
	if (path != NULL){ //find path if specified, otherwise start from cwd
		NODE* target_dir = findnode(root, path);
		if (target_dir == NULL) return;
		current = target_dir -> child;
	}else{
		current = root ->child;
	}
	if (current == NULL) printf("Directory is empty");
	while (current != NULL){
			printf("(%c) %s ", current->type, current->name);
			current = current -> sibling;
	}
	printf("\n");
}

void cd(NODE** root_dir, char* path, char* cwd_str){
		if (strcmp(path, "..") == 0 && strcmp((*root_dir) -> name, "/")==0){
			//cant go up a directory from root
			return;
		}
		if (strcmp(path, "..") == 0 && strcmp((*root_dir) -> name, "/") !=0){
			(*root_dir) = (*root_dir) -> parent;
			strcpy(cwd_str, path);
			return;
		}
		NODE* target_dir = findnode(*root_dir, path);
		if (target_dir == NULL){
			printf("Directory Not found\n");
		}else{
			(*root_dir) = target_dir;
			strcpy(cwd_str, path);
		}
}

void pwd(char* cwd_str){
	printf("Current working directory: %s\n", cwd_str);
}

void rm(NODE* root_dir, char* path){
	NODE* prevNode = findprevnode(root_dir, path);
	if (prevNode == NULL){
		printf("File does not exist\n");
		return;
	}
	char** path_items = getpathitems(path);
	int i = 0;
	char search_item[128] = "";
	//find name of node to delete
	while (path_items[i] != NULL){
		if (path_items[i+1] == NULL){
			strcpy(search_item, path_items[i]);
			break;
		}
		i++;
	}
	freestrarr(path_items);
	if (!isfile(search_item)){
		printf("Error, cannot delete directory using rm command\n");
		return;
	}
	NODE* delete_node = 0;
	if (prevNode -> child != NULL && strcmp(prevNode -> child -> name, search_item) == 0){
		delete_node = prevNode -> child;
		prevNode -> child = delete_node -> sibling;
		if (delete_node -> sibling){
			delete_node -> parent = prevNode;
		}
		free(delete_node);
	}else if ( prevNode -> sibling != NULL && strcmp(prevNode -> sibling -> name, search_item ) == 0){
		delete_node = prevNode -> sibling;
		prevNode -> sibling = delete_node -> sibling;
		free(delete_node);
	}else{
		printf("Error in rm: unable to delete file\n");
	}
}

void creat(NODE* root_dir, char* path){
	int ftype = getftypefrompath(path);
	if (ftype == 0){
		printf("Cannot use create command to make a directory\n");
		return;
	}

	char** path_items = getpathitems(path);
	NODE* current = root_dir, *prev = root_dir; // only need prev for the parent pointer
	int i = 0;
	while(path_items[i]){
		char ftype  = (isfile(path_items[i]) == 1) ? 'F' : 'D';
		if (current->child == NULL){ //
			NODE* new_node = make_node(path_items[i], ftype);
			current->child = new_node;
			new_node ->parent = current;
			current = new_node;
			prev = new_node;
		}else {
			//basically inserting into end of linked list
			current = current->child;
			while (current != NULL && strcmp(current->name, path_items[i]) != 0) {
				prev = current;
				current = current->sibling;
			}

			if (current != NULL && strcmp(current->name, path_items[i]) == 0) {
				if (path_items[i+1] == NULL){
					freestrarr(path_items);
					printf("Directory %s already exists\n", path);
					return; //path already exists
				} 
				current = prev;
				prev = current;
				current = current -> child;
			} else if (current == NULL) {
				// Insert the node here since the sibling does not exist
				NODE* new_node = make_node(path_items[i], ftype);
				prev->sibling = new_node;
			}
		}
		i++;
	}

	freestrarr(path_items);
}


void rmdir(NODE* root_dir, char* path){
	NODE* prevNode = findprevnode(root_dir, path);
	if (prevNode == NULL){
		printf("Directory does not exist\n");
		return;
	}
	char** path_items = getpathitems(path);
	int i = 0;
	char search_item[128] = "";
	//find name of node to delete
	while (path_items[i] != NULL){
		if (path_items[i+1] == NULL){
			strcpy(search_item, path_items[i]);
			break;
		}
		i++;
	}
	freestrarr(path_items);
	if (isfile(search_item)){
		printf("Error, cannot delete file using rmdir command\n");
		return;
	}
	NODE* delete_node = 0;
	if (prevNode -> child != NULL && strcmp(prevNode -> child -> name, search_item) == 0){
		delete_node = prevNode -> child;
		if (delete_node -> child != NULL){
			printf("Error: cannot delete a non empty directory\n");
			return;
		}
		prevNode -> child = delete_node -> sibling;
		if (delete_node -> sibling){
			delete_node -> parent = prevNode;
		}
		free(delete_node);
	}else if ( prevNode -> sibling != NULL && strcmp(prevNode -> sibling -> name, search_item ) == 0){
		delete_node = prevNode -> sibling;
		if (delete_node -> child != NULL){
			printf("Error: cannot delete a non empty directory\n");
			return;
		}
		prevNode -> sibling = delete_node -> sibling;
		free(delete_node);
	}else{
		printf("Error in rm: unable to delete file\n");
	}
}

void save_tree(NODE* root, char* fname){
	FILE *fp = fopen(fname, "w+"); // open a file stream
	if (fp == NULL){
		printf("Error saving file\n");
	}else{
		char print_path[64];
		strcpy(print_path, "");
		save_helper(root, fp, print_path);
		fclose(fp);
	}
}

void save_helper(NODE* root, FILE* file, char* prev_path) {
    if (root == NULL) return;

    char current_path[128];
    if (strlen(prev_path) == 0) {
        snprintf(current_path, sizeof(current_path), "%s", root->name);
    } else if (strcmp(prev_path, "/") == 0){
        snprintf(current_path, sizeof(current_path), "%s%s", prev_path, root->name);
	}else {
        snprintf(current_path, sizeof(current_path), "%s/%s", prev_path, root->name);
    }

    // Write the current path to the file
    fprintf(file, "%c  %s\n", root->type == 'F' ? 'F' : 'D', current_path);
    if (root->child != NULL) save_helper(root->child, file, current_path);
    if (root->sibling != NULL) save_helper(root->sibling, file, prev_path);
}

void tree_destructor(NODE* root) {
    if (root == NULL) return;

    tree_destructor(root -> child);

    tree_destructor(root->sibling);

    free(root);
}


void quit(NODE* root, NODE* cwd){
	cwd = root;
	save_tree(root, "fssim_goosman.txt");
	tree_destructor(root);
	exit(0);
}


void reload(NODE** root, NODE** cwd, char* fname){
	FILE *fp = fopen(fname, "r"); // open a file stream
	if (fp){
		tree_destructor(*root);
		initialize(root, cwd);
		char line[256];
		fgets(line, sizeof(line), fp); //ignore first line
		while(fgets(line, sizeof(line), fp)){
			
			char* type = strtok(line, " ");
			char* path = strtok(NULL, "\n");
			path[strcspn(path, "\n")] = 0;
			remove_whitespace(path);
			if (type[0] == 'D'){
				mkdir(*root, path);
			}else{
				creat(*root, path);
			}
		}
	}else{
		printf("Could not open file %s\n", fname);
	}
	
}

