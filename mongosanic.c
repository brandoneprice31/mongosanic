#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
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

    // Iterate over all files in mongosanic-api-src
    tilda_marker_pos = 0;
    copy_everything("./mongosanic-api-src", directory);

    // Replace valid ~~name~~ with argv[1]
    int tilda_1;
    int tilda_2;
    int tilda_3;
    int tilda_4;
    for (int i=0; i<tilda_marker_pos-3; i++) {
        if  ((strcmp(tilda_marker_arr[i].filepath, tilda_marker_arr[i+1].filepath)) ||
            (strcmp(tilda_marker_arr[i+1].filepath, tilda_marker_arr[i+2].filepath)) ||
            (strcmp(tilda_marker_arr[i+2].filepath, tilda_marker_arr[i+3].filepath)))
            continue;

        tilda_1 = tilda_marker_arr[i].tilda_pos;
        tilda_2 = tilda_marker_arr[i+1].tilda_pos;
        tilda_3 = tilda_marker_arr[i+2].tilda_pos;
        tilda_4 = tilda_marker_arr[i+3].tilda_pos;
        if ((tilda_1+1 != tilda_2) || (tilda_3+1 != tilda_4)) {
            continue;
        }

        FILE* tilda_file = fopen(tilda_marker_arr[i].filepath, "r");
        fseek(tilda_file, 0L, SEEK_END);
        size_t file_sz = ftell(tilda_file);
        rewind(tilda_file);

        char buf[file_sz];
        fread(buf, 1, file_sz, tilda_file);
        fclose(tilda_file);
        remove(tilda_marker_arr[i].filepath);

        tilda_file = fopen(tilda_marker_arr[i].filepath, "w");
        fwrite(buf, 1, tilda_1, tilda_file);
        fwrite(argv[1], 1, strlen(argv[1]), tilda_file);
        fwrite(&buf[tilda_4+1], 1, file_sz - tilda_4-1, tilda_file);

        for (int j=i+1; j<tilda_marker_pos; j++) {
            if (!strcmp(tilda_marker_arr[i].filepath, tilda_marker_arr[j].filepath)) {
                tilda_marker_arr[j].tilda_pos -= (tilda_4 - tilda_1 + 1) - strlen(argv[1]);
            }
        }

        i+=3;
        fclose(tilda_file);
    }
}
