#include <stdlib.h>
#include <string.h>
#include "CompilationEngine.h"
#include "CompilationIntestines.h"
#include "CompileStatements.h"

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

    writer = create_vm_writer(out_file_path);

    return compiler;
}

void syntax_error(char* actual, char* expected) {
    printf("syntax error: \n\tactual   - '%s'\n\texpected - '%s'\n", actual, expected);
}

void check_token(char* item, char* match, char *action) {
    token** token_list = compiler->tokenizer->tokenized_tokens;
    int i = compiler->tokenizer->next_index;

    if((equal(item, "type")  && not_equal(token_list[i]->type, match))
    || (equal(item, "token") && not_equal(token_list[i]->item, match))) {
        syntax_error(token_list[i]->item, match);
    }
}

token* _reset_token() {
  return current_token = compiler->tokenizer->tokenized_tokens[compiler->tokenizer->next_index];
}

token* advance_token() {
    compiler->tokenizer->next_index++;
    return _reset_token();
}
token* rewind_token() {
    compiler->tokenizer->next_index--;
    return _reset_token();
}

FILE *compileClass(compilation_engine *compiler) {
    class_symbol_table = create_symbol_table();
    subroutine_symbol_table = create_symbol_table();

    // 'class' className '{' classVarDec* subroutineDec* '}'

    // 'class'
    check_token("token", "class", "misc");
    advance_token();

    // className
    check_token("type", "identifier", "misc");
    className = strdup(current_token->item);
    advance_token();

    // '{'
    check_token("token", "{", "misc");
    advance_token();

    // classVarDec*
    while (compileClassVarDec()) {
        continue;
    }

    // subroutineDec*
    while (compileSubroutine()) {
        continue;
    }

    // '}'
    check_token("token", "}", "misc");
    advance_token();

    return writer->out_file;
}


bool compileClassVarDec() {
    // ('static' | 'field') type varName (',' varName)* ';'

    // ('static' | 'field')
    if (equal(current_token->item, "static") || equal(current_token->item, "field")) {

        char *kind = current_token->item;
        advance_token();

        char *type;
        // type { 'int' | 'char' | 'boolean' | varName }
        if (array_contains(var_types, 4, current_token->item)
         || equal(current_token->type, "identifier")) {

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

    // ('constructor' | 'function' | 'method') ('void' | type) subroutineName '(' parameterList ')'
    // '{' varDec* statements '}'

    // ('constructor' | 'function' | 'method')
    if(array_contains(function_types, 3, current_token->item)) {

        if (equal(current_token->item, "method")) {
            define_row("this", className, "argument", subroutine_symbol_table);
            subroutineKind = "method";
        } else if (equal(current_token->item, "function")) {
            subroutineKind = "function";
        } else if (equal(current_token->item, "constructor")) {
            subroutineKind = "constructor";
        }
        advance_token();

        // ('void' | type)
        if (array_contains(var_types, 4, current_token->item)
         || equal(current_token->type, "identifier")) {

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
            compileParameterList();

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
            if (equal(subroutineKind, "method")) {
                write_push_specific(writer, "argument", 0);
                write_pop(writer, "pointer", 0);
            } else if (equal(subroutineKind, "constructor")) {
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

        } else {
            syntax_error(current_token->item, "void or {type}");
        }

        return true;

    } else {
        return false;
    }
}


bool compileVarDec(int *var_count) {
    // 'var' type varName (',' varName)* ';'

    // 'var'
    if (equal(current_token->item, "var")) {

        advance_token();

        char *type;
        compileParameterOrVar(&type, "var");
        *var_count += 1;

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


bool compileSubroutineCall(char *subroutineName, bool voidFunction, token *previous_token, bool objPushedInDo) {
    // subroutineName                           '(' expressionList ')' |
    // (className | varName) '.' subroutineName '(' expressionList ')'

    char *subroutineFullName = subroutineName;

    bool pushed_this = false;

    //varName preceded
    // '.' subroutineName
    if (equal(current_token->item, ".")) {

        // '.'
        advance_token();

        // subroutineName
        check_token("type", "identifier", "misc");
        subroutineFullName = combineSubroutineName(subroutineName, current_token->item);
        advance_token();

        pushed_this = objPushedInDo ? true : false;

    } else {

        //look ahead
        advance_token();

        //ClassName.method()
        if (equal(current_token->item, ".")) {

            //to get past the '.'
            advance_token();

            check_token("type", "identifier", "misc");
            subroutineFullName = combineSubroutineName(subroutineName, current_token->item);


        // subroutineName (ie draw())
        } else {
            rewind_token();

            subroutineFullName = combineSubroutineName(className, subroutineName);

            write_push_specific(writer, "pointer", 0);
            pushed_this = true;

        }
        advance_token();
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


    //objPushedInLet (not Do)
    if(previous_token != NULL) {
        write_push(writer, previous_token); // this, unless constructor
        pushed_this = true;
    }
    if(pushed_this) {
        arg_count++;
    }
    write_call(writer, subroutineFullName, arg_count);
    if(voidFunction) {
        write_pop(writer, "temp", 0);
    }

    return true;
}


bool compileExpression() {

    // term (op term)*
    if (not_equal(current_token->type, "symbol")
        || array_contains(unary_operators, 2, current_token->item)
        || equal(current_token->item, "(")) {

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

  // if token is an identifer, need to look at the next token to distinguish between varName,
  // varName[], and subroutineCall

  // integerConstant | stringConstant | keywordConstant
  if (equal(current_token->type, "integerConstant") ||
      equal(current_token->type, "stringConstant") ||
      equal(current_token->type, "keyword")) {

    check_token("token", current_token->item, "misc");
    write_push(writer, current_token);
    advance_token();
  }

  // unaryOp term
  else if (array_contains(unary_operators, 2, current_token->item)) {

    check_token("type", "symbol", "misc");
    char *op = current_token->item;
    advance_token();
    compileTerm();

    write_arithmetic(writer, op, true);
  }

  // '(' expression ')'
  else if(equal(current_token->item, "(")) {
    check_token("token", "(", "misc");
    advance_token();

    compileExpression();

    check_token("token", ")", "misc");
    advance_token();
  }

  else if (equal(current_token->type, "identifier")) {
    // this is where we need to look two ahead

    // varName
    check_token("type", "identifier", "use");
    token *previous_token = current_token;
    advance_token();

    // TWO AHEAD/haven't processed prev token

    // varName '[' expression ']'
    if (equal(current_token->item, "[")) {
        write_push(writer, previous_token);
        compileIndexedExpression();
        write_pop(writer, "pointer", 1);
        write_push_specific(writer, "that", 0);
    }

    // subroutineName '(' expressionList ')' | (className | varName) '.' subroutineName ...
    else if (equal(current_token->item, "(") || equal(current_token->item, ".")) {
        if(find_by_name(previous_token->item, subroutine_symbol_table) || find_by_name(previous_token->item, class_symbol_table)) {
            compileSubroutineCall(previous_token->item, false, previous_token, false);
        } else {
            printf("class name encountered or previous_token not found in symbol tables: %s\n", previous_token->item);
            compileSubroutineCall(previous_token->item, false, NULL, false);
        }
    }

    else { // identifier by itself - process prev token
        rewind_token();
        write_push(writer, current_token);
        advance_token();
    }

  } else {
    return false;
  }

  return true;
}
