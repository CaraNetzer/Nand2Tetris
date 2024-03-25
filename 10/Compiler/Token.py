import re

var_types = { "int", "char", "boolean" }
function_types = { "function", "constructor", "method" }
operators = { "+", "-", "*", "/", "&", "|", "<", ">", "=", "~" }
statement_types = { "let", "do", "if", "else", "while", "return" }
unary_operators = { "-", "~" }
xml_operators = {
   "<": "&lt;",
   ">": "&gt;",
   "&": "&amp;"
}

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

  def set_token(self, in_token):
     # print("SET TOKEN---------------------------------------------------")
     self.token = in_token

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

  def to_xml(self):
    return xml_operators[self.token] if self.token in xml_operators else self.token

  def __str__(self):
     return f"<{self.type}> {self.to_xml()} </{self.type}>"
