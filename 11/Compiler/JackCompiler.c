#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>
#include <libgen.h>
#include <assert.h>
#include "CompilationEngine.h"
#include "VMWriter.h"


void process_file(char* in_file_name, char* in_dirname);

int main(int argc, char *argv[])
{
    char *filename;

    if (argc != 2) {
      fprintf(stderr, "usage: JackCompiler filepath\n");
      exit(1);
    }

    // stat library provides file status info
    struct stat file_info;
    filename = argv[1];

    stat(filename, &file_info);

    if(S_ISDIR(file_info.st_mode)) {
        printf("dir: %s\n", filename);
        struct dirent *dir;
        DIR *d = opendir(filename);

        if(!d) {
            perror(filename);
            exit(1);
        }

        while ((dir = readdir(d)) != NULL)
        {
            if (strstr(dir->d_name, ".jack") != NULL) {
                printf("file: %s\n", dir->d_name);
                process_file(dir->d_name, filename);
            }

        }
    } else {
        if(S_ISREG(file_info.st_mode)) {
            printf("file: %s\n", filename);
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
    int file_path_size = strlen(in_dirname) + strlen(in_file_name); //jack --> /vm\0
    char *out_file_path = malloc(file_path_size);

    assert(out_file_path != NULL);

    strcpy(out_file_path, in_dirname);
    strcat(out_file_path, "/"); // two slashes is fine in a linux file path

    //take jack off end of in file name and join directory path and new file name
    strncat(out_file_path, in_file_name, strlen(in_file_name) - 4);

    //concatonate vm to the out file path
    strcat(out_file_path, "vm");
    // printf("without .jack: %s\n", out_file_path);

    printf("%s\n", out_file_path);

    char *in_file_path = malloc(file_path_size + 2);
    strcpy(in_file_path, in_dirname);
    strcat(in_file_path, "/"); // two slashes is fine in a linux file path
    strcat(in_file_path, in_file_name);

    jack_tokenizer *tokenizer = open_file(in_file_path);
    tokenizer_execute(tokenizer);

    compilation_engine *compilationEngine = CompilationEngine(tokenizer, out_file_path);
    FILE *written_file = compileClass(compilationEngine);

    fclose(written_file);
    fclose(tokenizer->in_file);
}
