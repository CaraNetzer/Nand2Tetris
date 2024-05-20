#include "CompilationEngine.h"
#include <stdio.h>

typedef struct {
  FILE *out_file;
} vm_writer;

vm_writer *create_vm_writer(char *file_path);
void write_push(vm_writer * writer, token * token);
void write_arithmetic(vm_writer * writer, char *command, bool unary);
