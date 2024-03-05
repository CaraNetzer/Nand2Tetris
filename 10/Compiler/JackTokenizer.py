import re

class JackTokenizer:
    current_token = ""

    keywords = [
        "class", "constructor", "function", "method", "field", "static", "var", "int",
        "char", "boolean", "void", "true", "false", "null", "this", "let", "do", "if", "else",
        "while", "return"
    ]

    symbols = [
        '{', '}', '(', ')', '[', ']', '.', ',', ';', '+', '-', '*', '/', '&', '|', '<', '>', '=', '_'
    ]

    def __init__(self, in_file_path, out_file_path):
        self.in_file = open(in_file_path, "rt")
        self.out_file = open(out_file_path, "at", buffering = 1024)


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

    def symbol(self):
        if self.current_token == '<':
            return "&lt;"
        elif self.current_token == '>':
            return "&gt;"
        elif self.current_token == '&':
            return "&amp;"
        else:
            return self.current_token
