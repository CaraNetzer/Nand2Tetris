#include <ctype.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    char *type;
    char *token;
} token;

char *var_types[] = { "int", "char", "boolean" };
char *function_types[] = { "function", "constructor", "method" };
char *operators[] = { "+", "-", "*", "/", "&", "|", "<", ">", "=", "~" };
char *statement_types[] = { "let", "do", "if", "else", "while", "return" };
char *unary_operators[] = { "-", "~" };
char *other_keywords[] = { "class", "field", "static", "var", "void", "true", "false", "null", "this" }; //.union(var_types).union(function_types).union(statement_types);
char *symbols[] = { "{", "}", "(", ")", "[", "]", ".", ",", ";" }; //.union(operators)

char* token_type(char *token) {
    // TODO can't strstr an array
    if(strstr(token, other_keywords) || strstr(token, var_types) || strstr(token, function_types) || strstr(token, statement_types)) {
        return "keyword";
    } else if (strstr(token, symbols) || strstr(token, operators)) {
        return "symbol";
    } else if (isdigit(token[0])) {
        return "integerConstant";
    } else if (strstr(" ", token)) {
        return "stringConstant";
    } else if (!isdigit(token[0])) {
        return "identifier";
    }

    return "none";
}

char* to_str(token *token){
    char buf[BUFSIZ];
    int return_value = snprintf(buf, BUFSIZ - 1, "<%s> %s </%s>", token->type, token->token, token->type);

    if (return_value > 0) return strdup(buf);
    else return NULL;
}
