class Parser:

    current_line = ""

    def __init__(self, file_path):
        self.file = open(file_path, "rt")


    def command_type(self):
      try:
         command = self.current_line.split(" ")[0]
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
        "function": "C_FUNCTION",
        "call"    : "C_CALL"
      }

      return commands.get(command, "command not found")


    def arg1(self):
      try:
        return self.current_line.split(" ")[1]
      except IndexError:
         return self.current_line.split(" ")[0]
      except Exception as e:
         return "arg1: " + str(e)


    def arg2(self):
      try:
        return self.current_line.split(" ")[2]
      except Exception as e:
         return "arg2: " + str(e)