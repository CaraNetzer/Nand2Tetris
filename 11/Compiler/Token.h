#include <stdbool.h>

typedef struct {
    char *type;
    char *item;
} token;

token* create_token(char *in_token);
char* to_str(token *token);
char* token_type(char *token); 
bool array_contains(char **array, int size, char *item);
