#include "SymbolTable.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

symbol_table* create_symbol_table() {
    symbol_table *table = calloc(1, sizeof(symbol_table));
    table->max_rows = 2000;
    table->rows = calloc(table->max_rows, sizeof(symbol_table_row));
    table->next_index = 0;
    return table;
}

symbol_table* startSubroutine(symbol_table *table) {
    for(int i = 0; i < table->next_index; i++) {
        clear_symbol_table_row(table->rows[i]);
    }
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


symbol_table_row* alloc_new_row(symbol_table *table) {
    symbol_table_row* row = calloc(1, sizeof(symbol_table_row));
    return row;
}

void define_row(char *name, char *type, char *kind, symbol_table *table) {

    if (0 != strcmp(name, "")) {
        symbol_table_row *row;
        row = alloc_new_row(table);
        row->name = strdup(name);
        row->type = strdup(type);
        row->kind = strdup(kind);
        row->n = var_count(kind, table);
        table->rows[table->next_index++] = row;
    }

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
        if (!strcmp(table->rows[i]->kind, kind)) {
            count++;
        }
    }

    return count;

}

bool find_by_name(char *name, symbol_table *table) {

  for (int i = 0; i < table->next_index; i++) {
    if (!strcmp(table->rows[i]->name, name)) {
      return true;
    }
  }
  return false;

}

#define find_property(name, table, property) {                  \
                                                                \
    for (int i = 0; i < table->next_index; i++) {               \
      if (!strcmp(table->rows[i]->name, name)) {                \
          property = table->rows[i]->property;                  \
        }                                                       \
    }                                                           \
}

char* kind_of(char *name, symbol_table *table) {

    char *kind = "NONE";

    find_property(name, table, kind);

    if(!strcmp(kind, "field")) {
        kind = "this";
    }

    return kind;

}

char* type_of(char *name, symbol_table *table) {

    char *type = "NONE";

    find_property(name, table, type);

    return type;
}

int index_of(char *name, symbol_table *table) {

    int n;

    find_property(name, table, n);

    return n;
}
