#include <stdio.h>

typedef struct {
    FILE* out_file;
} vm_writer;

vm_writer* create_vm_writer(char *file_path);

void write_push(char *segment, int index);
void write_pop(char *segment, int index);
void write_arithmetic(char *command);
void write_label(char *label);
void write_goto(char *label);
void write_if(char *label);
void write_call(char *name, int n_args);
void write_function(char *name, int n_locals);
void write_return();
void close();