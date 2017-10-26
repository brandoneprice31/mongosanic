#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include "dir_nav.h"

void copy_everything(char* src_dir_name, char* dest_dir_name) {
    DIR* src_dir = opendir((const char*) src_dir_name);

    struct dirent* in_file;
    while ((in_file = readdir(src_dir))) {
        struct stat info;

        // build in_file path
        char in_file_path[sizeof(src_dir_name) + sizeof("/") + sizeof(in_file->d_name)];
        sprintf(in_file_path, "%s/%s", src_dir_name, in_file->d_name);

        // Make dest file path.
        char dest_file_path[sizeof(dest_dir_name) + sizeof("/") + sizeof(in_file->d_name)];
        sprintf(dest_file_path, "%s/%s", dest_dir_name, in_file->d_name);

        // use stat to see if its a dir or file
        stat(in_file_path, &info);
        if(S_ISDIR(info.st_mode)) {
            // Skip parent and current directories.
            if (!strcmp (in_file->d_name, "."))
                continue;
            if (!strcmp (in_file->d_name, ".."))
                continue;
            if (!strcmp (in_file->d_name, "venv"))
                continue;
                if (!strcmp (in_file->d_name, "__pycache__"))
                    continue;

            // mkdir
            mkdir(dest_file_path, 0700);

            // cp everything in this dir
            copy_everything(in_file_path, dest_file_path);

        } else {
            // not a directory
            FILE* curr_file = fopen(in_file_path, "r");

            // Open copy of in file.
            FILE* out_file = fopen(dest_file_path, "w");

            // Copy in file into out file.
            size_t bytes_in = 0;
            size_t sz_read;
            while ((sz_read = fread(BUFFER, 1, 256, curr_file))) {

                // mark the ~
                for (int i=0; i<= (int) sz_read; i++) {
                    if (BUFFER[i] == '~') {
                        memcpy(tilda_marker_arr[tilda_marker_pos].filepath, dest_file_path, strlen(dest_file_path));
                        tilda_marker_arr[tilda_marker_pos].tilda_pos = bytes_in + i;
                        tilda_marker_pos++;
                    }
                }

                fwrite(BUFFER, 1, sz_read, out_file);
                bytes_in+=sz_read;
            }

            fclose(curr_file);
            fclose(out_file);
        }
    }

    closedir(src_dir);
}

void replace_tildas(char* name) {
    // Replace valid ~~name~~ with name
    int tilda_1;
    int tilda_2;
    int tilda_3;
    int tilda_4;

    // Iterate over every tilda.
    for (int i=0; i<tilda_marker_pos-3; i++) {
        // If this set of four tilda's don't come from the same file then goto next.
        if  ((strcmp(tilda_marker_arr[i].filepath, tilda_marker_arr[i+1].filepath)) ||
            (strcmp(tilda_marker_arr[i+1].filepath, tilda_marker_arr[i+2].filepath)) ||
            (strcmp(tilda_marker_arr[i+2].filepath, tilda_marker_arr[i+3].filepath)))
            continue;

        tilda_1 = tilda_marker_arr[i].tilda_pos;
        tilda_2 = tilda_marker_arr[i+1].tilda_pos;
        tilda_3 = tilda_marker_arr[i+2].tilda_pos;
        tilda_4 = tilda_marker_arr[i+3].tilda_pos;

        // Ensure that this is a valid tilda posititoning.
        if ((tilda_1+1 != tilda_2) || (tilda_3+1 != tilda_4)) {
            continue;
        }

        // Open the copied file and get it's size.
        FILE* tilda_file = fopen(tilda_marker_arr[i].filepath, "r");
        fseek(tilda_file, 0L, SEEK_END);
        size_t file_sz = ftell(tilda_file);
        rewind(tilda_file);

        // Read file into buffer and then delete it.
        char buf[file_sz];
        fread(buf, 1, file_sz, tilda_file);
        fclose(tilda_file);
        remove(tilda_marker_arr[i].filepath);

        // Open new file with same name and copy everything except for the ~'s
        // and add the new name.
        tilda_file = fopen(tilda_marker_arr[i].filepath, "w");
        fwrite(buf, 1, tilda_1, tilda_file);
        fwrite(name, 1, strlen(name), tilda_file);
        fwrite(&buf[tilda_4+1], 1, file_sz - tilda_4-1, tilda_file);

        // Shift the stored tilda positions to account for the new file.
        for (int j=i+1; j<tilda_marker_pos; j++) {
            if (!strcmp(tilda_marker_arr[i].filepath, tilda_marker_arr[j].filepath)) {
                tilda_marker_arr[j].tilda_pos -= (tilda_4 - tilda_1 + 1) - strlen(name);
            }
        }

        // Goto set of next four and close the file.
        i+=3;
        fclose(tilda_file);
    }
}
