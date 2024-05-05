#include <stdio.h>
#include "JackTokenizer.h"

typedef struct {
    token **tokens;
    jack_tokenizer *tokenizer;
    FILE *out_file;
} compilation_engine;

compilation_engine *CompilationEngine(jack_tokenizer *in_tokenizer, char *out_file_path);

void compileClass(compilation_engine *compiler);

void syntax_error(char* actual, char* expected);

token* advance_token();

void process_identifier(char* item, char* match, token** tokens_list, int i);

void process_keyword(char* item, char* match, token** tokens_list, int i);

void check_token(char* item, char* match);

bool compileClassVarDec();
bool compileSubroutine();
