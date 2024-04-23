#include <VMWriter.h>

vm_writer* create_vm_writer(char *file_path) {

    vm_writer *writer = calloc(1, sizeof(vm_writer));
    writer->out_file = fopen(file_path, "w");
    if(!writer->out_file) {
        perror(file_path);
        exit(1);
    }
    return writer;
}

// TODO do I have to pass the wv_writer obct to all of these or is there a way to call a method on an obct??
void write_push(vm_writer *writer, char *segment, int index) {
    fprintf(writer->out_file, "push %s %d", segment, index);
}

void write_pop(vm_writer *writer, char *segment, int index) {
    fprintf(writer->out_file, "pop %s %d", segment, index);
}

void write_arithmetic(vm_writer *writer, char *command) {
    fprintf(writer->out_file, "%s", command);
}

void write_label(vm_writer *writer, char *label) {
    fprintf(writer->out_file, "(%s)", label);
}

void write_goto(vm_writer *writer, char *label) {
    fprintf(writer->out_file, "goto %s", label);
}

void write_if(vm_writer *writer, char *label) {
    fprintf(writer->out_file, "if %s", label);
}

void write_call(vm_writer *writer, char *name, int n_args) {
    fprintf(writer->out_file, "call %s %d", name, n_args);
}

void write_function(vm_writer *writer, char *name, int n_locals) {
    fprintf(writer->out_file, "function %s %d", name, n_locals);
}

void write_return(vm_writer *writer) {
    fprintf(writer->out_file, "return");
}

void close(vm_writer *writer) {
    fclose(writer->out_file);
}
