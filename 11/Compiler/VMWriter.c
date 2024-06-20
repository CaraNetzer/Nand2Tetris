#include "VMWriter.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

vm_writer *create_vm_writer(char *file_path) {

    vm_writer *writer = calloc(1, sizeof(vm_writer));
    writer->out_file = fopen(file_path, "w");
    if(!writer->out_file) {
        perror(file_path);
        exit(1);
    }
    return writer;
}

char *op_to_str(char *op) {
    if (!strcmp(op, "+")) {
        return "add";
    } else if (!strcmp(op, "-")) {
        return "sub";
    } else if (!strcmp(op, "=")) {
        return "eq";
    } else if (!strcmp(op, ">")) {
        return "gt";
    } else if (!strcmp(op, "<")) {
        return "lt";
    } else if (!strcmp(op, "&")) {
        return "and";
    } else if (!strcmp(op, "|")) {
        return "or";
    } else if (!strcmp(op, "*")) {
        return "call Math.multiply 2";
    } else if (!strcmp(op, "/")) {
        return "call Math.divide 2";
    } else {
        printf("unrecognized symbol: %s\n", op);
        return "error";
    }
}

char *unary_op_to_str(char *op) {
    if (!strcmp(op, "-")) {
        return "neg";
    } else if (!strcmp(op, "~")) {
        return "not";
    } else {
        printf("unrecognized symbol: %s\n", op);
        return "error";
    }
}

int translate_keywords(char *word) {
    if (!strcmp(word, "true")) {
        return 1;
    } else if (!strcmp(word, "false")) {
        return 0;
    } else if (!strcmp(word, "null")) {
        return 0;
    } else {
        printf("unrecognized keyword: %s\n", word);
        return 2;
    }
}

void write_push_specific(vm_writer *writer, char *segment, int index) {
    fprintf(writer->out_file, "push %s %d\n", segment, index);
}

void write_push(vm_writer *writer, token *token) {
    if (!strcmp(token->type, "integerConstant")) {
        fprintf(writer->out_file, "push constant %s\n", token->item);

    } else if (!strcmp(token->type, "stringConstant")) {
        int length = strlen(token->item) - 2; // minus the quotation marks
        fprintf(writer->out_file, "push constant %d\n", length);
        fprintf(writer->out_file, "call String.new 1\n");
        for (char *character = ++token->item; *(character + 1) != '\0'; character++) {
            fprintf(writer->out_file, "push constant %1$3d //'%1$c'\n", *character);
            fprintf(writer->out_file, "call String.appendChar 2\n");
        }
        // TODO if there's an assignment, pop segment index --> maybe add to write_push_specific
    } else if (!strcmp(token->type, "identifier")) {

        char *segment;
        int index;
        // look up string const in symbol tables
        if (find_by_name(token->item, subroutine_symbol_table)) {
            segment = kind_of(token->item, subroutine_symbol_table);
            index = index_of(token->item, subroutine_symbol_table);
            fprintf(writer->out_file, "push %s %d // %s\n", segment, index, token->item);
        } else if (find_by_name(token->item, class_symbol_table)) {
            segment = kind_of(token->item, class_symbol_table);
            index = index_of(token->item, class_symbol_table);
            fprintf(writer->out_file, "push %s %d // %s\n", segment, index, token->item);
        } else {
            syntax_error("identifier not found in either symbol table", "");
        }

    } else if (!strcmp(token->type, "keyword")) { // true false null this
        if(strcmp(token->item, "this") != 0) {
            int value = translate_keywords(token->item);
            fprintf(writer->out_file, "push constant %d\n", value);
            if (!strcmp(token->item, "true")) {
                write_arithmetic(writer, "-", true);
            }
        }
    }
}

void write_pop(vm_writer *writer, char *segment, int index) {
    fprintf(writer->out_file, "pop %s %d\n", segment, index);
}

void write_arithmetic(vm_writer *writer, char *command, bool unary) {
    char *translated_command;
    if (unary) {
        translated_command = unary_op_to_str(command);
    } else {
        translated_command = op_to_str(command);
    }
    fprintf(writer->out_file, "%s\n", translated_command);
}

void write_label(vm_writer *writer, char *label, int num) {
    fprintf(writer->out_file, "label %s%d\n", label, num);
}

void write_goto(vm_writer *writer, char *label, int num) {
    fprintf(writer->out_file, "goto %s%d\n", label, num);
}

void write_if(vm_writer *writer, char *label, int num) {
    fprintf(writer->out_file, "not\n");
    fprintf(writer->out_file, "if-goto %s%d\n", label, num);
}

void write_call(vm_writer *writer, char *name, int n_args) {
    fprintf(writer->out_file, "call %s %d\n", name, n_args);
}

void write_function(vm_writer *writer, char *name, int n_locals) {
    fprintf(writer->out_file, "function %s %d\n", name, n_locals);
}

void write_return(vm_writer *writer) {
    fprintf(writer->out_file, "return\n");
}

void writer_close(vm_writer *writer) {
    // TODO double check if fclose frees memory
    fclose(writer->out_file);
    writer->out_file = NULL;
}

