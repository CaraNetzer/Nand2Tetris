class Parser:

    currentLine = ""

    def __init__(self, filePath):
        self.file = open(filePath, "rt")


    def commandType(self):
      try:
         command = self.currentLine.split(" ")[0]
      except Exception as e:
        return e

      commands = {
        "add"     : "C_ARITHMETIC",        "sub": "C_ARITHMETIC",        "neg": "C_ARITHMETIC",
        "eq"      : "C_ARITHMETIC",        "gt" : "C_ARITHMETIC",        "lt" : "C_ARITHMETIC",
        "and"     : "C_ARITHMETIC",        "or" : "C_ARITHMETIC",        "not": "C_ARITHMETIC",

        "push"    : "C_PUSH",
        "pop"     : "C_POP",
        "label"   : "C_LABEL",
        "goto"    : "C_GOTO",
        "if-goto" : "C_IF",
        "func"    : "C_FUNCTION",
        "call"    : "C_CALL",
      }

      return commands.get(command, "command not found")


    def arg1(self):
      try:
        return self.currentLine.split(" ")[1]
      except Exception as e:
         return "arg1: " + e


    def arg2(self):
      try:
        return self.currentLine.split(" ")[2]
      except Exception as e:
         return "arg2: " + e