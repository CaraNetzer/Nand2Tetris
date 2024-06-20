#include <stdlib.h>
#include <string.h>
#include "CompilationEngine.h"
#include "VMWriter.h"

int ifCounter = 0;
int whileCounter = 0;

compilation_engine *compiler;
vm_writer *writer;
token *current_token;

symbol_table *class_symbol_table;
symbol_table *subroutine_symbol_table;
char *className;
char *subroutineType;
char *subroutineKind;

compilation_engine* CompilationEngine(jack_tokenizer *in_tokenizer, char *out_file_path) {
    compiler = calloc(1, sizeof(compilation_engine));
    if(!compiler) {
        perror("calloc failed for compilation engine constructor");
        exit(1);
    }

    compiler->tokens = in_tokenizer->tokenized_tokens;
    compiler->tokenizer = in_tokenizer;

    current_token = compiler->tokenizer->tokenized_tokens[compiler->tokenizer->next_index];

    /* compiler->out_file = fopen(out_file_path, "w"); */
    /* if(!compiler->out_file) { */
    /*     perror("failed to open out_file for writing"); */
    /*     exit(1); */
    /* } */

    writer = create_vm_writer(out_file_path);

    return compiler;
}

void syntax_error(char* actual, char* expected) {
    printf("syntax error: actual - '%s', expected - '%s'\n", actual, expected);
}

void emit(token *token, char * action) {

    // TODO do I need this ultimately?
    if(!strcmp(token->type, "identifier")) {
      if(find_by_name(token->item, class_symbol_table)) {
          printf("name: %s, ", token->item);
          printf("type: %s, ", type_of(token->item, class_symbol_table));
          printf("kind: %s, ", kind_of(token->item, class_symbol_table));
          printf("n: %d, ", index_of(token->item, class_symbol_table));
          printf("action: %s\n", action);
      // write vm code
      } else if(find_by_name(token->item, subroutine_symbol_table)) {

          printf("name: %s, ", token->item);
          printf("type: %s, ", type_of(token->item, subroutine_symbol_table));
          printf("kind: %s, ", kind_of(token->item, subroutine_symbol_table));
          printf("n: %d, ", index_of(token->item, subroutine_symbol_table));
          printf("action: %s\n", action);
      } else if (!strcmp(action, "define")){
          printf("name: %s, ", token->item);
          printf("action: %s\n", action);
      }
    } else if (!strcmp(token->type, "keyword")) {

    }
}

void check_token(char* item, char* match, char *action) {
    token** token_list = compiler->tokenizer->tokenized_tokens;
    int i = compiler->tokenizer->next_index;

    if(!strcmp(item, "type")) {
        if(!strcmp(token_list[i]->type, match)) {
          /* emit(token_list[i], action); */
        } else {
          syntax_error(token_list[i]->item, match);
        }
    } else if (!strcmp(item, "token")) {
        if(!strcmp(token_list[i]->item, match)) {
          /* emit(token_list[i], action); */
        } else {
          syntax_error(token_list[i]->item, match);
        }
    }
}


token* advance_token() {
  compiler->tokenizer->next_index++;

  current_token = compiler->tokenizer->tokenized_tokens[compiler->tokenizer->next_index];
  /* if(current_token) { */
  /*   printf("next token (idx=%d): item=%s, type=%s\n", compiler->tokenizer->next_index, */
  /*          current_token->item, current_token->type); */
  /* } */
  return compiler->tokenizer->tokenized_tokens[compiler->tokenizer->next_index];
}

FILE *compileClass(compilation_engine *compiler) {
    class_symbol_table = create_symbol_table();
    subroutine_symbol_table = create_symbol_table();

    /* token** token_list = compiler->tokenizer->tokenized_tokens; */
    // jack_tokenizer *tokenizer = compiler->tokenizer;

    // 'class' className '{' classVarDec* subroutineDec* '}'

    // # 'class'
    check_token("token", "class", "misc");
    advance_token();

    // # className
    check_token("type", "identifier", "misc");
    className = strdup(current_token->item);
    advance_token();

    // # '{'
    check_token("token", "{", "misc");
    advance_token();

    // classVarDec*
    while (compileClassVarDec()) {
        continue;
    }

    // subroutineDec*
    while (compileSubroutine()) {
        printf("finished subroutine\n");
        continue;
    }

    // # '}'
    check_token("token", "}", "misc");
    advance_token();

    /* for (int i = 0; i < compiler->tokenizer->next_index; i++) { */
    /*     fprintf(compiler->out_file, "%s, %s\n", token_list[i]->item, token_list[i]->type); */
    /*     // define_row(token_list[i]->item, token_list[i]->type, "kind unknown", class_symbol_table); */
    /* } */

    printf("num of rows in class symbol table: %d\n", class_symbol_table->next_index);
    for (int j = 0; j < class_symbol_table->next_index; j++) {
        printf("name: %s, ", class_symbol_table->rows[j]->name);
        printf("type: %s, ", class_symbol_table->rows[j]->type);
        printf("kind: %s, ", class_symbol_table->rows[j]->kind);
        printf("n: %d\n", class_symbol_table->rows[j]->n);
    }

    return writer->out_file;
}

bool check_for_one_or_more_parameters() {

    // (',' type varName)*)?
    if (!strcmp(",", current_token->item)) {
        advance_token();
    } else {
        return false;
    }

    // type
    if (array_contains(var_types, 4, current_token->item) || !strcmp(current_token->type, "identifier")) {

        char *type = current_token->item;
        advance_token();

        // varname
        char *name = current_token->item;
        check_token("type", "identifier", "define");
        advance_token();

        define_row(name, type, "argument", subroutine_symbol_table);

        return true;
    } else {
        syntax_error(current_token->item, "type");
        return false;
    }
}

bool check_for_one_or_more_identifiers(char *type, char *kind, char *scope, int *var_count) {

    // TODO do we need var_count in here?

    if (!strcmp(",", current_token->item)) {
        advance_token();
    } else {
        return false;
    }

    // identifier
    if (!strcmp(current_token->type, "identifier")) {

        // varname
        char *name = current_token->item;
        check_token("type", "identifier", "define");
        advance_token();

        if(!strcmp(scope, "class")) {
            define_row(name, type, kind, class_symbol_table);
        }
        else {
            define_row(name, type, kind, subroutine_symbol_table);
        }
        var_count += 1;

        return true;
    } else {
        syntax_error(current_token->item, "identifier");
        return false;
    }
}

bool compileClassVarDec() {

    // ('static' | 'field') type varName (',' varName)* ';'

    // ('static' | 'field')
    if (0 == strcmp(current_token->item, "static") || 0 == strcmp(current_token->item, "field")) {

        // self.out_file.write(f"\n{self.indent}<classVarDec>")
        // self.inc_indent()

        char *kind = current_token->item;
        advance_token();

        char *type;
        // type { 'int' | 'char' | 'boolean' | varName }
        // printf("after advance: %s\n", current_token->item);
        if (array_contains(var_types, 4, current_token->item) || !strcmp(current_token->type, "identifier")) {
            // printf("%s\n", current_token->item);
            type = current_token->item;
            advance_token();
        }

        // varName
        check_token("type", "identifier", "define");
        char *name = current_token->item;
        advance_token();

        define_row(name, type, kind, class_symbol_table);

        // (',' varName)*
        while (check_for_one_or_more_identifiers(type, kind, "class", 0)) {
            continue;
        }

        // ';'
        check_token("token", ";", "misc");
        advance_token();

        return true;

    } else {
        return false;
    }
}

bool compileSubroutine() {
    startSubroutine(subroutine_symbol_table);
    printf("start subroutine\n");

    // ('constructor' | 'function' | 'method') ('void' | type) subroutineName '(' parameterList ')' '{' varDec* statements '}'

    // ('constructor' | 'function' | 'method')
    if(array_contains(function_types, 3, current_token->item)) {

        if (!strcmp(current_token->item, "method")) {
            define_row("this", className, "argument", subroutine_symbol_table);
            subroutineKind = "method";
        } else if (!strcmp(current_token->item, "function")) {
            subroutineKind = "function";
        } else if (!strcmp(current_token->item, "constructor")) {
            subroutineKind = "constructor";
            printf("kind: %s\n", subroutineKind);
        }
        advance_token();

        // ('void' | type)
        if (array_contains(var_types, 4, current_token->item) ||
            !strcmp(current_token->type, "identifier")) {

            subroutineType = current_token->item;
            advance_token();

            // subroutineName
            char functionName[BUFSIZ] = "";
            check_token("type", "identifier", "misc");
            strcat(functionName, className); 
            strcat(functionName, ".");
            strcat(functionName, current_token->item);
            advance_token();

            // '('
            check_token("token", "(", "misc");
            advance_token();

            // parameterList
            while (compileParameterList()) {
                continue;
            }

            // ')'
            check_token("token", ")", "misc");
            advance_token();

            // subroutineBody = '{' varDec* statements '}'
            // '{'
            check_token("token", "{", "misc");
            advance_token();

            // varDec*
            int local_var_count = 0;
            while (compileVarDec(&local_var_count)) {
                continue;
            }

            write_function(writer, functionName, local_var_count);
            if (!strcmp(subroutineKind, "method")) {
                write_push_specific(writer, "argument", 0);
                write_pop(writer, "pointer", 0);
            } else if (!strcmp(subroutineKind, "constructor")) {
                write_push_specific(writer, "constant", var_count("field", class_symbol_table) + var_count("static", class_symbol_table));
                write_call(writer, "Memory.alloc", 1);
                write_pop(writer, "pointer", 0);
            }

            // statements
            while (compileStatements()) {
                continue;
            }

            // '}'
            check_token("token", "}", "misc");
            advance_token();

            printf("num of rows in subroutine symbol table: %d\n", subroutine_symbol_table->next_index);
            for (int j = 0; j < subroutine_symbol_table->next_index; j++) {
                printf("name: %s, ", subroutine_symbol_table->rows[j]->name);
                printf("type: %s, ", subroutine_symbol_table->rows[j]->type);
                printf("kind: %s, ", subroutine_symbol_table->rows[j]->kind);
                printf("n: %d\n", subroutine_symbol_table->rows[j]->n);
            }

        } else {
            syntax_error(current_token->item, "void or {type}");
        }

        return true;

    } else {
        return false;
    }
}


bool compileParameterList() {

    // ((type varName) (',' type varName)*)?

    // type
    if (array_contains(var_types, 4, current_token->item) || !strcmp(current_token->type, "identifier")) {

        char *type = current_token->item;
        advance_token();

        // varname
        char *name = current_token->item;
        check_token("type", "identifier", "define");
        advance_token();

        define_row(name, type, "argument", subroutine_symbol_table);

        // (',' varName)*
        while(check_for_one_or_more_parameters()) {
            continue;
        }

        return true;
    } else {
        return false;
    }
}

bool compileVarDec(int *var_count) {
    // 'var' type varName (',' varName)* ';'

    // 'var'
    if (0 == strcmp(current_token->item, "var")) {

        advance_token();

        // TODO this is the same as in parameter list, consider drying this up
        char *type;
        // type { 'int' | 'char' | 'boolean' | varName }
        if (array_contains(var_types, 4, current_token->item) || !strcmp(current_token->type, "identifier")) {
            // printf("%s\n", current_token->item);
            type = current_token->item;
            advance_token();
        }

        // varName
        check_token("type", "identifier", "define");
        char *name = current_token->item;
        advance_token();

        *var_count += 1;
        define_row(name, type, "local", subroutine_symbol_table);

        // (',' varName)*
        while(check_for_one_or_more_identifiers(type, "local", "subroutine", var_count)) {
            continue;
        }

        // ';'
        check_token("token", ";", "misc");
        advance_token();

        return true;

    } else {
        return false;
    }
}

bool compileStatements() {

    // (letStatement | ifStatement | whileStatement | doStatement | returnStatement)*
    if (array_contains(statement_types, 6, current_token->item)) {

        if(!strcmp(current_token->item, "let")) {
            compileLet();
        }

        if (!strcmp(current_token->item, "if")) {
            compileIf();
        }

        if (!strcmp(current_token->item, "while")) {
            compileWhile();
        }

        if (!strcmp(current_token->item, "do")) {
            compileDo();
        }

        if (!strcmp(current_token->item, "return")) {
            compileReturn();
        }

        return true;
    } else {
        return false;
    }
}

bool compileDo() {
    // 'do' subroutineCall = subroutineName '(' expressionList ')' | (className | varName) '.'

    // subroutineName '(' expressionList ')' ';'

    // 'do'
    check_token("token", "do", "misc");
    advance_token();

    // subroutineName | className | varName
    check_token("type", "identifier", "use");
    char *subroutineName = current_token->item;
    if(find_by_name(subroutineName, subroutine_symbol_table) || find_by_name(subroutineName, class_symbol_table)) {
        /* subroutineKind = "method"; */
        write_push(writer, current_token);
    } else {
        /* subroutineKind = "function"; */
    }
    advance_token();

    // subroutineCall
    compileSubroutineCall(subroutineName, true, NULL);

    //';'
    check_token("token", ";", "misc");
    advance_token();

    return true;
}

bool compileSubroutineCall(char *subroutineName, bool voidFunction, token *object) {
    // subroutineName                           '(' expressionList ')' |
    // (className | varName) '.' subroutineName '(' expressionList ')'

    // '.' subroutineName
    if (!strcmp(current_token->item, ".")) {

        // '.'
        advance_token();

        // subroutineName
        check_token("type", "identifier", "misc");
        strcat(subroutineName, ".");
        strcat(subroutineName, current_token->item);
        advance_token();
    }
    else {
        char *class = strdup(className);
        strcat(class, ".");
        strcat(class, subroutineName);
        subroutineName = class;
    }

    // '(' expressionList ')'
    check_token("token", "(", "misc");
    advance_token();

    // expressionList
    int arg_count = 0;
    while (compileExpressionList(&arg_count)) {
        continue;
    }

    // )
    check_token("token", ")", "misc");
    advance_token();

    // do statements aren't making an assignment so we're assuming it's calling a function, not a method --> void and no this arg
    if(!voidFunction) {
        write_push(writer, object);
    }
    write_call(writer, subroutineName, arg_count);
    if(voidFunction) {
        write_pop(writer, "temp", 0);
    }

    return true;
}

bool compileIndexedExpression() {
    // '[' expression ']'
    if (!strcmp(current_token->item, "[")) {

        advance_token();

        compileExpression();

        check_token("token", "]", "misc");
        advance_token();

        write_arithmetic(writer, "+", false);

        return true;
    } else {
        return false;
    }
}

bool compileLet() {
    // 'let' varName ('[' expression ']')? '=' expression ';'
    bool array_assignment = false;

    // 'let'
    check_token("token", "let", "misc");
    advance_token();

    // varName
    check_token("type", "identifier", "use");
    token *popVar = current_token;
    char *segment;
    int index;
    if(find_by_name(popVar->item, subroutine_symbol_table)) {
        segment = kind_of(popVar->item, subroutine_symbol_table);
        index = index_of(popVar->item, subroutine_symbol_table);
    } else if(find_by_name(popVar->item, class_symbol_table)) {
        segment = kind_of(popVar->item, class_symbol_table);
        index = index_of(popVar->item, class_symbol_table);
    }
    advance_token();
    if(!strcmp(current_token->item, "[")) {
        write_push(writer, popVar);
    }

    // '[' expression ']'
    if (compileIndexedExpression()) {
        array_assignment = true;
    }

    // '='
    check_token("token", "=", "misc");
    advance_token();

    // expression
    compileExpression();

    if(array_assignment) {
        write_pop(writer, "temp", 0);
        write_pop(writer, "pointer", 1);
        write_push_specific(writer, "temp", 0);
        write_pop(writer, "that", 0);
    } else {
        write_pop(writer, segment, index);
    }

    // ';'
    check_token("token", ";", "misc");
    advance_token();

    return true;
}

bool compileWhile() {

    int whileId = whileCounter++;

    write_label(writer, "BEGINWHILE", whileId);

    // 'while' '(' expression ')' '{' statements '}'
    check_token("token", "while", "misc");
    advance_token();

    check_token("token", "(", "misc");
    advance_token();

    compileExpression();

    write_if(writer, "ENDWHILE", whileId);

    check_token("token", ")", "misc");
    advance_token();

    check_token("token", "{", "misc");
    advance_token();

    while (compileStatements()) {
        continue;
    }

    write_goto(writer, "BEGINWHILE", whileId);

    check_token("token", "}", "misc");
    advance_token();

    write_label(writer, "ENDWHILE", whileId);

    return true;
}

bool compileReturn() {
    // 'return' expression
    check_token("token", "return", "misc");
    advance_token();

    while (compileExpression()) {
        continue;
    }

    check_token("token", ";", "misc");
    advance_token();

    printf("%s %s\n", subroutineKind, subroutineType);
    if(!strcmp(subroutineKind, "constructor")) {
        write_push_specific(writer, "pointer", 0);
    }
    else if (!strcmp(subroutineType, "void")) {
        write_push_specific(writer, "constant", 0);
    }

    write_return(writer);

    return true;
}

bool compileElseStatement() {
    // 'else' '{' statements '}'
    if (!strcmp(current_token->item, "else")) {
        // 'else'
        advance_token();

        // '{'
        check_token("token", "{", "misc");
        advance_token();

        // statements
        while (compileStatements()) {
            continue;
        }

        // '}'
        check_token("token", "}", "misc");
        advance_token();

        return true;
    } else {
        return false;
    }
}

bool compileIf() {

    int ifId = ifCounter++;

    // 'if' '(' expression ')' '{' statements '}' ('else' '{' statements '}')?

    // 'if'
    check_token("token", "if", "misc");
    advance_token();

    // '('
    check_token("token", "(", "misc");
    advance_token();

    compileExpression();

    // ')'
    check_token("token", ")", "misc");
    advance_token();

    write_if(writer, "ELSE", ifId);


    // '{'
    check_token("token", "{", "misc");
    advance_token();

    // statements
    while (compileStatements()) {
        continue;
    }

    write_goto(writer, "ENDIF", ifId);

    // '}'
    check_token("token", "}", "misc");
    advance_token();

    // 'else' '{' statements '}'
    write_label(writer, "ELSE", ifId);
    while (compileElseStatement()) {
        continue;
    }

    write_label(writer, "ENDIF", ifId);

    return true;
}

bool compileOpTerm() {
  // op term
  if(array_contains(operators, 10, current_token->item)) {
    // op
    check_token("type", "symbol", "misc");
    char *op = current_token->item;
    advance_token();

    // term
    compileTerm();

    write_arithmetic(writer, op, false);

    return true;
  } else {
      return false;
  }
}

bool compileExpression() {

    // term (op term)*
    if (strcmp(current_token->type, "symbol") != 0 || array_contains(unary_operators, 2, current_token->item) || !strcmp(current_token->item, "(")) {

        // term
        compileTerm();

        // (op term)*
        while (compileOpTerm()) {
            continue;
        }

        return true;
    } else {
        return false;
    }
}


bool compileTerm() {
  // integerConstant | stringConstant | keywordConstant | varName |
  // varName '[' expression ']' | subroutineCall |'(' expression ')' | unaryOp term

  // if token is an identifer, need to look at the next token to distinguish between varName, varName[], and subroutineCall

  // integerConstant | stringConstant | keywordConstant
  if (!strcmp(current_token->type, "integerConstant") ||
      !strcmp(current_token->type, "stringConstant") ||
      !strcmp(current_token->type, "keyword")) {

    check_token("token", current_token->item, "misc");
    write_push(writer, current_token);
    advance_token();
  }

  // unaryOp term
  else if (array_contains(unary_operators, 2, current_token->item)) {

    check_token("type", "symbol", "misc");
    char *op = current_token->item;
    advance_token();
    /* printf("about to eat ( after unary\n"); */
    compileTerm();

    write_arithmetic(writer, op, true);
  }

  // '(' expression ')'
  else if(!strcmp(current_token->item, "(")) {
    check_token("token", "(", "misc");
    advance_token();

    compileExpression();

    check_token("token", ")", "misc");
    advance_token();
  }

  else if (!strcmp(current_token->type, "identifier")) {
    // this is where we need to look two ahead

    // varName
    check_token("type", "identifier", "use");
    token *object = current_token;
    advance_token();

    // TWO AHEAD - TODO check if this is right + maybe make cleaner

    // varName '[' expression ']'
    if (!strcmp(current_token->item, "[")) {
        write_push(writer, object);
        compileIndexedExpression();
        write_pop(writer, "pointer", 1);
        write_push_specific(writer, "that", 0);
        /* /\* write_pop(writer, "temp", 0); *\/ */
    }

    // subroutineName '(' expressionList ')' | (className | varName) '.' subroutineName ...
    else if (strcmp(current_token->item, "(") == 0 || strcmp(current_token->item, ".") == 0) {
        if(find_by_name(object->item, subroutine_symbol_table) || find_by_name(object->item, class_symbol_table)) {
        } else {
            syntax_error(object->item, "object not found in symbol tables");
        }
        compileSubroutineCall(object->item, false, object);
    }
    else { // identifier by itself
        //go back one token
        compiler->tokenizer->next_index--;
        current_token = compiler->tokenizer->tokenized_tokens[compiler->tokenizer->next_index];
        write_push(writer, current_token);
        advance_token();
    }

  } else {
    return false;
  }

  return true;
}

bool check_for_one_or_more_expressions() {
    if (!strcmp(current_token->item, ",")) {
        advance_token();
    } else {
        return false;
    }

    if (compileTerm()) {
        // (op term)*
        while (compileOpTerm()) {
            continue;
        }
    } else {
        syntax_error(current_token->item, "op term after ,");
    }

    return true;
}

bool compileExpressionList(int* arg_count) {
    // (expression (',' expression)*)?

    // expression = term (op term)*
    if (compileTerm()) {

        *arg_count += 1;
        // (op term)*
        while (compileOpTerm()) {
            // print(tokenizer.get_current_token().get_token())
            continue;
        }

        // (',' expression)*
        while (check_for_one_or_more_expressions()) {
            *arg_count += 1;
            continue;
        }

        return true;
    } else {
        return false;
    }
}
