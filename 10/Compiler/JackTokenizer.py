import re

class JackTokenizer:
    current_token = ""
    tagged_tokens = []

    keywords = [
        "class", "constructor", "function", "method", "field", "static", "var", "int",
        "char", "boolean", "void", "true", "false", "null", "this", "let", "do", "if", "else",
        "while", "return"
    ]

    symbols = [
        '{', '}', '(', ')', '[', ']', '.', ',', ';', '+', '-', '*', '/', '&', '|', '<', '>', '=', '~'
    ]

    def __init__(self, in_file_path, out_file_path):
        self.in_file = open(in_file_path, "rt")
        self.out_file = open(out_file_path, "at", buffering = 1024)

    
    def advance(self):
        current_index = self.tagged_tokens.index(self.current_token)
        self.current_token = self.tagged_tokens[current_index + 1]


    def token_type(self):
        if self.current_token in self.keywords:
            return "KEYWORD"
        elif self.current_token in self.symbols:
            return "SYMBOL"
        elif re.match("\d", self.current_token):
            return "INT_CONST"
        elif self.current_token.__contains__(" "):
            return "STRING_CONST"
        else:
            return "IDENTIFIER"

    def convert_symbol(self):
        if self.current_token == '<':
            return "&lt;"
        elif self.current_token == '>':
            return "&gt;"
        elif self.current_token == '&':
            return "&amp;"
        else:
            return self.current_token


    def keyWord(self):
        keyword = self.current_token.split(" ")[1]
        print(self.keywords.__getitem__(self.keywords.index(keyword)).upper())
        return self.keywords.__getitem__(self.keywords.index(keyword)).upper()
    
    
    def symbol(self):
        symbol = self.current_token.split(" ")[1]
        print(self.symbols.__getitem__(self.symbols.index(symbol)).upper())
        return self.symbols.__getitem__(self.symbols.index(symbol)).upper()
        
    
    def append_tokens(words, token_list):
        for word in words:
            tokens_and_words = re.split("(\W)", word)
            for token in tokens_and_words:
                if token != '':
                    token_list.append(token)


    def execute(self):
        tokens = []
        block_comment = False

        for line in self.in_file:
            if block_comment:
                if line.__contains__("*/"):
                    block_comment = False
                continue
            if line.strip().startswith("/*"):
                block_comment = True
                if line.__contains__("*/"):
                    block_comment = False
                continue

            if not line.startswith("//"):
                line = re.split("(/\*)|(//)", line)[0].strip()
                if line.__contains__("\""):
                    words = line.split("\"")
                    self.append_tokens(words[0].split(" "), tokens)
                    tokens.append(words[1])
                    self.append_tokens(words[2].split(" "), tokens)
                else:
                    words = line.split(" ")
                    self.append_tokens(words, tokens)


        for token in tokens:
            self.current_token = token

            if self.token_type() == "KEYWORD":
                self.tagged_tokens.append(f"<keyword> {self.current_token} </keyword>")
            if self.token_type() == "SYMBOL":
                self.tagged_tokens.append(f"<symbol> {self.convert_symbol()} </symbol>")
            if self.token_type() == "IDENTIFIER":
                self.tagged_tokens.append(f"<identifier> {self.current_token} </identifier>")
            if self.token_type() == "INT_CONST":
                self.tagged_tokens.append(f"<integerConstant> {self.current_token} </integerConstant>")
            if self.token_type() == "STRING_CONST":
                self.tagged_tokens.append(f"<stringConstant> {self.current_token} </stringConstant>")

            # self.out_file.write(f"<stringConstant> {self.current_token} </stringConstant>\n")
