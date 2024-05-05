#include <stdlib.h>
#include "CompilationEngine.h"
#include "SymbolTable.h"
#include "VMWriter.h"

int level = 0;
int indent = 0;
char tab[] = "  ";

compilation_engine *compiler;
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

void emit(char *token) {

    fprintf(compiler->out_file, "\n%s", compiler->tokenizer->file_path); //get_current_token_str());
    // out_file.write(f"\n{self.indent}{self.tokenizer.get_current_token_str()}");
    // tokenizer.advance();
}

    // def eat(self, item, match):
    //     # print("eat: " + match)
    //     current_token = self.tokenizer.get_current_token().get_token()
    //
    //     if item == "token":
    //         if current_token == match:
    //             self.emit()
    //         else:
    //             self.syntax_error(current_token)
    //     elif item == "type":
    //         if self.tokenizer.get_current_token().token_type(current_token) == match:
    //             self.emit()
    //         else:
    //             self.syntax_error(current_token + " " + match)
    //
    //
void compileClass(compilation_engine *compiler) {
    class_symbol_table = create_symbol_table();
    subroutine_symbol_table = create_symbol_table();

    token** tokens_list = compiler->tokenizer->tokenized_tokens;

    for (int i = 0; i < compiler->tokenizer->next_index; i++) {
        fprintf(compiler->out_file, "%s, %s\n", tokens_list[i]->item, tokens_list[i]->type);
        // printf("%s, %s\n", tokens_list[i]->item, tokens_list[i]->type);
        define_row(tokens_list[i]->item, tokens_list[i]->type, "kind unknown", class_symbol_table);
    }

    for (int j = 0; j < class_symbol_table->next_index; j++) {
        printf("name: %s, ", class_symbol_table->rows[j]->name);
        printf("type: %s, ", class_symbol_table->rows[j]->type);
        printf("kind: %s, ", class_symbol_table->rows[j]->kind);
        printf("n: %d\n", class_symbol_table->rows[j]->n);
    }

    // 'class' className '{' classVarDec* subroutineDec* '}'
    // self.out_file.write("<class>")
    // self.inc_indent()
    // self.tokenizer.advance()
    //
    // # 'class'
    // self.eat("token", "class")
    //
    // # className
    // self.eat("type", "identifier")
    //
    // # '{'
    // self.eat("token", "{")
    //
    // # classVarDec*
    // while self.compileClassVarDec():
    //     pass
    //
    // # subroutineDec*
    // while self.compileSubroutine():
    //     pass
    //
    // # '}'
    // self.eat("token", "}")
    //
    // self.dec_indent()
    // self.out_file.write(f"\n{self.indent}</class>")
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
    // def compileClassVarDec(self):
    //
    //     # ('static' | 'field') type varName (',' varName)* ';'
    //
    //     # ('static' | 'field')
    //     if (self.tokenizer.get_current_token().get_token() == "static" or
    //         self.tokenizer.get_current_token().get_token() == "field"):
    //
    //         self.out_file.write(f"\n{self.indent}<classVarDec>")
    //         self.inc_indent()
    //
    //         self.emit()
    //
    //         # type { 'int' | 'char' | 'boolean' | varName }
    //         if (self.tokenizer.get_current_token().get_token() in Token.var_types
    //             or self.tokenizer.get_current_token().get_type() == "identifier"):
    //
    //             self.emit()
    //
    //         # varName
    //         self.eat("type", "identifier")
    //
    //         #(',' varName)*
    //         while self.check_for_one_or_more_identifiers():
    //             pass
    //
    //         # ';'
    //         self.eat("token", ";")
    //
    //         self.dec_indent()
    //         self.out_file.write(f"\n{self.indent}</classVarDec>")
    //     else:
    //         return False
    //
    //
    // def compileSubroutine(self):
    //     # ('constructor' | 'function' | 'method') ('void' | type) subroutineName '(' parameterList ')' '{' varDec* statements '}'
    //
    //     # ('constructor' | 'function' | 'method')
    //     if self.tokenizer.get_current_token().get_token() in Token.function_types:
    //         self.out_file.write(f"\n{self.indent}<subroutineDec>")
    //         self.inc_indent()
    //
    //         self.emit()
    //
    //         # ('void' | type)
    //         if (self.tokenizer.get_current_token().get_token() in Token.var_types
    //             or self.tokenizer.get_current_token().get_token() == "void"):
    //
    //             self.emit()
    //
    //             # subroutineName
    //             self.eat("type", "identifier")
    //
    //             # '('
    //             self.eat("token", "(")
    //
    //             # parameterList
    //             self.out_file.write(f"\n{self.indent}<parameterList>")
    //             self.inc_indent()
    //             while self.compileParameterList():
    //                 pass
    //             self.dec_indent()
    //             self.out_file.write(f"\n{self.indent}</parameterList>")
    //
    //             # ')'
    //             self.eat("token", ")")
    //
    //             # subroutineBody = '{' varDec* statements '}'
    //             self.out_file.write(f"\n{self.indent}<subroutineBody>")
    //             self.inc_indent()
    //
    //             # '{'
    //             self.eat("token", "{")
    //
    //             # varDec*
    //             while self.compileVarDec():
    //                 pass
    //
    //             # statements
    //             self.out_file.write(f"\n{self.indent}<statements>")
    //             self.inc_indent()
    //             while self.compileStatements():
    //                 pass
    //             self.dec_indent()
    //             self.out_file.write(f"\n{self.indent}</statements>")
    //
    //             # '}'
    //             self.eat("token", "}")
    //
    //             self.dec_indent()
    //             self.out_file.write(f"\n{self.indent}</subroutineBody>")
    //
    //         else:
    //             self.syntax_error(self.tokenizer.get_current_token().get_token())
    //
    //         self.dec_indent()
    //         self.out_file.write(f"\n{self.indent}</subroutineDec>")
    //
    //         return True
    //     else:
    //         return False
    //
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
    // def syntax_error(self, place):
    //     print("syntax error: " + str(place))
