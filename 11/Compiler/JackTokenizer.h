#include <stdio.h>

typedef struct {
    FILE* in_file;
    char* file_path;
    // char* tokens[2000];
    char **tokens;
    int next_index;
    int max_tokens;
} jack_tokenizer;

// FILE read_file(char *in_file_path);
jack_tokenizer* open_file(char *in_file_path);

void tokenizer_execute(jack_tokenizer *tokenizer);

void append_tokens(char **words, char **token_list);
