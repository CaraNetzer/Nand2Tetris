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
        if item == "token":
            if self.tokenizer.get_current_token().get_token() == match:
                self.emit()
            else:
                self.syntax_error(1)
        elif item == "type":
            if self.tokenizer.get_current_token().get_type() == match:
                self.emit()
            else:
                self.syntax_error(2)


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
        self.inc_indent()

        # classVarDec*
        while self.compileClassVarDec():
            pass

        # subroutineDec*
        while self.compileSubroutine():
            pass

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
            self.syntax_error(3)


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
            self.out_file.write(f"\n{self.indent}<subroutineDec>")
            self.inc_indent()

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
                while self.compileParameterList():
                    pass

                # ')'
                self.eat("token", ")")

                # subroutineBody = '{' varDec* statements '}'
                # '{'
                self.eat("token", "{")

                # varDec*
                while self.compileVarDec():
                    print('vardec loop')
                    pass

                # statements
                while self.compileStatements():
                    pass

                # '}'
                self.eat("token", "}")

            else:
                self.syntax_error(6)
        
            self.dec_indent()
            self.out_file.write(f"\n{self.indent}</subroutineDec>")
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
        while self.compileLet():
            pass

        while self.compileIf():
            pass

        while self.compileWhile():
            pass

        while self.compileDo():
            pass

        while self.compileReturn():
            pass

        return False

    
    def compileDo(self):
        # 'do' subroutineCall ';'
        return ""
    
    def compileLet(self):
        # 'let' varName ('[' expression ']')? '=' expression ';'
        return ""

    def compileWhile(self):
        # 'while' '(' expression ')' '{' statements '}'
        self.out_file.write("<whileStatement>")


    def compileReturn(self):
        # 'return' expression
        return ""
    
    def compileIf(self):
        # 'if' '(' expression ')' '{' statements '}' ('else' '{' statements '}')?
        return ""

    def compileExpression(self):
        # term (op term)*
        self.out_file.write("<expression>")
        self.compileTerm()
        self.out_file.write("</expression>")

    def compileTerm(self):
        # integerConstant | stringConstant | keywordConstant | varName | varName '[' expression ']' | subroutineCall |'(' expression ')' | unaryOp term
        # if token is an identifer, need to look at the next token to distinguish between varName, varName[], and subroutineCall
        self.out_file.write("<term>")
        self.tokenizer.advance()

        self.out_file.write("</term>")

    def compileExpressionList(self):
        # (expression (',' expression)*)?
        return ""

    def syntax_error(self, place):
        print("syntax error: " + str(place))