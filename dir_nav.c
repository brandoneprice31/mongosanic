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
