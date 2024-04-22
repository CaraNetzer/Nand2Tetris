#include <cstdio>

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

symbol_table* create_symbol_table;

void startSubroutine;

void define(char *name, char *type, char *kind);

int var_count(char *kind);

char* kind_of(char *name);
char* type_of(char *name);
int index_of(char *name);