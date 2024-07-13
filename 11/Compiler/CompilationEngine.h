#ifndef _CompilationEngine_H
#define _CompilationEngine_H true

#include "JackTokenizer.h"
#include "VMWriter.h"
#include <stdio.h>

typedef struct {
  token **tokens;
  jack_tokenizer *tokenizer;
  FILE *out_file;
} compilation_engine;

extern compilation_engine *compiler;
extern vm_writer *writer;
extern token *current_token;
extern symbol_table *class_symbol_table;
extern symbol_table *subroutine_symbol_table;
extern char *className;
extern char *subroutineType;
extern char *subroutineKind;
extern int ifCounter;
extern int whileCounter;


compilation_engine *CompilationEngine(jack_tokenizer *in_tokenizer, char *out_file_path);

void syntax_error(char *actual, char *expected);
void check_token(char *item, char *match, char *action);
token* advance_token();
token* rewind_token();

FILE *compileClass(compilation_engine *compiler);

bool compileClassVarDec();
bool compileSubroutine();
bool compileVarDec();
bool compileSubroutineCall(char *subroutineName, bool voidFunction, token *object, bool objPushedInDo);
bool compileExpression();
bool compileTerm();

#endif //_CompilationEngine_H
