#ifndef _JackTokenizer_H
#define _JackTokenizer_H

#include <stdio.h>
#include "Token.h"

typedef struct {
    FILE* in_file;
    char* file_path;
    char **tokens;
    token **tokenized_tokens;
    int next_index;
    int max_tokens;
} jack_tokenizer;

jack_tokenizer* open_file(char *in_file_path);
void tokenizer_execute(jack_tokenizer *tokenizer);
void append_token(char *token);
void append_tokenized_token(token *tok, int i);

#endif //_JackTokenizer_H
