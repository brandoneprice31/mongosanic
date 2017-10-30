#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <libgen.h>
#include "dir_nav.h"

int main(int argc, char *argv[]){
    if (argc != 2) {
        printf("mongosanic takes in 1 command line argument\n");
        return 0;
    }

    char directory[strlen("./") + strlen(argv[1])];
    sprintf(directory, "./%s", argv[1]);

    // Make directory if it doesn't exist already.
    struct stat st = {0};
    if (stat(directory, &st) != -1) {
        printf("%s directory already exists\n", argv[1]);
        return 0;
    }
    mkdir(directory, 0700);

    // Get path to mongosanic-api-src
    char src_path[1024];
    sprintf(src_path, "%s/mongosanic-api-src", dirname(argv[0]));

    copy_everything(src_path, directory);
    replace_tildas(argv[1]);

    printf("Created %s mongo-sanic api.\n", argv[1]);
}
