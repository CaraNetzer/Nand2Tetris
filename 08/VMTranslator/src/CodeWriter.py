class CodeWriter:

    segment_pointers = {
        "local": "LCL",
        "argument": "ARG",
        "this": "THIS",
        "that": "THAT"
    }

    math = {
        "add": "+",
        "sub": "-",
        "and": "&",
        "or": "|",
        "neg": "-",
        "not": "!"
    }

    def __init__(self, file_path, file_name):
        self.file_name = file_name[0:-3]
        self.file = open(file_path, "at", buffering = 1024)
        self.bool_label_counter = 0
        self.return_label_counter = 0
        self.function_name = ""

    #region arithmetic
    def write_arithmetic(self, command):

        def save_args_and_compute(computation):
            # SP--
            self.file.write("@SP\n")
            self.file.write("M=M-1\n")

            # D=RAM[SP]
            self.file.write("A=M\n")
            self.file.write("D=M\n")

            # put first arg in R13
            self.file.write("@R13\n")
            self.file.write("M=D\n")

            # repeat that
            # SP--
            self.file.write("@SP\n")
            self.file.write("M=M-1\n")

            # D=RAM[SP]
            self.file.write("A=M\n")
            self.file.write("D=M\n")
            # put second arg in R14
            self.file.write("@R14\n")
            self.file.write("M=D\n")

            #  +, -, &, or |
            self.file.write("@R13\n")
            self.file.write("D=D" + computation + "M\n")

        def save_args_and_compute_bool(bool):
            # SP--
            self.file.write("@SP\n")
            self.file.write("M=M-1\n")

            # D=RAM[SP]
            self.file.write("A=M\n")
            self.file.write("D=M\n")

            # put first arg in R13
            self.file.write("@R13\n")
            self.file.write("M=D\n")

            # repeat that
            # SP--
            self.file.write("@SP\n")
            self.file.write("M=M-1\n")
            # D=RAM[SP]
            self.file.write("A=M\n")
            self.file.write("D=M\n")
            # put second arg in R14
            self.file.write("@R14\n")
            self.file.write("M=D\n")

            # D = first number
            self.file.write("@R14\n")
            self.file.write("D=M\n")

            # D = x - y
            self.file.write("@R13\n")
            self.file.write("D=D-M \n")

            # if D (eq/gt/lt) 0 (bool) goto TRUE
            self.file.write("@" + self.function_name + "$TRUE." + str(self.bool_label_counter) + "\n")
            self.file.write("D;J" + bool.upper() + "\n")

            # else goto FALSE
            self.file.write("@" + self.function_name + "$FALSE." + str(self.bool_label_counter) + "\n")
            self.file.write("0;JMP\n")

            self.file.write("(" + self.function_name + "$TRUE." + str(self.bool_label_counter) + ")\n")
            self.file.write("D=-1\n")
            self.file.write("@" + self.function_name + "$END." + str(self.bool_label_counter) + "\n")
            self.file.write("0;JMP\n")

            self.file.write("(" + self.function_name + "$FALSE." + str(self.bool_label_counter) + ")\n")
            self.file.write("D=0\n")

            self.file.write("(" + self.function_name + "$END." + str(self.bool_label_counter) + ")\n")

            self.bool_label_counter += 1

        def push_back_onto_stack():
            # RAM[SP] = D
            self.file.write("@SP\n")
            self.file.write("A=M\n")
            self.file.write("M=D\n")

            # SP++
            self.file.write("@SP\n")
            self.file.write("M=M+1\n")

        self.file.write("// " + command + "\n")

        if command == "add" or command == "sub" or command == "and" or command == "or":
            save_args_and_compute(self.math.get(command))
            push_back_onto_stack()
        elif command == "neg" or command == "not":
            # SP--
            self.file.write("@SP\n")
            self.file.write("M=M-1\n")

            # D=(-/!)RAM[SP]
            self.file.write("A=M\n")
            self.file.write("D=" + self.math.get(command) + "M\n")

            self.push_back_onto_stack()
        elif command == "eq" or command == "lt" or command == "gt":
            save_args_and_compute_bool(command)
            push_back_onto_stack()

    #endregion

    def write_push_pop(self, command, segment, index):

        if (segment == "local" or segment == "argument" or segment == "this" or segment == "that"):
            if command == "C_PUSH":

                self.file.write("// push " + segment + " " + index + "\n")

                # addr = segmentPointers.get(segment) + index
                self.file.write("@" + self.segment_pointers.get(segment) + "\n")
                self.file.write("D=M\n")
                self.file.write("@" + index + "\n")
                self.file.write("D=D+A\n")

                self.file.write("A=D\n")  # M of this will be addr
                self.file.write("D=M\n")  # RAM[addr]

                # RAM[SP] = RAM[addr]
                self.push()

            elif command == "C_POP":

                self.file.write("// pop " + segment + " " + index + "\n")

                # addr = segmentPointers.get(segment) + index
                self.file.write("@" + self.segment_pointers.get(segment) + "\n")
                self.file.write("D=M\n")
                self.file.write("@" + index + "\n")
                self.file.write("D=D+A\n")
                self.file.write("@R13\n")
                self.file.write("M=D\n")  # store address in R13

                self.pop()

        elif segment == "constant":
            self.file.write("// push constant " + index + "\n")

            # D=index
            self.file.write("@" + index + "\n")
            self.file.write("D=A\n")

            # RAM[SP]=D
            self.push()


        elif segment == "static":
            if command == "C_PUSH":
                self.file.write("// push " + segment + " " + index + "\n")

                # RAM[SP] = RAM[addr]
                self.file.write("@" + self.file_name + "." + index + "\n")
                self.file.write("D=M\n")  # RAM[addr]

                self.push()

            elif command == "C_POP":
                self.file.write("// pop " + segment + " " + index + "\n")

                # SP--
                self.file.write("@SP\n")
                self.file.write("M=M-1\n")

                # RAM[addr] = RAM[SP]
                self.file.write("A=M\n")
                self.file.write("D=M\n")

                self.file.write("@" + self.file_name + "." + index + "\n")
                self.file.write("M=D\n")

        elif segment == "temp":
            if command == "C_PUSH":

                self.file.write("// push " + segment + " " + index + "\n")

                # addr = 5 + index
                self.file.write("@5\n")
                self.file.write("D=A\n")
                self.file.write("@" + index + "\n")
                self.file.write("D=D+A\n")

                self.file.write("A=D\n")
                self.file.write("D=M\n")  # RAM[addr]

                # RAM[SP] = RAM[addr]
                self.push()

            elif command == "C_POP":
                self.file.write("// pop " + segment + " " + index + "\n")

                # addr = 5 + index
                self.file.write("@5\n")
                self.file.write("D=A\n")
                self.file.write("@" + index + "\n")
                self.file.write("D=D+A\n")
                self.file.write("@R13\n")
                self.file.write("M=D\n")  # store address in R13

                self.pop()

        elif segment == "pointer":
            if command == "C_PUSH":

                self.file.write("// push " + segment + " " + index + "\n")

                # addr = 3 + index
                self.file.write("@3\n")
                self.file.write("D=A\n")
                self.file.write("@" + index + "\n")
                self.file.write("D=D+A\n")

                self.file.write("A=D\n")
                self.file.write("D=M\n")  # RAM[addr]

                # RAM[SP] = RAM[addr]
                self.push()

            elif command == "C_POP":

                self.file.write("// pop " + segment + " " + index + "\n")

                # addr = 3 + index
                self.file.write("@3\n")
                self.file.write("D=A\n")
                self.file.write("@" + index + "\n")
                self.file.write("D=D+A\n")
                self.file.write("@R13\n")
                self.file.write("M=D\n")  # store address in R13

                self.pop()


    def push(self):
        # RAM[SP] = RAM[addr]
        self.file.write("@SP\n")
        self.file.write("A=M\n")
        self.file.write("M=D\n")

        # SP++
        self.file.write("@SP\n")
        self.file.write("M=M+1\n")

    def pop(self):

        # SP--
        self.file.write("@SP\n")
        self.file.write("M=M-1\n")

        # RAM[addr] = RAM[SP]
        self.file.write("A=M\n")
        self.file.write("D=M\n") # D=RAM[SP]

        self.file.write("@R13\n")
        self.file.write("A=M\n")
        self.file.write("M=D\n")

    def write_init(self):
        self.file.write("// init\n")

        # initialize the stack pointer to RAM[256]
        self.file.write("// initialize the stack pointer to RAM[256]\n")
        self.file.write("@256\n")
        self.file.write("D=A\n")
        self.file.write("@SP\n")
        self.file.write("M=D\n")

        # call Sys.init 0
        # push return-address, lcl, arg, this, that
        self.file.write("// push return-address, lcl, arg, this, that\n")
        self.file.write("@BOOTSTRAP_CODE_RETURN_ADDRESS\n")
        self.file.write("D=A\n")
        self.push()
        self.file.write("@LCL\n")
        self.file.write("D=M\n")
        self.push()
        self.file.write("@ARG\n")
        self.file.write("D=M\n")
        self.push()
        self.file.write("@THIS\n")
        self.file.write("D=M\n")
        self.push()
        self.file.write("@THAT\n")
        self.file.write("D=M\n")
        self.push()

        # ARG = SP-(n=0)-5
        self.file.write("// ARG = SP-(n=0)-5\n")
        self.file.write("@SP\n")
        self.file.write("D=M\n") # D=sp
        self.file.write("@5\n")
        self.file.write("D=D-A\n")
        self.file.write("@ARG\n")
        self.file.write("M=D\n") # ARG = sp - 5

        # LCL = SP
        self.file.write("// LCL = SP\n")
        self.file.write("@SP\n")
        self.file.write("D=M\n")
        self.file.write("@LCL\n")
        self.file.write("M=D\n")

        # goto Sys.init
        self.file.write("// goto Sys.init\n")
        self.file.write("@Sys.init\n")
        self.file.write("0;JMP\n")

        # (return_address)
        self.file.write("(BOOTSTRAP_CODE_RETURN_ADDRESS)\n")

    #region program_flow
    def write_label(self, label):
        self.file.write("// label " + self.function_name + "$" + label + "\n")
        self.file.write("(" + self.function_name + "$" + label + ")\n")

    def write_goto(self, label):
        self.file.write("// goto " + self.function_name + "$" + label + "\n")
        self.file.write("@" + self.function_name + "$" + label + "\n")
        self.file.write("0;JMP\n")

    def write_if(self, label):
        self.file.write("// if-goto " + self.function_name + "$" + label + "\n")

        # SP--
        self.file.write("@SP\n")
        self.file.write("M=M-1\n")

        # D = RAM[SP]
        self.file.write("A=M\n")
        self.file.write("D=M\n")

        # jump to label if D = 0
        self.file.write("@" + self.function_name + "$" + label + "\n")
        self.file.write("D;JNE\n")

    #endregion

    def write_call(self, function_name, num_args):
        return_label = self.function_name + "$RETURN_ADDRESS." + str(self.return_label_counter)

        self.file.write("// call " + function_name + " " + num_args + "\n")

        # push return-address, lcl, arg, this, that
        self.file.write("// push return-address, lcl, arg, this, that\n")
        self.file.write("@" + return_label + "\n")
        self.file.write("D=A\n")
        self.push()
        self.file.write("@LCL\n")
        self.file.write("D=M\n")
        self.push()
        self.file.write("@ARG\n")
        self.file.write("D=M\n")
        self.push()
        self.file.write("@THIS\n")
        self.file.write("D=M\n")
        self.push()
        self.file.write("@THAT\n")
        self.file.write("D=M\n")
        self.push()

        # ARG = SP - num_args - 5
        self.file.write("// ARG = SP - num_args - 5\n")
        self.file.write("@SP\n")
        self.file.write("D=M\n") # D = sp
        self.file.write("@" + num_args + "\n")
        self.file.write("D=D-A\n") # D = sp - num_args
        self.file.write("@5\n")
        self.file.write("D=D-A\n") # D = sp - num_args - 5
        self.file.write("@ARG\n")
        self.file.write("M=D\n") # ARG = sp - num_args - 5

        # LCL = SP
        self.file.write("// LCL = SP\n")
        self.file.write("@SP\n")
        self.file.write("D=M\n")
        self.file.write("@LCL\n")
        self.file.write("M=D\n")

        # goto function_name
        self.file.write("// goto function_name\n")
        self.file.write("@" + function_name + "\n")
        self.file.write("0;JMP\n")

        # (return_address)
        self.file.write("(" + return_label + ")\n")

        self.return_label_counter += 1

    def write_return(self):

        def reset_pointer(decrement, pointer):
            self.file.write("// (segmentPointer) = RAM[FRAME - (decrement)]\n")
            self.file.write("@" + str(decrement) + "\n")
            self.file.write("D=A\n")
            self.file.write("@endFrame\n")
            self.file.write("A=M-D\n")

            self.file.write("D=M\n") # D = RAM[FRAME - decrement]
            self.file.write("@" + pointer + "\n")
            self.file.write("M=D\n")

        self.file.write("// return\n")

        # FRAME = LCL
        self.file.write("// FRAME = LCL\n")
        self.file.write("@LCL\n")
        self.file.write("D=M\n") # D = LCL = endFrame
        self.file.write("@endFrame\n") # endFrame = D
        self.file.write("M=D\n")

        # RET = RAM[FRAME - 5]
        self.file.write("// RET = RAM[FRAME - 5]\n")
        self.file.write("@5\n")
        self.file.write("A=D-A\n")
        self.file.write("D=M\n") # D = RAM[endFrame - 5]

        self.file.write("@retAddr\n")
        self.file.write("M=D\n") # return address from stack

        # RAM[ARG] = pop()
        self.file.write("// RAM[ARG] = pop()\n")
        self.file.write("@SP\n")
        self.file.write("M=M-1\n")

        self.file.write("A=M\n")
        self.file.write("D=M\n") # D=RAM[SP]

        self.file.write("@ARG\n")
        self.file.write("A=M\n")
        self.file.write("M=D\n")

        # SP = ARG + 1
        self.file.write("//SP = ARG + 1\n")
        self.file.write("@ARG\n")
        self.file.write("D=M\n")
        self.file.write("@1\n")
        self.file.write("D=D+A\n")
        self.file.write("@SP\n")
        self.file.write("M=D\n")

        # THAT = RAM[FRAME - 1]
        reset_pointer(1, "THAT")

        # THIS = RAM[FRAME - 2]
        reset_pointer(2, "THIS")

        # ARG = RAM[FRAME - 3]
        reset_pointer(3, "ARG")

        # LCL = RAM[FRAME - 4]
        reset_pointer(4, "LCL")

        # goto RET
        self.file.write("@retAddr\n")
        self.file.write("A=M\n")
        self.file.write("0;JMP\n")


    def write_function(self, function_name, num_locals):
        self.function_name = function_name

        self.file.write("// function " + function_name + " " + num_locals + "\n")
        self.file.write("(" + function_name + ")\n")

        # initialize num_locals and save it at temp[0]
        self.write_push_pop("C_PUSH", "constant", num_locals)
        self.write_push_pop("C_POP", "temp", "0")

        #region beginning of locals loop
        self.file.write("(" + function_name + "$LOCALS)\n")
        self.write_push_pop("C_PUSH", "temp", "0")

        # if the value at the top of the stack is not 0, initialize local[numLocalsLeft] to 0
        self.write_if("INIT_LOCAL")

        # else the difference is 0, we've set all locals to 0, jump to end
        self.file.write("@" + function_name + "$LOCALS_END\n")
        self.file.write("0;JMP\n")

        # initialize local[numLocalsLeft] to 0
        self.file.write("(" + function_name + "$INIT_LOCAL)\n")
        self.write_push_pop("C_PUSH", "constant", "0")

        # numLocalsLeft--
        self.write_push_pop("C_PUSH", "temp", "0")
        self.write_push_pop("C_PUSH", "constant", "1")
        self.write_arithmetic("sub")
        self.write_push_pop("C_POP", "temp", "0")
        # go to top of loop
        self.write_goto("LOCALS")

        # end of locals loop
        self.file.write("(" + function_name + "$LOCALS_END)\n")
        #endregion