import re

class Token:

  keywords = [
      "class", "constructor", "function", "method", "field", "static", "var", "int",
      "char", "boolean", "void", "true", "false", "null", "this", "let", "do", "if", "else",
      "while", "return"
  ]

  symbols = [
      '{', '}', '(', ')', '[', ']', '.', ',', ';', '+', '-', '*', '/', '&', '|', '<', '>', '=', '~'
  ]

  def __init__(self, token):
    self.type = self.token_type(token)
    self.token = token

  def get_type(self):
    return self.type

  def get_token(self):
     return self.token

  def token_type(self, token):
    if token in self.keywords:
        return "keyword"
    elif token in self.symbols:
        return "symbol"
    elif re.match("\d", token):
        return "integerConstant"
    elif token.__contains__(" "):
        return "stringConstant"
    else:
        return "identifier"


  def __str__(self):
     return f"<{self.type}>{self.token}</{self.type}>"