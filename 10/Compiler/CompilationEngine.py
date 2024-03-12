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

    def stack_push(self, token):
        self.stack.insert(token)
        self.level = self.level + 1
        self.indent = self.tab * self.level
        print(str(self.stack))

    def stack_pop(self):
        self.stack.pop()
        self.level = self.level - 1
        self.indent = self.tab * self.level
        print(str(self.stack))

    def compileClass(self):
        self.out_file.write("<class>")
        self.level = self.level + 1

        self.indent = self.tab * self.level
        if self.tokenizer.tagged_tokens[0].get_token() == "class":
            self.out_file.write(f"\n{self.indent}{self.tokenizer.get_current_token_str()}")
            self.tokenizer.advance()

        if self.tokenizer.get_current_token().get_type() == "identifier":
            self.out_file.write(f"\n{self.indent}{self.tokenizer.get_current_token_str()}")
            self.tokenizer.advance()

        if self.tokenizer.get_current_token().get_token() == "{":
            self.out_file.write(f"\n{self.indent}{self.tokenizer.get_current_token_str()}")
            self.stack_push(self.tokenizer.get_current_token())
            self.tokenizer.advance()

        # while not self.stack.is_empty():
            self.compileClassVarDec()
        # while not self.stack.is_empty():
            self.compileSubroutine()
        print(str(self.stack))

        if self.tokenizer.get_current_token().get_token() == "}":
            self.out_file.write(f"\n{self.indent}{self.tokenizer.get_current_token_str()}")
            if self.stack.match(self.tokenizer.get_current_token()):
                self.stack_pop()
            else:
                self.stack_push(self.tokenizer.get_current_token().get_token())
                self.out_file.write(f"\n{self.indent}{self.tokenizer.get_current_token_str()}")
                self.tokenizer.advance()

            # if token = is the pair of whats already on the stack
                # stack.pop
                # level--
                # self.indent = self.tab * self.level
            # else
                # self.stack.insert(self.tokenizer.get_current_token().get_token())
                # level++
                # self.indent = self.tab * self.level
            self.tokenizer.advance()


    def compileClassVarDec(self):
        # ('static' | 'field') type varName (',' varName)* '}'

        # ('static' | 'field')
        if self.tokenizer.get_current_token().get_type() == "keyword":
            if (self.tokenizer.get_current_token().get_token() == "static" or
               self.tokenizer.get_current_token().get_token() == "field"):

                self.out_file.write(f"\n{self.indent}{self.tokenizer.get_current_token_str()}")
                self.tokenizer.advance()

                # type
                if (self.tokenizer.get_current_token().get_token() == "int" or
                    self.tokenizer.get_current_token().get_token() == "char" or
                    self.tokenizer. get_current_token().get_token() == "boolean"
                    or self.tokenizer.get_current_token().get_type() == "identifier"):

                    self.out_file.write(f"\n{self.indent}{self.tokenizer.get_current_token_str()}")
                    self.tokenizer.advance()

                # varName
                if self.tokenizer.get_current_token().get_type() == "identifier":
                    self.out_file.write(f"\n{self.indent}{self.tokenizer.get_current_token_str()}")
                    self.tokenizer.advance()

                #(',' varName)*
                while self.tokenizer.get_current_token().get_token() != ";":
                    if self.tokenizer.get_current_token().get_type() == "symbol":
                        if self.tokenizer. get_current_token().get_token() == ",":
                            if self.stack.match(self.tokenizer.get_current_token()):
                                self.stack_pop()
                            else:
                                self.stack_push(self.tokenizer.get_current_token().get_token())
                                self.out_file.write(f"\n{self.indent}{self.tokenizer.get_current_token_str()}")
                                self.tokenizer.advance()

                                if self.tokenizer.get_current_token().token_type() == "identifier":
                                    self.out_file.write(f"\n{self.indent}{self.tokenizer.get_current_token_str()}")
                                    self.tokenizer.advance()

                # at this point token must be ;
                self.out_file.write(f"\n{self.indent}{self.tokenizer.get_current_token_str()}")
                self.tokenizer.advance()


    def compileSubroutine(self):
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