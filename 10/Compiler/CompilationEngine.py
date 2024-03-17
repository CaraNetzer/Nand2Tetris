import LinkedList
import Token

class CompilationEngine:
    stack = LinkedList.LinkedList()
    level = 0
    indent = 0
    tab = "\t"

    def __init__(self, in_tokenizer, out_file):
        self.tokens = in_tokenizer.tagged_tokens
        self.tokenizer = in_tokenizer
        self.out_file = open(out_file, "at", buffering = 1024)

    def inc_indent(self):
        self.level = self.level + 1
        self.indent = self.tab * self.level

    def dec_indent(self):
        self.level = self.level - 1
        self.indent = self.tab * self.level

    def emit(self):
        print((f"\n{self.indent}{self.tokenizer.get_current_token_str()}"))
        self.out_file.write(f"\n{self.indent}{self.tokenizer.get_current_token_str()}")
        self.tokenizer.advance()

    def eat(self, item, match):
        print("eat: " + match)
        current_token = self.tokenizer.get_current_token().get_token()

        if item == "token":
            if current_token == match:
                self.emit()
            else:
                self.syntax_error(current_token)
        elif item == "type":
            if self.tokenizer.get_current_token().token_type(current_token) == match:
                self.emit()
            else:
                self.syntax_error(current_token + " " + match)


    def compileClass(self):
        # 'class' className '{' classVarDec* subroutineDec* '}'
        self.out_file.write("<class>")
        self.inc_indent()
        self.tokenizer.advance()

        # 'class'
        # print(self.tokenizer.tagged_tokens[0].get_token())
        self.eat("token", "class")

        # className
        self.eat("type", "identifier")

        # '{'
        self.eat("token", "{")

        # classVarDec*
        self.out_file.write(f"\n{self.indent}<classVarDec>")
        self.inc_indent()
        while self.compileClassVarDec():
            pass
        self.dec_indent()
        self.out_file.write(f"\n{self.indent}</classVarDec>")

        # subroutineDec*
        self.out_file.write(f"\n{self.indent}<subroutineDec>")
        self.inc_indent()
        while self.compileSubroutine():
            pass
        self.dec_indent()
        self.out_file.write(f"\n{self.indent}</subroutineDec>")

        # '}'
        self.eat("token", "}")

        self.dec_indent()
        self.out_file.write(f"\n{self.indent}</class>")


    def check_for_one_or_more_identifiers(self):
        if self.tokenizer.get_current_token().get_token() == ",":
            self.emit()
        else:
            return False

        if self.tokenizer.get_current_token().get_type() == "identifier":
            self.emit()
            return True
        else:
            self.syntax_error(self.tokenizer.get_current_token().get_token())


    def compileClassVarDec(self):

        # ('static' | 'field') type varName (',' varName)* ';'

        # ('static' | 'field')
        if self.tokenizer.get_current_token().get_type() == "keyword":
            if (self.tokenizer.get_current_token().get_token() == "static" or
               self.tokenizer.get_current_token().get_token() == "field"):

                self.emit()

                # type { 'int' | 'char' | 'boolean' | varName }
                if (self.tokenizer.get_current_token().get_token() in Token.var_types
                    or self.tokenizer.get_current_token().get_type() == "identifier"):

                    self.emit()

                # varName
                self.eat("type", "identifier")

                #(',' varName)*
                while self.check_for_one_or_more_identifiers():
                    pass

                # ';'
                self.eat("token", ";")

            else:
                return False


    def compileSubroutine(self):
        # ('constructor' | 'function' | 'method') ('void' | type) subroutineName '(' parameterList ')' '{' varDec* statements '}'

        # ('constructor' | 'function' | 'method')
        if self.tokenizer.get_current_token().get_token() in Token.function_types:

            self.emit()

            # ('void' | type)
            if (self.tokenizer.get_current_token().get_token() in Token.var_types
                or self.tokenizer.get_current_token().get_token() == "void"):

                self.emit()

                # subroutineName
                self.eat("type", "identifier")

                # '('
                self.eat("token", "(")

                # parameterList
                self.out_file.write(f"\n{self.indent}<parameterList>")
                self.inc_indent()
                while self.compileParameterList():
                    pass
                self.dec_indent()
                self.out_file.write(f"\n{self.indent}</parameterList>")

                # ')'
                self.eat("token", ")")

                # subroutineBody = '{' varDec* statements '}'
                self.out_file.write(f"\n{self.indent}<subroutineBody>")
                self.inc_indent()
                # '{'
                self.eat("token", "{")

                # varDec*
                self.out_file.write(f"\n{self.indent}<varDec>") #TODO clean this up
                self.inc_indent()
                while self.compileVarDec():
                    self.dec_indent()
                    self.out_file.write(f"\n{self.indent}</varDec>")
                    self.out_file.write(f"\n{self.indent}<varDec>")
                    self.inc_indent()
                    pass
                self.dec_indent()
                self.out_file.write(f"\n{self.indent}</varDec>")

                # statements
                self.out_file.write(f"\n{self.indent}<statements>")
                self.inc_indent()
                while self.compileStatements():
                    pass
                self.dec_indent()
                self.out_file.write(f"\n{self.indent}</statements>")

                # '}'
                self.eat("token", "}")

                self.dec_indent()
                self.out_file.write(f"\n{self.indent}</subroutineBody>")

            else:
                self.syntax_error(self.tokenizer.get_current_token().get_token())

        else:
            return False


    def compileParameterList(self):
        # ((type varName) (',' varName)*)?

        # type
        if (self.tokenizer.get_current_token().get_token() in Token.var_types
            or self.tokenizer.get_current_token().get_type() == "identifier"):
            self.emit()

            # varname
            self.eat("type", "identifier")

            # (',' varName)*
            while self.check_for_one_or_more_identifiers():
                pass

        else:
            return False


    def compileVarDec(self):
        # 'var' type varName (',' varName)* ';'

        # 'var'
        self.eat("token", "var")

        # TODO this is the same as in parameter list, consider drying this up
        # type
        if (self.tokenizer.get_current_token().get_token() in Token.var_types
            or self.tokenizer.get_current_token().get_type() == "identifier"):
            self.emit()

            # varname
            self.eat("type", "identifier")

            # (',' varName)*
            while self.check_for_one_or_more_identifiers():
                pass

            # ';'
            self.eat("token", ";")

            return True
        else:
            return False


    def compileStatements(self):
        # (letStatement | ifStatement | whileStatement | doStatement | returnStatement)*
        current_token = self.tokenizer.get_current_token().get_token()
        if current_token in Token.statement_types:
            if current_token == "let":
                self.compileLet()

            if current_token == "if":
                self.compileIf()

            if current_token == "while":
                self.compileWhile()

            if current_token == "do":
                self.compileDo()

            if current_token == "return":
                self.compileReturn()
        else:
            return False


    def compileDo(self):
        # 'do' subroutineCall = subroutineName '(' expressionList ')' | (className | varName) '.'
        # subroutineName '(' expressionList ')' ';'

        # 'do'
        self.eat("token", "do")

        # subroutineCall
        self.compileSubroutineCall()


    def compileSubroutineCall(self):
        # subroutineName '(' expressionList ')' |
        # (className | varName) '.' subroutineName '(' expressionList ')' ';'

        # subroutineName | className | varName
        self.eat("type", "identifier")

        # '.' subroutineName '(' expressionList ')'
        if (self.tokenizer.get_current_token().get_token() == "."):
            # '.'
            self.emit()

            # sunroutineName
            self.eat("type", "identifier")

        #     # (
        #     self.eat("token", "(")

        #     # expressionList
        #     self.out_file.write(f"\n{self.indent}<expressionList>")
        #     self.inc_indent()
        #     while self.compileExpressionList():
        #         pass
        #     self.dec_indent()
        #     self.out_file.write(f"\n{self.indent}</expressionList>")

        #     # )
        #     self.eat("token", ")")

        # # '(' expressionList ')'
        # elif self.tokenizer.get_current_token().get_token() == "(":

        # (
        self.eat("token", "(")

        # expressionList
        self.out_file.write(f"\n{self.indent}<expressionList>")
        self.inc_indent()
        while self.compileExpressionList():
            pass
        self.dec_indent()
        self.out_file.write(f"\n{self.indent}</expressionList>")

        # )
        self.eat("token", ")")

        self.eat("token", ";")


    def compileIndexedExpression(self):
        # '[' expression ']'
        if self.tokenizer.get_current_token().get_token() == "[":
            self.emit()

            self.compileExpression()

            self.eat("token", "]")
        else:
            return False


    def compileLet(self):
        # 'let' varName ('[' expression ']')? '=' expression ';'

        # 'let'
        self.eat("token", "let")

        # varName
        self.eat("type", "identifier")

        # '[' expression ']'
        while self.compileIndexedExpression():
            pass

        # '='
        self.eat("token", "=")

        # expression
        self.compileExpression()

        # ';'
        self.eat("token", "=")


    def compileWhile(self):
        # 'while' '(' expression ')' '{' statements '}'
        self.out_file.write(f"\n{self.indent}<whileStatement>")


    def compileReturn(self):
        # 'return' expression
        self.eat("token", "return")

        self.compileExpression()



    def compileElseStatement(self):
        # 'else' '{' statements '}'
        if self.tokenizer.get_current_token().get_token() == "else":
            # 'else'
            self.emit()

            # '{'
            self.eat("token", "{")

            # statements
            while self.compileStatements():
                pass

            # '}'
            self.eat("token", "}")
        else:
            return False


    def compileIf(self):

        self.out_file.write(f"\n{self.indent}<ifStatement>")
        self.inc_indent()

        # 'if' '(' expression ')' '{' statements '}' ('else' '{' statements '}')?

        # 'if'
        self.eat("token", "if")

        # '('
        self.eat("token", "(")

        self.compileExpression()

        # ')'
        self.eat("token", ")")

        # '{'
        self.eat("token", "{")

        # statements
        while self.compileStatements():
            pass

        # '}'
        self.eat("token", "}")

        # 'else' '{' statements '}'
        while self.compileElseStatement():
            pass


    def compileOpTerm(self):
        # op term
        if self.tokenizer.get_current_token().get_token() in Token.operators:
            # op
            self.eat("type", "symbol")

            # term
            while self.compileTerm():
                pass
        else:
            return False


    def compileExpression(self):
        # term (op term)*
        self.out_file.write(f"\n{self.indent}<expression>")

        # term
        self.compileTerm()

        # (op term)*
        while self.compileOpTerm():
            pass

        self.out_file.write(f"\n{self.indent}</expression>")


    def compileTerm(self):
        # integerConstant | stringConstant | keywordConstant | varName | varName '[' expression ']' | subroutineCall |'(' expression ')' | unaryOp term
        # if token is an identifer, need to look at the next token to distinguish between varName, varName[], and subroutineCall
        self.inc_indent()
        self.out_file.write(f"\n{self.indent}<term>")

        # integerConstant | stringConstant | keywordConstant | varName
        current_token = self.tokenizer.get_current_token().get_token()
        if (self.tokenizer.get_current_token().token_type(current_token) == "integetConstant" or
           self.tokenizer.get_current_token().token_type(current_token) == "stringConstant" or
           self.tokenizer.get_current_token().token_type(current_token) == "keyword"):

            self.eat("token", current_token)

        elif self.tokenizer.get_current_token().token_type(current_token) == "identifier":
            # TODO this is where we need to look two ahead
            if self.compileIndexedExpression():
                pass
        elif self.compileSubroutineCall():
            pass

        self.out_file.write(f"\n{self.indent}</term>")
        self.dec_indent()

    def compileExpressionList(self):
        # (expression (',' expression)*)?
        return ""

    def syntax_error(self, place):
        print("syntax error: " + str(place))