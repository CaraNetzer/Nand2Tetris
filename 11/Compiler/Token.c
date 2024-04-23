
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
char *symbols[] = { '{', '}', '(', ')', '[', ']', '.', ',', ';' }; //.union(operators)

char* token_type(char *token) {
    if(strstr(token, other_keywords) || strstr(token, var_types) || strstr(token, function_types) || strstr(token, statement_types)) {
        return "keyword";
    } else if (strstr(token, symbols) || strstr(token, operators)) {
        return "symbol";
    } else if (isnum(token)) {
        return "integerConstant";
    } else if (strstr(" ", token)) {
        return "stringConstant";
    } else if (!isnum(token[0])) {
        return "identifier";
    }
}

char* to_str(token *token){
     return sprintf("<%s> %s </%s>", token->type, token->token, token->type);
}