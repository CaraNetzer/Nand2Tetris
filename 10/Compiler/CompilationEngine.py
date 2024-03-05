class CompilationEngine:

    def __init__(self, in_tokenizer, out_file):
        self.tokens = in_tokenizer.tagged_tokens
        self.tokenizer = in_tokenizer
        self.out_file = out_file

    def compileClass(self):
        self.tokenizer.current_token = self.tokens[0]
        self.out_file.write("<class>")
        self.tokenizer.advance()
        self.out_file.write(f"\n\t{self.tokenizer.current_token}")
        self.tokenizer.advance()
        self.out_file.write(f"\n\t{self.tokenizer.current_token}")
        self.tokenizer.advance()
        self.out_file.write(f"\n\t{self.tokenizer.current_token}")

        self.compileClassVarDec()
        self.compileSubroutine()

        self.tokenizer.advance()
        self.out_file.write(f"\n\t{self.tokenizer.current_token}")

    
    def compileClassVarDec(self):
        self.tokenizer.advance()

        if self.tokenizer.token_type() == "KEYWORD":
            if self.tokenizer.keyWord() == "STATIC" or self.tokenizer.keyWord() == "FIELD":
                self.tokenizer.advance()
                self.out_file.write(f"\n\t\t{self.tokenizer.current_token}")
                self.tokenizer.advance()
                self.out_file.write(f"\n\t\t{self.tokenizer.current_token}")
                self.tokenizer.advance()
                self.out_file.write(f"\n\t\t{self.tokenizer.current_token}")
                self.tokenizer.advance()
                if self.tokenizer.token_type() == "SYMBOL":
                    if self.tokenizer.symbol() == ",":
                        self.out_file.write(f"\n\t\t{self.tokenizer.current_token}")
                        self.tokenizer.advance()
                        self.out_file.write(f"\n\t\t{self.tokenizer.current_token}")



    def compileSubroutine(self):
        self.tokenizer.advance()
        return ""
    
    def compileWhile(self):
        self.out_file.write("<whileStatement>")
        if self.tokenizer.keyWord() == "WHILE":
            self.out_file.write(f"\n\t\t{self.tokenizer.current_token}")
            self.tokenizer.advance()
            if self.tokenizer.symbol() == "(":
                self.out_file.write(f"\n\t\t{self.tokenizer.current_token}")
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