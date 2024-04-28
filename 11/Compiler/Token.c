#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "Token.h"

char *var_types[] = { "int", "char", "boolean" };
char *function_types[] = { "function", "constructor", "method" };
char *operators[] = { "+", "-", "*", "/", "&", "|", "<", ">", "=", "~" };
char *statement_types[] = { "let", "do", "if", "else", "while", "return" };
char *unary_operators[] = { "-", "~" };
char *other_keywords[] = { "class", "field", "static", "var", "void", "true", "false", "null", "this" }; //.union(var_types).union(function_types).union(statement_types);
char *symbols[] = { "{", "}", "(", ")", "[", "]", ".", ",", ";" }; //.union(operators)


bool array_contains(char **array, int size, char *item) {
    for(int i = 0; i < size; i++) {
        if (array[i] == item) {
            return true;
        }
    }
    return false;
}


char* token_type(char *token) {
    if(array_contains(other_keywords, 9, token) || array_contains(var_types, 3, token) || array_contains(function_types, 3, token) || array_contains(statement_types, 6, token)) {
        return "keyword";
    } else if (array_contains(symbols, 9, token) || array_contains(operators, 10, token)) {
        return "symbol";
    } else if (isdigit(token[0])) {
        return "integerConstant";
    } else if (strstr(" ", token)) { // TODO find better way to decide if a token is a string
        return "stringConstant";
    } else if (!isdigit(token[0])) {
        return "identifier";
    }

    return "none";
}

char* to_str(token *token) {
    char buf[BUFSIZ];
    int return_value = snprintf(buf, BUFSIZ - 1, "<%s> %s </%s>", token->type, token->token, token->type);

    if (return_value > 0) return strdup(buf);
    else return NULL;
}
