struct tilda_marker {
    char filepath[1024];
    size_t tilda_pos;
};

int tilda_marker_pos;
struct tilda_marker tilda_marker_arr[1024];

char BUFFER[BUFSIZ];

void copy_everything(char* src_dir_name, char* dest_dir_name);

void replace_tildas(char* name);
