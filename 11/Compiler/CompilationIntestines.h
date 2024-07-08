#ifndef _CompilationIntestines_H
#define _CompilationIntestines_H true

bool check_for_comma();
bool check_for_one_or_more_identifiers(char *type, char *kind, char *scope, int *var_count);
bool check_for_one_or_more_expressions();
bool compileExpressionList(int* arg_count);
void compileParameterList();
bool compileParameterOrVar(char *varType, char *compileType);
bool compileOpTerm();
bool compileIndexedExpression();

#endif //_CompilationIntestines_H