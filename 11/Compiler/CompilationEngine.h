#include <stdio.h>
#include "JackTokenizer.h"

typedef struct {
    char **tokens;
    jack_tokenizer *tokenizer;
    FILE *out_file;
} compilation_engine;

compilation_engine *CompilationEngine(jack_tokenizer *in_tokenizer, char *out_file_path);
