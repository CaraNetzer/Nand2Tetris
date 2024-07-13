#ifndef _Token_H
#define _Token_H

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

bool equal(char *first_item, char *second_item);
bool not_equal(char *first_item, char *second_item);
token* create_token(char *in_token);
char* to_str(token *token);
char* token_type(char *token);
bool array_contains(char **array, int size, char *item);

#endif //_Token_H
