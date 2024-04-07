#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>
#include <dirent.h>

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
                //         process_file(file_name, os.path.dirname(in_file_path))
            }

        }
    } else {
        if(S_ISREG(file_info.st_mode)) {
            printf("file: %s\n", argv[1]);
            // process_file(file_name, os.path.dirname(in_file_path))
        } else {
            fprintf(stderr, "Invalid file name: %s\n", filename);
            exit(1);
        }

    }


    return 0;
}

// def process_file(in_file_name, in_dirname):
//     out_file_name = in_file_name[0: -4] + "test.xml"
//     out_file_path = os.path.join(in_dirname, out_file_name)
//
//     tokenizer = JackTokenizer.JackTokenizer(os.path.join(in_dirname, in_file_name), out_file_path)
//     tokenizer.execute()
//     compilationEngine = CompilationEngine.CompilationEngine(tokenizer, out_file_path)
//     compilationEngine.compileClass()
