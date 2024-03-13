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

                self.out_file.write(f"\n{self.indent}{self.tokenizer.get_current_token_str()}")
                self.tokenizer.advance()

                # type { 'int' | 'char' | 'boolean' | varName }
                if (self.tokenizer.get_current_token().get_token() in Token.var_types
                    or self.tokenizer.get_current_token().get_type() == "identifier"):

                    self.out_file.write(f"\n{self.indent}{self.tokenizer.get_current_token_str()}")
                    self.tokenizer.advance()

                # varName
                self.eat("type", "identifier")

                #(',' varName)*
                while self.check_for_one_or_more_identifiers():
                    pass

                # ';'
                if self.tokenizer.get_current_token().get_token() == ";":
                    self.emit()
                else:
                    self.syntax_error(4)

            else:
                return False


    def compileSubroutine(self):
        print("compileSubroutine\n")
        self.tokenizer.advance()
        return ""

    def compileWhile(self):
        self.out_file.write("<whileStatement>")
        if self.tokenizer.keyWord() == "WHILE":
            self.out_file.write(f"\n{self.indent}{self.tokenizer.get_current_token()}")
            self.tokenizer.advance()
            if self.tokenizer.symbol() == "(":
                self.out_file.write(f"\n{self.indent}{self.tokenizer.get_current_token()}")
                self.tokenizer.advance()
                self.compileExpression()


    def compileExpression(self):
        self.out_file.write("<expression>")
        self.compileTerm()
        self.out_file.write("</expression>")


    def compileTerm(self):
        self.out_file.write("<term>")
        self.tokenizer.advance()

        self.out_file.write("</term>")


    def syntax_error(self, place):
        print("syntax error: " + str(place))