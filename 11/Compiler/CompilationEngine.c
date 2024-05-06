#include <stdlib.h>
#include <string.h>
#include "CompilationEngine.h"
#include "SymbolTable.h"
#include "VMWriter.h"

int level = 0;
int indent = 0;
char tab[] = "  ";

compilation_engine *compiler;
token *current_token;
symbol_table *class_symbol_table;
symbol_table *subroutine_symbol_table;

compilation_engine* CompilationEngine(jack_tokenizer *in_tokenizer, char *out_file_path) {
    compiler = calloc(1, sizeof(compilation_engine));
    if(!compiler) {
        perror("calloc failed for compilation engine constructor");
        exit(1);
    }

    compiler->tokens = in_tokenizer->tokenized_tokens;
    compiler->tokenizer = in_tokenizer;

    current_token = compiler->tokenizer->tokenized_tokens[compiler->tokenizer->next_index];

    compiler->out_file = fopen(out_file_path, "w");
    if(!compiler->out_file) {
        perror("failed to open out_file for writing");
        exit(1);
    }

    return compiler;
}

    // def inc_indent(self):
    //     self.level = self.level + 1
    //     self.indent = self.tab * self.level
    //
    // def dec_indent(self):
    //     self.level = self.level - 1
    //     self.indent = self.tab * self.level

void emit(token *token) {

    if(!strcmp(token->type, "identifier")) {

    } else if (!strcmp(token->type, "keyword")) {

    }
}

void check_token(char* item, char* match) {
    token** token_list = compiler->tokenizer->tokenized_tokens;
    int i = compiler->tokenizer->next_index;

    if(!strcmp(item, "type")) {
        if(!strcmp(token_list[i]->type, match)) {
            emit(token_list[i]);
        } else {
            syntax_error(token_list[i]->item, match);
        }
    } else if (!strcmp(item, "token")) {
        if(!strcmp(token_list[i]->item, match)) {
            emit(token_list[i]);
        } else {
            syntax_error(token_list[i]->item, match);
        }
    }
}

void process_keyword(char* item, char* match, token** tokens_list, int i) {

    token* current_token = tokens_list[i];

    if(!strcmp(current_token->type, "keyword")) {
        if(array_contains(variable_declarations, 3, current_token->item)) {
            define_row(current_token->item, current_token->item, current_token->item, class_symbol_table);
        }
    }

}

void process_identifier(char* item, char* match, token** tokens_list, int i) {

    token* current_token = tokens_list[i];

    if(!strcmp(current_token->type, "identifier")) {
        define_row(current_token->item, current_token->item, current_token->item, class_symbol_table);
    }
}

void syntax_error(char* actual, char* expected) {
    printf("syntax error: actual - %s, expected - %s\n", actual, expected);
}

token* advance_token() {
    compiler->tokenizer->next_index++;
    // printf("next token: %s\n", compiler->tokenizer->tokenized_tokens[compiler->tokenizer->next_index]->item);
    current_token = compiler->tokenizer->tokenized_tokens[compiler->tokenizer->next_index];
    return compiler->tokenizer->tokenized_tokens[compiler->tokenizer->next_index];
}

void compileClass(compilation_engine *compiler) {
    class_symbol_table = create_symbol_table();
    subroutine_symbol_table = create_symbol_table();

    token** token_list = compiler->tokenizer->tokenized_tokens;
    // jack_tokenizer *tokenizer = compiler->tokenizer;

    // 'class' className '{' classVarDec* subroutineDec* '}'

    // self.out_file.write("<class>")
    // self.inc_indent()

    // # 'class'
    check_token("token", "class");
    advance_token();

    // # className
    check_token("type", "identifier");
    advance_token();

    // # '{'
    check_token("token", "{");
    advance_token();

    // classVarDec*
    while (compileClassVarDec()) {
        continue;
    }

    // subroutineDec*
    while (compileSubroutine()) {
        continue;
    }

    // # '}'
    check_token("token", "}");
    advance_token();

    // self.dec_indent()
    // self.out_file.write(f"\n{self.indent}</class>")

    for (int i = 0; i < compiler->tokenizer->next_index; i++) {
        fprintf(compiler->out_file, "%s, %s\n", token_list[i]->item, token_list[i]->type);
        // define_row(token_list[i]->item, token_list[i]->type, "kind unknown", class_symbol_table);
    }

    for (int j = 0; j < class_symbol_table->next_index; j++) {
        printf("name: %s, ", class_symbol_table->rows[j]->name);
        printf("type: %s, ", class_symbol_table->rows[j]->type);
        printf("kind: %s, ", class_symbol_table->rows[j]->kind);
        printf("n: %d\n", class_symbol_table->rows[j]->n);
    }
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
        check_token("type", "identifier");
        advance_token();

        define_row(name, type, "ARG", subroutine_symbol_table);

        return true;
    } else {
        syntax_error(current_token->item, "type");
        return false;
    }
}

bool check_for_one_or_more_identifiers() {

    if (!strcmp(",", current_token->item)) {
        advance_token();
    } else {
        return false;
    }

    // identifier
    if (!strcmp(current_token->type, "identifier")) {

        // varname
        char *name = current_token->item;
        check_token("type", "identifier");
        advance_token();

        char *type = current_token->item;
        advance_token();


        define_row(name, type, "VAR", subroutine_symbol_table);

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
        // emit(current_token);

        char *type;
        // type { 'int' | 'char' | 'boolean' | varName }
        // printf("after advance: %s\n", current_token->item);
        if (array_contains(var_types, 4, current_token->item) || !strcmp(current_token->type, "identifier")) {
            // printf("%s\n", current_token->item);
            type = current_token->item;
            advance_token();
            // self.emit()
        }

        // varName
        check_token("type", "identifier");
        char *name = current_token->item;
        advance_token();

        define_row(name, type, kind, class_symbol_table);

        // (',' varName)*
        // while check_for_one_or_more_identifiers() {
        // }

        // ';'
        check_token("token", ";");
        advance_token();

        // self.dec_indent()
        // self.out_file.write(f"\n{self.indent}</classVarDec>")

        return true;

    } else {
        return false;
    }
}

bool compileSubroutine() {
    startSubroutine(subroutine_symbol_table);

    // ('constructor' | 'function' | 'method') ('void' | type) subroutineName '(' parameterList ')' '{' varDec* statements '}'
    
    // ('constructor' | 'function' | 'method')
    if(array_contains(function_types, 2, current_token->item)) {
        // self.out_file.write(f"\n{self.indent}<subroutineDec>")
        // self.inc_indent()

        // self.emit()
        // check_token("token", current_token->item);
        advance_token();

        // ('void' | type)
        if (array_contains(var_types, 4, current_token->item)) {

            printf("%s\n", current_token->item);
            // self.emit()
            advance_token();

            // subroutineName
            // self.eat("type", "identifier")
            check_token("type", "identifier");
            advance_token();

            // '('
            // self.eat("token", "(")
            check_token("token", "(");
            advance_token();

            // parameterList
            // self.out_file.write(f"\n{self.indent}<parameterList>")
            // self.inc_indent()

            while (compileParameterList()) {
                continue;
            }

            // self.dec_indent()
            // self.out_file.write(f"\n{self.indent}</parameterList>")

            // ')'
            // self.eat("token", ")")
            check_token("token", ")");
            advance_token();

            // subroutineBody = '{' varDec* statements '}'
            // self.out_file.write(f"\n{self.indent}<subroutineBody>")
            // self.inc_indent()

            // '{'
            // self.eat("token", "{")
            check_token("token", "{");
            advance_token();

            // varDec*
            while (compileVarDec()) {
                continue;
            }

            // statements
            // self.out_file.write(f"\n{self.indent}<statements>")
            // self.inc_indent()
            while (compileStatements()) {
                continue;
            }
            // self.dec_indent()
            // self.out_file.write(f"\n{self.indent}</statements>")

            // '}'
            // self.eat("token", "}")
            check_token("token", "}");
            advance_token();

            // self.dec_indent()
            // self.out_file.write(f"\n{self.indent}</subroutineBody>")
            printf("num of rows: %d\n", subroutine_symbol_table->next_index);
            for (int j = 0; j < subroutine_symbol_table->next_index; j++) {
                printf("name: %s, ", subroutine_symbol_table->rows[j]->name);
                printf("type: %s, ", subroutine_symbol_table->rows[j]->type);
                printf("kind: %s, ", subroutine_symbol_table->rows[j]->kind);
                printf("n: %d\n", subroutine_symbol_table->rows[j]->n);
            }
        } else {
            syntax_error(current_token->item, "void or {type}");
        }

        // self.dec_indent()
        // self.out_file.write(f"\n{self.indent}</subroutineDec>")

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
        check_token("type", "identifier");
        advance_token();

        define_row(name, type, "ARG", subroutine_symbol_table);

        // (',' varName)*
        while(check_for_one_or_more_parameters()) {
            continue;
        }

        return true;
    } else {
        return false;
    }
}

bool compileVarDec() {
    // 'var' type varName (',' varName)* ';'

    // 'var'
    if (0 == strcmp(current_token->item, "var")) {

        advance_token();

        // TODO this is the same as in parameter list, consider drying this up
        //
        char *type;
        // type { 'int' | 'char' | 'boolean' | varName }
        if (array_contains(var_types, 4, current_token->item) || !strcmp(current_token->type, "identifier")) {
            // printf("%s\n", current_token->item);
            type = current_token->item;
            advance_token();
        }

        // varName
        check_token("type", "identifier");
        char *name = current_token->item;
        advance_token();

        define_row(name, type, "VAR", subroutine_symbol_table);

        // (',' varName)*
        while(check_for_one_or_more_identifiers()) {
            continue;
        }

        // ';'
        check_token("token", ";");
        advance_token();

            // self.dec_indent()
            // self.out_file.write(f"\n{self.indent}</varDec>")
        return true;
        // self.out_file.write(f"\n{self.indent}<classVarDec>")
        // self.inc_indent()

    } else {
        return false;
    }
}

bool compileStatements() {

    // (letStatement | ifStatement | whileStatement | doStatement | returnStatement)*
    if (array_contains(statement_types, 6, current_token->item)) { 

        if(!strcmp(current_token->item, "let")) {
            // self.out_file.write(f"\n{self.indent}<letStatement>")
            // self.inc_indent()
            compileLet();
            // self.dec_indent()
            // self.out_file.write(f"\n{self.indent}</letStatement>")
        }

        if (!strcmp(current_token->item, "if")) {
            // self.out_file.write(f"\n{self.indent}<ifStatement>")
            // self.inc_indent()
            compileIf();
            // self.dec_indent()
            // self.out_file.write(f"\n{self.indent}</ifStatement>")
        }

        if (!strcmp(current_token->item, "while")) {
            // self.out_file.write(f"\n{self.indent}<whileStatement>")
            // self.inc_indent()
            compileWhile();
            // self.dec_indent()
            // self.out_file.write(f"\n{self.indent}</whileStatement>")
        }

        if (!strcmp(current_token->item, "do")) {
            // self.out_file.write(f"\n{self.indent}<doStatement>")
            // self.inc_indent()
            compileDo();
            // self.dec_indent()
            // self.out_file.write(f"\n{self.indent}</doStatement>")
        }

        if (!strcmp(current_token->item, "return")) {
            // self.out_file.write(f"\n{self.indent}<returnStatement>")
            // self.inc_indent()
            compileReturn();
            // self.dec_indent()
            // self.out_file.write(f"\n{self.indent}</returnStatement>")
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
    check_token("token", "do");
    advance_token();

    // subroutineName | className | varName
    check_token("type", "identifier");
    advance_token();

    // subroutineCall
    compileSubroutineCall();

    //';'
    check_token("token", ";");
    advance_token();

    return true;
}

bool compileSubroutineCall() {
    // subroutineName                           '(' expressionList ')' |
    // (className | varName) '.' subroutineName '(' expressionList ')'

    // '.' subroutineName
    if (!strcmp(current_token->item, ".")) {

        // '.'
        advance_token();

        // sunroutineName
        check_token("type", "identifier");
        advance_token();
    }

    // '(' expressionList ')'
    check_token("token", "(");
    advance_token();

    // expressionList
    // self.out_file.write(f"\n{self.indent}<expressionList>")
    // self.inc_indent()
    while (compileExpressionList()) {
        continue;
    }
    // self.dec_indent()
    // self.out_file.write(f"\n{self.indent}</expressionList>")

    // )
    check_token("token", ")");
    advance_token();

    return true;
}

bool compileIndexedExpression() {
    // '[' expression ']'
    if (!strcmp(current_token->item, "[")) {

        advance_token();

        compileExpression();

        check_token("token", "]");
        advance_token();

        return true;
    } else {
        return false;
    }
}

bool compileLet() {
    // 'let' varName ('[' expression ']')? '=' expression ';'

    // 'let'
    check_token("token", "let");
    advance_token();

    // varName
    check_token("type", "identifier");
    advance_token();

    // '[' expression ']'
    while (compileIndexedExpression()) {
        continue;
    }

    // '='
    check_token("token", "=");
    advance_token();

    // expression
    compileExpression();

    // ';'
    check_token("token", ";");
    advance_token();

    return true;
}

bool compileWhile() {
    // 'while' '(' expression ')' '{' statements '}'
    check_token("token", "while");
    advance_token();

    check_token("token", "(");
    advance_token();

    compileExpression();

    check_token("token", ")");
    advance_token();

    check_token("token", "{");
    advance_token();

    // out_file.write(f"\n{self.indent}<statements>")
    // inc_indent()
    while (compileStatements()) {
        continue;
    }
    // dec_indent()
    // out_file.write(f"\n{self.indent}</statements>")

    check_token("token", "}");
    advance_token();

    return true;
}

bool compileReturn() {
    // 'return' expression
    check_token("token", "return");
    advance_token();

    while (compileExpression()) {
        continue;
    }

    check_token("token", ";");
    advance_token();

    return true;
}

bool compileElseStatement() {
    // 'else' '{' statements '}'
    if (!strcmp(current_token->item, "else")) {
        // 'else'
        advance_token();

        // '{'
        check_token("token", "{");
        advance_token();

        // statements
        // out_file.write(f"\n{self.indent}<statements>")
        // inc_indent()
        while (compileStatements()) {
            continue;
        }
        // dec_indent()
        // out_file.write(f"\n{self.indent}</statements>")

        // '}'
        check_token("token", "}");
        advance_token();

        return true;
    } else {
        return false;
    }
}

bool compileIf() {

    // 'if' '(' expression ')' '{' statements '}' ('else' '{' statements '}')?

    // 'if'
    check_token("token", "if");
    advance_token();

    // '('
    check_token("token", "(");
    advance_token();

    compileExpression();

    // ')'
    check_token("token", ")");
    advance_token();

    // '{'
    check_token("token", "{");
    advance_token();

    // statements
    // out_file.write(f"\n{self.indent}<statements>")
    // inc_indent()
    while (compileStatements()) {
        continue;
    }
    // dec_indent()
    // out_file.write(f"\n{self.indent}</statements>")

    // '}'
    check_token("token", "}");
    advance_token();

    // 'else' '{' statements '}'
    while (compileElseStatement()) {
        continue;
    }

    return true;
}

bool compileOpTerm() {
    // op term
    if(array_contains(operators, 10, current_token->item)) {
        // op
        check_token("type", "symbol");
        advance_token();

        // term
        compileTerm();

        return true;
    } else {
        return false;
    }
}

bool compileExpression() {

    // term (op term)*
    if (strcmp(current_token->type, "symbol") != 0 || array_contains(unary_operators, 2, current_token->item)) {
        // self.out_file.write(f"\n{self.indent}<expression>")
        // self.inc_indent()

        // term
        compileTerm();

        // (op term)*
        while (compileOpTerm()) {
            continue;
        }

        // self.dec_indent()
        // self.out_file.write(f"\n{self.indent}</expression>")

        return true;
    } else {
        return false;
    }
}


bool compileTerm() {
    // integerConstant | stringConstant | keywordConstant | varName |
    // varName '[' expression ']' | subroutineCall |'(' expression ')' | unaryOp term

    // if token is an identifer, need to look at the next token to distinguish between varName, varName[], and subroutineCall

    // if (strcmp(current_token->type, "symbol") != 0 || strcmp("(", current_token->item)) {
    //     out_file.write(f"\n{self.indent}<term>")
    //     inc_indent()

    // integerConstant | stringConstant | keywordConstant
    if (!strcmp(current_token->type, "integerConstant") ||
        !strcmp(current_token->type, "stringConstant") ||
        !strcmp(current_token->type, "keyword")) {

        check_token("token", current_token->item);
        advance_token();
    }

    // unaryOp term
    else if (array_contains(unary_operators, 2, current_token->item)) {
        // out_file.write(f"\n{self.indent}<term>")
        // inc_indent()

        check_token("type", "symbol");
        advance_token();
        compileTerm();
    }

    // '(' expression ')'
    else if(!strcmp(current_token->item, "(")) {
        check_token("token", "(");
        advance_token();

        compileExpression();

        check_token("token", ")");
        advance_token();
    }

    else if (!strcmp(current_token->type, "identifier")) {
        // this is where we need to look two ahead

        // varName
        check_token("type", "identifier");
        advance_token();

        // TWO AHEAD - TODO check if this is right
        // advance_token(); 

        // varName '[' expression ']'
        if (!strcmp(current_token->item, "[")) {
            compileIndexedExpression();
        }

        // subroutineName '(' expressionList ')' | (className | varName) '.' subroutineName ...
        else if (strcmp(current_token->item, "(") == 0 || strcmp(current_token->item, ".") == 0) {
            compileSubroutineCall();
        }

    } else {
        return false;
    }


    // dec_indent()
    // out_file.write(f"\n{self.indent}</term>")

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

bool compileExpressionList() {
    // (expression (',' expression)*)?

    // if tokenizer.get_current_token().token_type(current_token) != "symbol":
        // out_file.write(f"\n{self.indent}<expression>")
        // inc_indent()

    // expression = term (op term)*
    if (compileTerm()) {

        // (op term)*
        while (compileOpTerm()) {
            // print(tokenizer.get_current_token().get_token())
            continue;
        }        

        // (',' expression)*
        while (check_for_one_or_more_expressions()) {
            continue;
        }

        // dec_indent()
        // out_file.write(f"\n{self.indent}</expression>")
        return true;
    } else {
        return false;
    }
}
