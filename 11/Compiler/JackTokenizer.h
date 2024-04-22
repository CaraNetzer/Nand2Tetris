#include <stdio.h>

typedef struct {
    FILE* in_file;
    char* file_path;
    // char* tokens[2000];
    char **tokens;
    int next_index;
    int max_tokens;
} jack_tokenizer;

jack_tokenizer* open_file(char *in_file_path);

void tokenizer_execute(jack_tokenizer *tokenizer);

void append_token(char *token);
