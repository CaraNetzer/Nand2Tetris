#include <stdio.h>

typedef struct {
    FILE* out_file;
} vm_writer;

vm_writer* create_vm_writer(char *file_path);

void write_push(vm_writer *writer, char *segment, int index);
void write_pop(vm_writer *writer, char *segment, int index);
void write_arithmetic(vm_writer *writer, char *command);
void write_label(vm_writer *writer, char *label);
void write_goto(vm_writer *writer, char *label);
void write_if(vm_writer *writer, char *label);
void write_call(vm_writer *writer, char *name, int n_args);
void write_function(vm_writer *writer, char *name, int n_locals);
void write_return(vm_writer *writer);
void wrtier_close(vm_writer *writer);
