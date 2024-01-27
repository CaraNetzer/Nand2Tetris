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
        self.file = open(file_path, "wt", buffering = 1024)
        self.label_counter = 0

    #region arithmetic
    def write_arithmetic(self, command):

        self.file.write("// " + command + "\n")

        if command == "add" or command == "sub" or command == "and" or command == "or":
            self.save_args_and_compute(self.math.get(command))
            self.push_back_onto_stack()
        elif command == "neg" or command == "not":
            # SP--
            self.file.write("@SP\n")
            self.file.write("M=M-1\n")

            # D=(-/!)RAM[SP]
            self.file.write("A=M\n")
            self.file.write("D=" + self.math.get(command) + "M\n")

            self.push_back_onto_stack()
        elif command == "eq" or command == "lt" or command == "gt":
            self.save_args_and_compute_bool(command)
            self.push_back_onto_stack()


    def save_args_and_compute(self, computation):
        print("save")
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

    def save_args_and_compute_bool(self, bool):
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
        self.file.write("@TRUE." + self.label_counter + "\n")
        self.file.write("D;J" + bool.toUpperCase() + "\n")

        # else goto FALSE
        self.file.write("@FALSE." + self.label_counter + "\n")
        self.file.write("0;JMP\n")

        self.file.write("(TRUE." + self.label_counter + ")\n")
        self.file.write("D=-1\n")
        self.file.write("@END." + self.label_counter + "\n")
        self.file.write("0 JMP\n")

        self.file.write("(FALSE." + self.label_counter + ")\n")
        self.file.write("D=0\n")

        self.file.write("(END." + self.label_counter + ")\n")

        self.label_counter += 1

    #endregion

    def push_back_onto_stack(self):
        # RAM[SP] = D
        self.file.write("@SP\n")
        self.file.write("A=M\n")
        self.file.write("M=D\n")

        # SP++
        self.file.write("@SP\n")
        self.file.write("M=M+1\n")

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

                # System.out.println("in this C_POP? -- " + segment)

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
        self.file.write("D=M\n")

        self.file.write("@R13\n")
        self.file.write("A=M\n")
        self.file.write("M=D\n")

    def write_init(self):
        self.file.write("// init\n")

        # initialize the stack pointer to RAM[256]
        self.file.write("@256\n")
        self.file.write("D=A\n")
        self.file.write("@SP\n")
        self.file.write("M=D\n")

        # call Sys.init 0
        # push return-address, lcl, arg, this, that
        self.file.write("@SYS_INIT_RETURN_ADDRESS\n")
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
        self.file.write("@SP\n")
        self.file.write("D=M\n") # D=sp
        self.file.write("@5\n")
        self.file.write("D=D-A\n")
        self.file.write("@ARG\n")
        self.file.write("M=D\n") # ARG = sp - 5

        # LCL = SP
        self.file.write("@SP\n")
        self.file.write("D=M\n")
        self.file.write("@LCL\n")
        self.file.write("M=D\n")

        # goto Sys.init
        self.file.write("@Sys.Init\n")
        self.file.write("0;JMP\n")

        # (return_address)
        self.file.write("(SYS_INIT_RETURN_ADDRESS)\n")

    def write_label(self, label):
        self.file.write("// label " + self.file_name + "." + label + "\n")
        self.file.write("(" + self.file_name + "." + label + ")\n")

    def write_goto(self, label):
        self.file.write("// goto " + self.file_name + "." + label + "\n")
        self.file.write("@" + self.file_name + "." + label + "\n")
        self.file.write("0;JMP\n")

    def write_if(self, label):
        self.file.write("// if-goto " + self.file_name + "." + label + "\n")

        # SP--
        self.file.write("@SP\n")
        self.file.write("M=M-1\n")

        # D = RAM[SP]
        self.file.write("A=M\n")
        self.file.write("D=M\n")

        # jump to label if D = 0
        self.file.write("@" + self.file_name + "." + label + "\n")
        self.file.write("D;JNE\n")

    def write_call(self, function_name, num_args):
        self.file.write("// function " + function_name + " " + num_args + "\n")
        print("writeCall")

    def write_return(self):
        self.file.write("// return\n")
        print("writeReturn")

    def write_function(self, function_name, num_locals):
        self.file.write("// function " + function_name + " " + num_locals + "\n")
        print("writeFunction")