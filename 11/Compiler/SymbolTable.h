#include <stdio.h>
#include <stdbool.h>

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

extern symbol_table *class_symbol_table;
extern symbol_table *subroutine_symbol_table;

symbol_table* create_symbol_table();

symbol_table* startSubroutine(symbol_table *table);

void clear_symbol_table_row(symbol_table_row *row);
void define_row(char *name, char *type, char *kind, symbol_table *table);
symbol_table_row* alloc_new_row(symbol_table *table);

int var_count(char *kind, symbol_table *table);
bool find_by_name(char *name, symbol_table *table);

char* kind_of(char *name, symbol_table *table);
char* type_of(char *name, symbol_table *table);
int index_of(char *name, symbol_table *table);
