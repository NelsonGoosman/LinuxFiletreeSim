#include <stdio.h>            
#include <stdlib.h>
#include <string.h>
#include "filetree.h"   
#include "utils.h"        
// additional headers as needed
//todo: rename all functions/variables to c style guide standards

/*
to run: make clean (if executable already exists), 
*/


NODE *root = NULL;
NODE *cwd = NULL;
char *cmd_list[] = {"mkdir", "rmdir", "ls", "cd", "pwd", "creat", "rm",
"reload", "save", "quit", 0};
char cwd_str[64] = "/";
// other global variables



int main() {
	// other initialization as needed
	initialize(&root, &cwd);
	list_cmds(cmd_list);
	while(1) {
		printf("Enter command: ");
		char cmd_buffer[100];
		fgets(cmd_buffer, 100, stdin);

		// Remove newline character
		//NOTE: Change to getline when running on virtual box
		cmd_buffer[strcspn(cmd_buffer, "\n")] = 0;

		// Split into command and path
		//TODO: further input checking
		char* cmd = strtok(cmd_buffer, " ");
		char* path = strtok(NULL, "\n");
		int cmd_idx = find_command(cmd, cmd_list);
		int validpathflag = isvalidpath(path);

		if (cmd_idx > -1 && (validpathflag == 1 || (cmd_idx == 4 || cmd_idx == 9 || cmd_idx == 2 || cmd_idx == 3))){
			int pathtypeflag = isabsolutepath(path);
			switch (cmd_idx){
				case 0:
					if (pathtypeflag == 1){
						mkdir(root, path);
					}else{
						mkdir(cwd, path);
					}
					break;
				case 1:
					if (pathtypeflag == 1){
						rmdir(root, path);
					}else{
						rmdir(cwd, path);
					}
					break;
				case 2:
					if (validpathflag != 1){ //if path not specified
						ls(cwd, NULL);
					}else if (pathtypeflag == 1){
						ls(root, path);
					}else{
						ls(cwd, path);
					}
					break;
				case 3:
					if (validpathflag == 1){
						cd(&cwd, path, cwd_str);
					}else{
						cwd = root;
						strcpy(cwd_str, "/");
					}
					break;
				case 4:
					pwd(cwd_str);
					break;
				case 5:
					if (pathtypeflag == 1){
						creat(root, path);
					}else{
						creat(cwd, path);
					}
					break;
				case 6:
					if (pathtypeflag == 1){
						rm(root, path);
					}else{
						rm(cwd, path); 
					}
					break;
				case 7:
					reload(&root, &cwd, path);
					break;
				case 8:
					save_tree(root, path); //in this case, path is a file name
					break;
				case 9:
					quit(root, cwd);
					break;
				default:
					break;
			}

			//switch case cmd idx
		}else if (validpathflag == 0){ 
			printf("Invalid path\n");
		}else{
			printf("Invalid command \n");
		}

	}
	return 0;
}

