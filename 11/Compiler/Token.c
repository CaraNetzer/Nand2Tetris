#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Token.h"


char *var_types[] = { "int", "char", "boolean", "void" };
char *function_types[] = { "function", "constructor", "method" };
char *operators[] = { "+", "-", "*", "/", "&", "|", "<", ">", "=", "~" };
char *statement_types[] = { "let", "do", "if", "else", "while", "return" };
char *unary_operators[] = { "-", "~" };
char *other_keywords[] = { "class", "void", "true", "false", "null", "this" }; //.union(var_types).union(function_types).union(statement_types);
char *variable_declarations[] = { "field", "static", "var" }; //.union(var_types).union(function_types).union(statement_types);
char *symbols[] = { "{", "}", "(", ")", "[", "]", ".", ",", ";" }; //.union(operators)

// this is in the token class so it gets included in all the other files
bool equal(char *first_item, char *second_item) {
  return !strcmp(first_item, second_item);
}
bool not_equal(char *first_item, char *second_item) {
  return strcmp(first_item, second_item);
}

token *create_token(char *in_token) {
    token *new_token = malloc(sizeof(token));

    if(!new_token) {
        perror("malloc failed for new tokenized token");
        exit(1);
    }

    new_token->item = in_token;
    new_token->type = token_type(in_token);

    return new_token;
}

bool array_contains(char **array, int size, char *item) {
    for(int i = 0; i < size; i++) {
        if (equal(array[i], item)) {
            return true;
        }
    }
    return false;
}


char* token_type(char *token) {
    if(array_contains(other_keywords, 9, token)
    || array_contains(var_types, 3, token)
    || array_contains(function_types, 3, token)
    || array_contains(statement_types, 6, token)
    || array_contains(variable_declarations, 3, token)) {
        return "keyword";
    } else if (array_contains(symbols, 9, token) || array_contains(operators, 10, token)) {
        return "symbol";
    } else if (isdigit(token[0])) {
        return "integerConstant";
    } else if (!strncmp("\"", token, 1)) {
        return "stringConstant";
    } else if (!isdigit(token[0])) {
        return "identifier";
    }

    return "none";
}


char* to_str(token *token) {
    char buf[BUFSIZ];
    int return_value = snprintf(buf, BUFSIZ - 1, "<%s> %s </%s>", token->type, token->item, token->type);

    if (return_value > 0) return strdup(buf);
    else return NULL;
}
