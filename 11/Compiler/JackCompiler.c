#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <libgen.h>
#include "JackTokenizer.c"
#include "CompilationEngine.c"



void process_file(char* in_file_name, char* in_dirname);

int main(int argc, char *argv[])
{
    int opt;
    char filename[200];

    if (argc != 2) {
	fprintf(stderr, "usage: JackCompiler filepath\n");
	exit(1);
    }

    // stat library provides file status info
    struct stat file_info;
    strncpy(filename, argv[1], 200);
    stat(filename, &file_info);

    if(S_ISDIR(file_info.st_mode)) {
        printf("dir: %s\n", argv[1]);
        struct dirent *dir;
        DIR *d = opendir(filename);
        while ((dir = readdir(d)) != NULL)
        {
            if (strstr(dir->d_name, ".jack") != NULL) {
                printf("file: %s\n", dir->d_name);
                process_file(dir->d_name, argv[1]);
            }

        }
    } else {
        if(S_ISREG(file_info.st_mode)) {
            printf("file: %s\n", argv[1]);
            process_file(basename(filename), dirname(filename));
        } else {
            fprintf(stderr, "Invalid file name: %s\n", filename);
            exit(1);
        }

    }


    return 0;
}

void process_file(char* in_file_name, char* in_dirname) {

    // printf("%s: %zu\n", in_file_name, strlen(in_file_name));
    // declare out file path with enough space for .vm
    char* out_file_name = malloc(strlen(in_file_name) - 2);
        
    //take jack off end of in file name and copy the rest to out file name
    strncpy(out_file_name, in_file_name, strlen(in_file_name) - 4);

    //concatonate vm to the out file path
    strcat(out_file_name, "vm");
    // printf("without .jack: %s\n", out_file_path);

    // join directory path and new file name
    size_t len_dir_path = strlen(in_dirname);
    size_t len_file_name = strlen(out_file_name);
    char* out_file_path = malloc(len_dir_path + len_file_name + 1);
    if (in_dirname[len_dir_path - 1] != '/') {
        strcat(in_dirname, "/");
    }
    out_file_path = strcat(in_dirname, out_file_name);
    // printf("%s\n", out_file_path);


    FILE tokenizer = read_file(strcat(in_dirname, in_file_name));
    tokenizer_execute();

    // CompilationEngine compilationEngine = new CompilationEngine(tokenizer, out_file_path)
    // compilationEngine.compileClass()
}
