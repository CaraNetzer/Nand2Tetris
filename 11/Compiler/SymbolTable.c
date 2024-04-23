#include <SymbolTable.h>

symbol_table* create_symbol_table() {
    symbol_table *table = calloc(1, sizeof(symbol_table));
    table->max_rows = 2000;
    return table;
}

void startSubroutine() { // TODO figure out what this method needs to do differently
    symbol_table *table = calloc(1, sizeof(symbol_table));
    table->max_rows = 2000;
    return table;
}

void define(char *name, char *type, char *kind, symbol_table *table) {

    // if (0 != strcmp(token, "")) {
        table->rows[table->next_index++]->name = strdup(name);
        table->rows[table->next_index++]->type = strdup(type);
        table->rows[table->next_index++]->kind = strdup(kind);
    // }

    if(table->next_index > table->max_rows) {
        char **new_table = reallocarray(table->rows, table->max_rows * 2, sizeof(char*));
    
        if(new_table) {
            table->rows = new_table;
            table->max_rows *= 2;
        } else {
            perror(*new_table);
            exit(1);
        }
    }
}

int var_count(char *kind, symbol_table *table) {
    //how many of {kind} are in the current scope's symbol table?

    int count = 0;

    int num_of_rows = sizeof(table) / sizeof(symbol_table_row);
    for (int i = 0; i < num_of_rows; i++) {
        if (table->rows[i]->kind == kind) {
            count++;
        }
    }

    return count;

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

    char *index;

    find_property(name, table, index);

    return index;
}


#define find_property(name, table, property) (                  \
                                                                \
    int num_of_rows = sizeof(table) / sizeof(symbol_table_row); \
                                                                \
    for (int i = 0; i < num_of_rows; i++) {                     \
        if (table->rows[i]->name == name) {                     \
            property = table->rows[i]->property;                \
        }                                                       \
    }                                                           \
)