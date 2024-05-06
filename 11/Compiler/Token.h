#include <stdbool.h>

typedef struct {
    char *type;
    char *item;
} token;

extern char *variable_declarations[];
extern char *var_types[];
extern char *function_types[];
extern char *statement_types[];
extern char *operators[];
extern char *unary_operators[];

token* create_token(char *in_token);
char* to_str(token *token);
char* token_type(char *token); 
bool array_contains(char **array, int size, char *item);
