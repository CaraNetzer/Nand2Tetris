#!/usr/bin/env python3
# note: written by david, not me

import sys

class Thingy:
  def __init__(self):
    # for testing this example:
    self.file = sys.stdout

  def write_return(self):
    def set_label_to_deref(dest, start, offset, direction="-"):
      self.file.write(f"// *{dest} = *({start} {direction} {offset})\n")
      self.file.write(f"@{start}\n")
      self.file.write(f"D=M\n")
      self.file.write(f"@{offset}\n")
      self.file.write(f"D=D{direction}A\n")
      self.file.write(f"@{dest}\n")
      self.file.write(f"M=D\n")

    # FRAME = LCL
    self.file.write("// FRAME = LCL\n")
    self.file.write("@LCL\n")
    self.file.write("D=M\n") # D = LCL = endFrame
    self.file.write("@endFrame\n") # endFrame = D
    self.file.write("M=D\n")

    # RET = RAM[FRAME - 5]
    set_label_to_deref("retAddr", "endFrame", 5)
    set_label_to_deref("THIS", "endFrame", 1)
    set_label_to_deref("THAT", "endFrame", 2)
    set_label_to_deref("LCL", "endFrame", 3)
    set_label_to_deref("ARG", "endFrame", 4)

thingy = Thingy()
thingy.write_return()