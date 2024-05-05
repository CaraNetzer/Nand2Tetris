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


    // # print("eat: " + match)
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

    //
    //
    // def check_for_one_or_more_identifiers(self):
    //     if self.tokenizer.get_current_token().get_token() == ",":
    //         self.emit()
    //     else:
    //         return False
    //
    //     if self.tokenizer.get_current_token().get_type() == "identifier":
    //         self.emit()
    //         return True
    //     else:
    //         self.syntax_error(self.tokenizer.get_current_token().get_token())
    //
    //
bool compileClassVarDec() {

    // ('static' | 'field') type varName (',' varName)* ';'
    
    current_token = compiler->tokenizer->tokenized_tokens[compiler->tokenizer->next_index];

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
        if (array_contains(var_types, 3, current_token->item) || !strcmp(current_token->type, "identifier")) {
            // printf("%s\n", current_token->item);
            type = compiler->tokenizer->tokenized_tokens[compiler->tokenizer->next_index]->item;
            advance_token();
            // self.emit()
        }

        // varName
        check_token("type", "identifier");
        char *name = compiler->tokenizer->tokenized_tokens[compiler->tokenizer->next_index]->item;
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

            // while (compileParameterList()) {
            //     continue;
            // }

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
            // while (compileVarDec()) {
            //     continue;
            // }

            // statements
            // self.out_file.write(f"\n{self.indent}<statements>")
            // self.inc_indent()
            // while (compileStatements()) {
            //     continue;
            // }
            // self.dec_indent()
            // self.out_file.write(f"\n{self.indent}</statements>")

            // '}'
            // self.eat("token", "}")
            check_token("token", "}");
            advance_token();

            // self.dec_indent()
            // self.out_file.write(f"\n{self.indent}</subroutineBody>")
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
    //
    // def compileParameterList(self):
    //     # ((type varName) (',' varName)*)?
    //
    //     # type
    //     if (self.tokenizer.get_current_token().get_token() in Token.var_types
    //         or self.tokenizer.get_current_token().get_type() == "identifier"):
    //         self.emit()
    //
    //         # varname
    //         self.eat("type", "identifier")
    //
    //         # (',' varName)*
    //         while self.check_for_one_or_more_identifiers():
    //             pass
    //
    //     else:
    //         return False
    //
    //
    // def compileVarDec(self):
    //     # 'var' type varName (',' varName)* ';'
    //
    //     # 'var'
    //     if self.tokenizer.get_current_token().get_token() == "var":
    //         self.out_file.write(f"\n{self.indent}<varDec>") #TODO clean this up
    //         self.inc_indent()
    //         self.emit()
    //
    //         # TODO this is the same as in parameter list, consider drying this up
    //         # type
    //         if (self.tokenizer.get_current_token().get_token() in Token.var_types
    //             or self.tokenizer.get_current_token().get_type() == "identifier"):
    //             self.emit()
    //
    //             # varname
    //             self.eat("type", "identifier")
    //
    //             # (',' varName)*
    //             while self.check_for_one_or_more_identifiers():
    //                 pass
    //
    //             # ';'
    //             self.eat("token", ";")
    //
    //             self.dec_indent()
    //             self.out_file.write(f"\n{self.indent}</varDec>")
    //             return True
    //         else:
    //             self.syntax_error(self.tokenizer.get_current_token().get_token())
    //     else:
    //         return False
    //
    //
    // def compileStatements(self):
    //
    //     # (letStatement | ifStatement | whileStatement | doStatement | returnStatement)*
    //     current_token = self.tokenizer.get_current_token().get_token()
    //     if current_token in Token.statement_types:
    //
    //         if current_token == "let":
    //             self.out_file.write(f"\n{self.indent}<letStatement>")
    //             self.inc_indent()
    //             self.compileLet()
    //             self.dec_indent()
    //             self.out_file.write(f"\n{self.indent}</letStatement>")
    //
    //         if current_token == "if":
    //             self.out_file.write(f"\n{self.indent}<ifStatement>")
    //             self.inc_indent()
    //             self.compileIf()
    //             self.dec_indent()
    //             self.out_file.write(f"\n{self.indent}</ifStatement>")
    //
    //         if current_token == "while":
    //             self.out_file.write(f"\n{self.indent}<whileStatement>")
    //             self.inc_indent()
    //             self.compileWhile()
    //             self.dec_indent()
    //             self.out_file.write(f"\n{self.indent}</whileStatement>")
    //
    //         if current_token == "do":
    //             self.out_file.write(f"\n{self.indent}<doStatement>")
    //             self.inc_indent()
    //             self.compileDo()
    //             self.dec_indent()
    //             self.out_file.write(f"\n{self.indent}</doStatement>")
    //
    //         if current_token == "return":
    //             self.out_file.write(f"\n{self.indent}<returnStatement>")
    //             self.inc_indent()
    //             self.compileReturn()
    //             self.dec_indent()
    //             self.out_file.write(f"\n{self.indent}</returnStatement>")
    //
    //         return True
    //     else:
    //         return False
    //
    //
    // def compileDo(self):
    //     # 'do' subroutineCall = subroutineName '(' expressionList ')' | (className | varName) '.'
    //     # subroutineName '(' expressionList ')' ';'
    //
    //     # 'do'
    //     self.eat("token", "do")
    //
    //     # subroutineName | className | varName
    //     self.eat("type", "identifier")
    //
    //     # subroutineCall
    //     self.compileSubroutineCall()
    //
    //     #';'
    //     self.eat("token", ";")
    //
    //     return True
    //
    //
    // def compileSubroutineCall(self):
    //     # subroutineName                           '(' expressionList ')' |
    //     # (className | varName) '.' subroutineName '(' expressionList ')'
    //
    //     # '.' subroutineName
    //     if (self.tokenizer.get_current_token().get_token() == "."):
    //         # '.'
    //         self.emit()
    //
    //         # sunroutineName
    //         self.eat("type", "identifier")
    //
    //     # '(' expressionList ')'
    //
    //     self.eat("token", "(")
    //
    //     # expressionList
    //     self.out_file.write(f"\n{self.indent}<expressionList>")
    //     self.inc_indent()
    //     while self.compileExpressionList():
    //         pass
    //     self.dec_indent()
    //     self.out_file.write(f"\n{self.indent}</expressionList>")
    //
    //     # )
    //     self.eat("token", ")")
    //
    //
    // def compileIndexedExpression(self):
    //     # '[' expression ']'
    //     if self.tokenizer.get_current_token().get_token() == "[":
    //         self.emit()
    //
    //         self.compileExpression()
    //
    //         self.eat("token", "]")
    //     else:
    //         return False
    //
    //
    // def compileLet(self):
    //     # 'let' varName ('[' expression ']')? '=' expression ';'
    //
    //     # 'let'
    //     self.eat("token", "let")
    //
    //     # varName
    //     self.eat("type", "identifier")
    //
    //     # '[' expression ']'
    //     while self.compileIndexedExpression():
    //         pass
    //
    //     # '='
    //     self.eat("token", "=")
    //
    //     # expression
    //     self.compileExpression()
    //
    //     # ';'
    //     self.eat("token", ";")
    //
    //     return True
    //
    // def compileWhile(self):
    //     # 'while' '(' expression ')' '{' statements '}'
    //     self.eat("token", "while")
    //
    //     self.eat("token", "(")
    //
    //     self.compileExpression()
    //
    //     self.eat("token", ")")
    //
    //     self.eat("token", "{")
    //
    //     self.out_file.write(f"\n{self.indent}<statements>")
    //     self.inc_indent()
    //     while self.compileStatements():
    //         pass
    //     self.dec_indent()
    //     self.out_file.write(f"\n{self.indent}</statements>")
    //
    //     self.eat("token", "}")
    //
    //     return True
    //
    //
    // def compileReturn(self):
    //     # 'return' expression
    //     self.eat("token", "return")
    //
    //     while self.compileExpression():
    //         pass
    //
    //     self.eat("token", ";")
    //
    //     return True
    //
    //
    // def compileElseStatement(self):
    //     # 'else' '{' statements '}'
    //     if self.tokenizer.get_current_token().get_token() == "else":
    //         # 'else'
    //         self.emit()
    //
    //         # '{'
    //         self.eat("token", "{")
    //
    //         # statements
    //         self.out_file.write(f"\n{self.indent}<statements>")
    //         self.inc_indent()
    //         while self.compileStatements():
    //             pass
    //         self.dec_indent()
    //         self.out_file.write(f"\n{self.indent}</statements>")
    //
    //         # '}'
    //         self.eat("token", "}")
    //     else:
    //         return False
    //
    //
    // def compileIf(self):
    //
    //     # 'if' '(' expression ')' '{' statements '}' ('else' '{' statements '}')?
    //
    //     # 'if'
    //     self.eat("token", "if")
    //
    //     # '('
    //     self.eat("token", "(")
    //
    //     self.compileExpression()
    //
    //     # ')'
    //     self.eat("token", ")")
    //
    //     # '{'
    //     self.eat("token", "{")
    //
    //     # statements
    //     self.out_file.write(f"\n{self.indent}<statements>")
    //     self.inc_indent()
    //     while self.compileStatements():
    //         pass
    //     self.dec_indent()
    //     self.out_file.write(f"\n{self.indent}</statements>")
    //
    //     # '}'
    //     self.eat("token", "}")
    //
    //     # 'else' '{' statements '}'
    //     while self.compileElseStatement():
    //         pass
    //
    //     return True
    //
    // def compileOpTerm(self):
    //     # op term
    //     # print("op term: ", self.tokenizer.get_current_token().get_token())
    //     if self.tokenizer.get_current_token().get_token() in Token.operators:
    //         # op
    //         self.eat("type", "symbol")
    //
    //         # term
    //         self.compileTerm()
    //
    //         return True
    //     else:
    //         return False
    //
    //
    // def compileExpression(self):
    //
    //     # term (op term)*
    //     current_token = self.tokenizer.get_current_token().get_token()
    //     if self.tokenizer.get_current_token().token_type(current_token) != "symbol" or current_token in Token.unary_operators:
    //         self.out_file.write(f"\n{self.indent}<expression>")
    //         self.inc_indent()
    //
    //         # term
    //         self.compileTerm()
    //
    //         # (op term)*
    //         while self.compileOpTerm():
    //             pass
    //
    //         self.dec_indent()
    //         self.out_file.write(f"\n{self.indent}</expression>")
    //
    //         return True
    //     else:
    //         return False
    //
    //
    // def compileTerm(self):
    //     # integerConstant | stringConstant | keywordConstant | varName |
    //     # varName '[' expression ']' | subroutineCall |'(' expression ')' | unaryOp term
    //
    //     # if token is an identifer, need to look at the next token to distinguish between varName, varName[], and subroutineCall
    //     current_token = self.tokenizer.get_current_token().get_token()
    //
    //     if self.tokenizer.get_current_token().token_type(current_token) != "symbol" or current_token == "(":
    //         self.out_file.write(f"\n{self.indent}<term>")
    //         self.inc_indent()
    //
    //     # integerConstant | stringConstant | keywordConstant
    //     if (self.tokenizer.get_current_token().token_type(current_token) == "integerConstant" or
    //        self.tokenizer.get_current_token().token_type(current_token) == "stringConstant" or
    //        self.tokenizer.get_current_token().token_type(current_token) == "keyword"):
    //
    //         self.eat("token", current_token)
    //
    //     # unaryOp term
    //     elif current_token in Token.unary_operators:
    //         self.out_file.write(f"\n{self.indent}<term>")
    //         self.inc_indent()
    //
    //         self.eat("type", "symbol")
    //         self.compileTerm()
    //
    //
    //     # '(' expression ')'
    //     elif current_token == '(':
    //         self.eat("token", "(")
    //         self.compileExpression()
    //         self.eat("token", ")")
    //
    //     elif self.tokenizer.get_current_token().token_type(current_token) == "identifier":
    //         # this is where we need to look two ahead
    //
    //         # varName
    //         self.eat("type", "identifier")
    //
    //         current_token = self.tokenizer.get_current_token().get_token()
    //
    //         # varName '[' expression ']'
    //         if current_token == "[":
    //             self.compileIndexedExpression()
    //
    //         # subroutineName '(' expressionList ')' | (className | varName) '.' subroutineName ...
    //         elif current_token == "(" or current_token == ".":
    //             self.compileSubroutineCall()
    //
    //     else:
    //         return False
    //
    //
    //     self.dec_indent()
    //     self.out_file.write(f"\n{self.indent}</term>")
    //
    //     return True
    //
    //
    // def check_for_one_or_more_expressions(self):
    //     if self.tokenizer.get_current_token().get_token() == ",":
    //         self.emit()
    //     else:
    //         return False
    //
    //     if self.compileTerm():
    //         # (op term)*
    //         while self.compileOpTerm():
    //             pass
    //
    //         return True
    //     else:
    //         self.syntax_error(self.tokenizer.get_current_token().get_token())
    //
    //
    // def compileExpressionList(self):
    //     # (expression (',' expression)*)?
    //
    //     current_token = self.tokenizer.get_current_token().get_token()
    //     if self.tokenizer.get_current_token().token_type(current_token) != "symbol":
    //         self.out_file.write(f"\n{self.indent}<expression>")
    //         self.inc_indent()
    //
    //     # expression = term (op term)*
    //     if self.compileTerm():
    //
    //         # (op term)*
    //         while self.compileOpTerm():
    //             # print(self.tokenizer.get_current_token().get_token())
    //             pass
    //
    //         # (',' expression)*
    //         while self.check_for_one_or_more_expressions():
    //             pass
    //
    //         self.dec_indent()
    //         self.out_file.write(f"\n{self.indent}</expression>")
    //
    //     else:
    //         return False
    //
    //
