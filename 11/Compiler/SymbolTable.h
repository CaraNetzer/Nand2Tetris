#include <stdio.h>

typedef struct {
    char *name;
    char *type;
    char *kind;
    int n;
} symbol_table_row;

typedef struct {
    symbol_table_row **rows;
    int next_index;
    int max_rows;
} symbol_table;

symbol_table* create_symbol_table();

symbol_table* startSubroutine(symbol_table *table);

void define(char *name, char *type, char *kind, symbol_table *table);

int var_count(char *kind, symbol_table *table);

char* kind_of(char *name, symbol_table *table);
char* type_of(char *name, symbol_table *table);
int index_of(char *name, symbol_table *table);