#include "CompileStatements.h"
#include "CompilationEngine.h"
#include "CompilationIntestines.h"
#include <stdlib.h>
#include <string.h>

bool compileStatements() {

    // (letStatement | ifStatement | whileStatement | doStatement | returnStatement)*
    if (array_contains(statement_types, 6, current_token->item)) {

        if(!strcmp(current_token->item, "let")) {
            compileLet();
        }

        if (!strcmp(current_token->item, "if")) {
            compileIf();
        }

        if (!strcmp(current_token->item, "while")) {
            compileWhile();
        }

        if (!strcmp(current_token->item, "do")) {
            compileDo();
        }

        if (!strcmp(current_token->item, "return")) {
            compileReturn();
        }

        return true;
    } else {
        return false;
    }
}

bool compileDo() {

    // 'do' subroutineCall = subroutineName '(' expressionList ')' | (className | varName) '.'

    // subroutineName '(' expressionList ')' ';'

    // 'do'
    check_token("token", "do", "misc");
    advance_token();

    // subroutineName | className | varName

    check_token("type", "identifier", "use");
    char *methodOrObject = current_token->item;

    //varName (ie square1.draw())
    bool in_sub = find_by_name(methodOrObject, subroutine_symbol_table);
    bool in_class = find_by_name(methodOrObject, class_symbol_table);
    if(in_sub || in_class) {
        write_push(writer, current_token);
        advance_token();
        if (in_sub) {
          methodOrObject = type_of(methodOrObject, subroutine_symbol_table);
        } else {
          methodOrObject = type_of(methodOrObject, class_symbol_table);
        }
    }

    // subroutineCall
    // subroutineName (ie draw()) and className (ie Square.new()) handled in here
    printf("68: methodOrObject - %s, current_token - %s\n", methodOrObject, current_token->item);
    compileSubroutineCall(methodOrObject, true, NULL);

    //';'
    check_token("token", ";", "misc");
    advance_token();

    return true;
}

bool compileLet() {
    // 'let' varName ('[' expression ']')? '=' expression ';'
    bool array_assignment = false;

    // 'let'
    check_token("token", "let", "misc");
    advance_token();

    // varName
    check_token("type", "identifier", "use");
    token *popVar = current_token;
    char *segment;
    int index;
    if(find_by_name(popVar->item, subroutine_symbol_table)) {
        segment = kind_of(popVar->item, subroutine_symbol_table);
        index = index_of(popVar->item, subroutine_symbol_table);
    } else if(find_by_name(popVar->item, class_symbol_table)) {
        segment = kind_of(popVar->item, class_symbol_table);
        index = index_of(popVar->item, class_symbol_table);
    }
    advance_token();
    if(!strcmp(current_token->item, "[")) {
        write_push(writer, popVar);
    }

    // '[' expression ']'
    if (compileIndexedExpression()) {
        array_assignment = true;
    }

    // '='
    check_token("token", "=", "misc");
    advance_token();

    // expression
    compileExpression();

    if(array_assignment) {
        write_pop(writer, "temp", 0);
        write_pop(writer, "pointer", 1);
        write_push_specific(writer, "temp", 0);
        write_pop(writer, "that", 0);
    } else {
        write_pop(writer, segment, index);
    }

    // ';'
    check_token("token", ";", "misc");
    advance_token();

    return true;
}

bool compileWhile() {

    int whileId = whileCounter++;

    write_label(writer, "BEGINWHILE", whileId);

    // 'while' '(' expression ')' '{' statements '}'
    check_token("token", "while", "misc");
    advance_token();

    check_token("token", "(", "misc");
    advance_token();

    compileExpression();

    write_if(writer, "ENDWHILE", whileId);

    check_token("token", ")", "misc");
    advance_token();

    check_token("token", "{", "misc");
    advance_token();

    while (compileStatements()) {
        continue;
    }

    write_goto(writer, "BEGINWHILE", whileId);

    check_token("token", "}", "misc");
    advance_token();

    write_label(writer, "ENDWHILE", whileId);

    return true;
}

bool compileReturn() {
    // 'return' expression
    check_token("token", "return", "misc");
    advance_token();

    while (compileExpression()) {
        continue;
    }

    check_token("token", ";", "misc");
    advance_token();

    // printf("%s %s\n", subroutineKind, subroutineType);
    if(!strcmp(subroutineKind, "constructor")) {
        // this is handled in write_push for keywords now
        // constructors are expected to explicitly return 'this'
        // write_push_specific(writer, "pointer", 0);
    }
    else if (!strcmp(subroutineType, "void")) {
        write_push_specific(writer, "constant", 0);
    }

    write_return(writer);

    return true;
}

bool compileElse() {
    // 'else' '{' statements '}'
    if (!strcmp(current_token->item, "else")) {
        // 'else'
        advance_token();

        // '{'
        check_token("token", "{", "misc");
        advance_token();

        // statements
        while (compileStatements()) {
            continue;
        }

        // '}'
        check_token("token", "}", "misc");
        advance_token();

        return true;
    } else {
        return false;
    }
}

bool compileIf() {

    int ifId = ifCounter++;

    // 'if' '(' expression ')' '{' statements '}' ('else' '{' statements '}')?

    // 'if'
    check_token("token", "if", "misc");
    advance_token();

    // '('
    check_token("token", "(", "misc");
    advance_token();

    compileExpression();

    // ')'
    check_token("token", ")", "misc");
    advance_token();

    write_if(writer, "ELSE", ifId);


    // '{'
    check_token("token", "{", "misc");
    advance_token();

    // statements
    while (compileStatements()) {
        continue;
    }

    write_goto(writer, "ENDIF", ifId);

    // '}'
    check_token("token", "}", "misc");
    advance_token();

    // 'else' '{' statements '}'
    write_label(writer, "ELSE", ifId);
    while (compileElse()) {
        continue;
    }

    write_label(writer, "ENDIF", ifId);

    return true;
}