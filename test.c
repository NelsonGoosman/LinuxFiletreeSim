#include <stdio.h>
#include "utils.h"
#include "filetree.h"
// some simple tests to confirm my utils functions work (do not compile with main, not meant to be part of project)
int main(){
    char* validpath1 = "/this/is/a/valid/path";
    char* validpath2 = "/this/is/a/valid/path.txt";
    char* invalidpath1 = "/this/is/an/invalid /path.txt";
    char* invalidpath2= "/this/is/an/invalid.txt/path.txt";
    char* relativepath = "this/is/a/relative/path.txt";
    char* absolutepath = "/this/is/an/absolute/path.txt";
    char* correctpathitems[5] = {"this", "is", "a", "valid", "path"};
    char* file = "filetest.txt";
    char* directory = "DirectoryTest";

    // all tests should return 1
    int validpathtest1 = isvalidpath(validpath1); 
    int validpathtest2 = isvalidpath(validpath2); 
    int invalidpathtest1 = !isvalidpath(invalidpath1);  
    int invalidpathtest2 = !isvalidpath(invalidpath2); 
    int absolutepathtest = isabsolutepath(absolutepath); 
    int relativepathtest = !isabsolutepath(relativepath); 
    char** correctpathitemstest = getpathitems(validpath1); 
    char** correctpathitemstest2 = getpathitems("one_item_test"); 
    int filetest = isfile(file);
    int directorytest = !isfile(directory);

    printf("Running unit tests\n \n");
    int i = 0;
    while (correctpathitemstest[i] != NULL) { 
        printf("%s - ", correctpathitemstest[i]);
        i++;
    }
    freestrarr(correctpathitemstest);
    printf("\n");

    i = 0;
    while (correctpathitemstest2[i] != NULL) { 
        printf("%s - ", correctpathitemstest2[i]);
        i++;
    }
    freestrarr(correctpathitemstest2);
    printf("\n");

    int testresults[8] = {validpathtest1, validpathtest2, invalidpathtest1, invalidpathtest2, absolutepathtest, 
                            relativepathtest, filetest, directorytest};

    const int num_tests = 8;

    for (int i = 0; i < num_tests; i++){
        if (testresults[i] == 1){
            printf("test %d / 8 passed ", i+1);
        }else{
            printf("test %d / 8 failed ", i+1);
        }
    }
    
    return 0;
}