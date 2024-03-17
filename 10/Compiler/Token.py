import re

var_types = { "int", "char", "boolean" }
function_types = { "function", "constructor", "method" }
operators = { "+", "-", "*", "/", "&", "|", "<", ">", "=", "~" }
statement_types = { "let", "do", "if", "else", "while", "return" }

class Token:


  keywords = {
      "class", "field", "static", "var", "void",
      "true", "false", "null", "this"
  }.union(var_types).union(function_types).union(statement_types)

  symbols = {
      '{', '}', '(', ')', '[', ']', '.', ',', ';'
  }.union(operators)

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
    elif re.match("\\d", token):
        return "integerConstant"
    elif token.__contains__(" "):
        return "stringConstant"
    elif not token[0].isdigit():
        return "identifier"


  def __str__(self):
     return f"<{self.type}> {self.token} </{self.type}>"