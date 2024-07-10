#include "CompilationEngine.h"
#include "CompilationIntestines.h"
#include <stdlib.h>
#include <string.h>

bool check_for_comma() {
  if (!strcmp(current_token->item, ",")) {
      advance_token();
      return true;
  } else {
      return false;
  }
}

bool check_for_one_or_more_identifiers(char *type, char *kind, char *scope, int *var_count) {

    if(!check_for_comma()) {
      return false;
    }

    // identifier
    if (!strcmp(current_token->type, "identifier")) {

        // varname
        char *name = current_token->item;
        check_token("type", "identifier", "define");
        advance_token();

        if(!strcmp(scope, "class")) {
            define_row(name, type, kind, class_symbol_table);
        }
        else {
            define_row(name, type, kind, subroutine_symbol_table);
        }
        var_count += 1;

        return true;
    } else {
        syntax_error(current_token->item, "identifier");
        return false;
    }
}

bool compileParameterOrVar(char **varType, char *compileType) {

  if (array_contains(var_types, 4, current_token->item) || !strcmp(current_token->type, "identifier")) {

    *varType = current_token->item;
    advance_token();

    // varname
    char *name = current_token->item;
    check_token("type", "identifier", "define");
    advance_token();

    if(!strcmp(compileType, "param")) {
      define_row(name, *varType, "argument", subroutine_symbol_table);
    } else if(!strcmp(compileType, "var")) {
      define_row(name, *varType, "local", subroutine_symbol_table);
    }

    printf("in type: '%s'\n", *varType);
    return true;
  } else {
    // if we're compiling an empty parameter list, no syntax error
    if(!(!strcmp(compileType, "param") && !strcmp(current_token->item, ")"))) {
      syntax_error(current_token->item, "type");
    }
    return false;
  }
}

void compileParameterList() {
  // ((type varName) (',' type varName)*)?

  char *type;
  bool output = compileParameterOrVar(&type, "param");

  // (',' varName)*
  do {
    if(check_for_comma()) {
      printf("after comma - current token: %s, type: %s\n", current_token->item, current_token->type);
      output = compileParameterOrVar(&type, "param");
    } else {
      output = false;
    }
  } while(output);
}


bool check_for_one_or_more_expressions() {

    if(!check_for_comma()) {
      return false;
    }

    if (compileTerm()) {
        // (op term)*
        while (compileOpTerm()) {
            continue;
        }
    } else {
        syntax_error(current_token->item, "op term after ,");
    }

    return true;
}


bool compileExpressionList(int* arg_count) {
    // (expression (',' expression)*)?

    // expression = term (op term)*
    if (compileTerm()) {

        *arg_count += 1;
        // (op term)*
        while (compileOpTerm()) {
            continue;
        }

        // (',' expression)*
        while (check_for_one_or_more_expressions()) {
            *arg_count += 1;
            continue;
        }

        return true;
    } else {
        return false;
    }
}

bool compileIndexedExpression() {
    // '[' expression ']'
    if (!strcmp(current_token->item, "[")) {

        advance_token();

        compileExpression();

        check_token("token", "]", "misc");
        advance_token();

        write_arithmetic(writer, "+", false);

        return true;
    } else {
        return false;
    }
}

bool compileOpTerm() {
  // op term
  if(array_contains(operators, 10, current_token->item)) {
    // op
    check_token("type", "symbol", "misc");
    char *op = current_token->item;
    advance_token();

    // term
    compileTerm();

    write_arithmetic(writer, op, false);

    return true;
  } else {
      return false;
  }
}

char* combineSubroutineName(char* objectOrClass, char* method) {
  char buffer[BUFSIZ];
  strcpy(buffer, objectOrClass);
  strcat(buffer, ".");
  strcat(buffer, method);
  return strdup(buffer);
}