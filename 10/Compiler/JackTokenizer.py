import re
import LinkedList
import Token

class JackTokenizer:
    current_token = ""
    tagged_tokens = []
    current_index = 0

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
        # self.out_file = open(out_file_path, "at", buffering = 1024)


    def advance(self):
        self.current_token = self.tagged_tokens[self.current_index]
        self.current_index += 1


    def get_current_token_str(self):
        return str(self.current_token)

    def get_current_token(self):
        return self.current_token

    # def token_type(self):
    #     if str(self.current_token) in self.keywords:
    #         return "KEYWORD"
    #     elif str(self.current_token) in self.symbols:
    #         return "SYMBOL"
    #     elif re.match("\\d", str(self.current_token)):
    #         return "INT_CONST"
    #     elif self.current_token.get_token().__contains__(" "):
    #         return "STRING_CONST"
    #     else:
    #         return "IDENTIFIER"

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
        return self.keywords.__getitem__(self.keywords.index(keyword)).upper()


    def symbol(self):
        symbol = self.current_token.split(" ")[1]
        print(self.symbols.__getitem__(self.symbols.index(symbol)).upper())
        return self.symbols.__getitem__(self.symbols.index(symbol)).upper()


    def append_tokens(self, words, token_list):
        for word in words:
            tokens_and_words = re.split("(\\W)", word)
            for token in tokens_and_words:
                if token != '':
                    token_list.append(token)


    def execute(self):
        tokens = []
        block_comment = False

        for line in self.in_file:

            # skip block comments
            if block_comment:
                if line.__contains__("*/"):
                    block_comment = False
                continue
            if line.strip().startswith("/*"):
                block_comment = True
                if line.__contains__("*/"):
                    block_comment = False
                continue

            # all other lines
            if not line.startswith("//"):
                line = re.split("(/\\*)|(//)", line)[0].strip()
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

            newToken = Token.Token(token)
            self.tagged_tokens.append(newToken)

        # t = self.tagged_tokens.head
        # g = self.tagged_tokens.head.get_next().get_next().get_next()
        # print(t.get_data().get_type() + ": " + t.get_data().get_token() + "\n")
        # print(g.get_data().get_type() + ": " + g.get_data().get_token() + "\n")

# TO-DO ideas:
# token class
# symbol stack