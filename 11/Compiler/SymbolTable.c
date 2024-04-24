#include "SymbolTable.h"
#include <stdlib.h>
#include <string.h>

symbol_table* create_symbol_table() {
    symbol_table *table = calloc(1, sizeof(symbol_table));
    table->max_rows = 2000;
    return table;
}

symbol_table* startSubroutine(symbol_table *table) { 
    // TODO free prior table's data
    table->next_index = 0;
    return table;
}

void clear_symbol_table_row(symbol_table_row *row) {
    if(!row) return;
    
    if(row->name) free(row->name);
    if(row->type) free(row->type);
    if(row->kind) free(row->kind);

    row->name = row->type = row->kind = NULL;
}


symbol_table_row* st_next_row() {
    // symbol_table_row* row = calloc();
    // set next index
}

void define(char *name, char *type, char *kind, symbol_table *table) {

    // if (0 != strcmp(token, "")) {
    // symbol_table_row *row;
    // row = table->rows[table->next_index++] 
    // row = st_next_row(table)
        table->rows[table->next_index++]->name = strdup(name);
        table->rows[table->next_index++]->type = strdup(type);
        table->rows[table->next_index++]->kind = strdup(kind);
    // }

    if(table->next_index > table->max_rows) {
        symbol_table_row **new_table = reallocarray(table->rows, table->max_rows * 2, sizeof(char*));
    
        if(new_table) {
            table->rows = new_table;
            table->max_rows *= 2;
        } else {
            perror("realloc symbol table");
            exit(1);
        }
    }
}

int var_count(char *kind, symbol_table *table) {
    //how many of {kind} are in the current scope's symbol table?

    int count = 0;

    for (int i = 0; i < table->next_index; i++) {
        if (table->rows[i]->kind == kind) {
            count++;
        }
    }

    return count;

}

#define find_property(name, table, property) {                  \
                                                                \
    for (int i = 0; i < table->next_index; i++) {               \
        if (table->rows[i]->name == name) {                     \
            property = table->rows[i]->property;                \
        }                                                       \
    }                                                           \
}

char* kind_of(char *name, symbol_table *table) {

    char *kind;

    find_property(name, table, kind);

    return kind;

}

char* type_of(char *name, symbol_table *table) {

    char *type;

    find_property(name, table, type);

    return type;
}

int index_of(char *name, symbol_table *table) {

    int n;

    find_property(name, table, n);

    return n;
}


