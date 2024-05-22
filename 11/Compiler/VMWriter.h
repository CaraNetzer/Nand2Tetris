#ifndef _VMWriter_H
#define _VMWriter_H true

#include "SymbolTable.h"
#include "CompilationEngine.h"
#include <stdio.h>

typedef struct {
  FILE *out_file;
} vm_writer;

vm_writer *create_vm_writer(char *file_path);
void write_push(vm_writer * writer, token * token);
void write_arithmetic(vm_writer * writer, char *command, bool unary);
void write_if(vm_writer *writer, char *label, int num);
void write_goto(vm_writer *writer, char *label, int num);
void write_label(vm_writer *writer, char *label, int num);

#endif //_VMWriter_H
