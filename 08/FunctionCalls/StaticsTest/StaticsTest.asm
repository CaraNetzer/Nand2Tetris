// init
// initialize the stack pointer to RAM[256]
@256
D=A
@SP
M=D
// push return-address, lcl, arg, this, that
@BOOTSTRAP_CODE_RETURN_ADDRESS
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1
// ARG = SP-(n=0)-5
@SP
D=M
@5
D=D-A
@ARG
M=D
// LCL = SP
@SP
D=M
@LCL
M=D
// goto Sys.init
@Sys.init
0;JMP
(BOOTSTRAP_CODE_RETURN_ADDRESS)
// function Sys.init 0
(Sys.init)
// push constant 0
@0
D=A
@SP
A=M
M=D
@SP
M=M+1
// pop temp 0
@5
D=A
@0
D=D+A
@R13
M=D
@SP
M=M-1
A=M
D=M
@R13
A=M
M=D
(Sys.init$LOCALS)
// push temp 0
@5
D=A
@0
D=D+A
A=D
D=M
@SP
A=M
M=D
@SP
M=M+1
// if-goto Sys.init$INIT_LOCAL
@SP
M=M-1
A=M
D=M
@Sys.init$INIT_LOCAL
D;JNE
@Sys.init$LOCALS_END
0;JMP
(Sys.init$INIT_LOCAL)
// push constant 0
@0
D=A
@SP
A=M
M=D
@SP
M=M+1
// push temp 0
@5
D=A
@0
D=D+A
A=D
D=M
@SP
A=M
M=D
@SP
M=M+1
// push constant 1
@1
D=A
@SP
A=M
M=D
@SP
M=M+1
// sub
@SP
M=M-1
A=M
D=M
@R13
M=D
@SP
M=M-1
A=M
D=M
@R14
M=D
@R13
D=D-M
@SP
A=M
M=D
@SP
M=M+1
// pop temp 0
@5
D=A
@0
D=D+A
@R13
M=D
@SP
M=M-1
A=M
D=M
@R13
A=M
M=D
// goto Sys.init$LOCALS
@Sys.init$LOCALS
0;JMP
(Sys.init$LOCALS_END)
// push constant 6
@6
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant 8
@8
D=A
@SP
A=M
M=D
@SP
M=M+1
// call Class1.set 2
// push return-address, lcl, arg, this, that
@Sys.init$RETURN_ADDRESS.0
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1
// ARG = SP - num_args - 5
@SP
D=M
@2
D=D-A
@5
D=D-A
@ARG
M=D
// LCL = SP
@SP
D=M
@LCL
M=D
// goto function_name
@Class1.set
0;JMP
(Sys.init$RETURN_ADDRESS.0)
// pop temp 0
@5
D=A
@0
D=D+A
@R13
M=D
@SP
M=M-1
A=M
D=M
@R13
A=M
M=D
// push constant 23
@23
D=A
@SP
A=M
M=D
@SP
M=M+1
// push constant 15
@15
D=A
@SP
A=M
M=D
@SP
M=M+1
// call Class2.set 2
// push return-address, lcl, arg, this, that
@Sys.init$RETURN_ADDRESS.1
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1
// ARG = SP - num_args - 5
@SP
D=M
@2
D=D-A
@5
D=D-A
@ARG
M=D
// LCL = SP
@SP
D=M
@LCL
M=D
// goto function_name
@Class2.set
0;JMP
(Sys.init$RETURN_ADDRESS.1)
// pop temp 0
@5
D=A
@0
D=D+A
@R13
M=D
@SP
M=M-1
A=M
D=M
@R13
A=M
M=D
// call Class1.get 0
// push return-address, lcl, arg, this, that
@Sys.init$RETURN_ADDRESS.2
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1
// ARG = SP - num_args - 5
@SP
D=M
@0
D=D-A
@5
D=D-A
@ARG
M=D
// LCL = SP
@SP
D=M
@LCL
M=D
// goto function_name
@Class1.get
0;JMP
(Sys.init$RETURN_ADDRESS.2)
// call Class2.get 0
// push return-address, lcl, arg, this, that
@Sys.init$RETURN_ADDRESS.3
D=A
@SP
A=M
M=D
@SP
M=M+1
@LCL
D=M
@SP
A=M
M=D
@SP
M=M+1
@ARG
D=M
@SP
A=M
M=D
@SP
M=M+1
@THIS
D=M
@SP
A=M
M=D
@SP
M=M+1
@THAT
D=M
@SP
A=M
M=D
@SP
M=M+1
// ARG = SP - num_args - 5
@SP
D=M
@0
D=D-A
@5
D=D-A
@ARG
M=D
// LCL = SP
@SP
D=M
@LCL
M=D
// goto function_name
@Class2.get
0;JMP
(Sys.init$RETURN_ADDRESS.3)
// label Sys.init$WHILE
(Sys.init$WHILE)
// goto Sys.init$WHILE
@Sys.init$WHILE
0;JMP
// function Class1.set 0
(Class1.set)
// push constant 0
@0
D=A
@SP
A=M
M=D
@SP
M=M+1
// pop temp 0
@5
D=A
@0
D=D+A
@R13
M=D
@SP
M=M-1
A=M
D=M
@R13
A=M
M=D
(Class1.set$LOCALS)
// push temp 0
@5
D=A
@0
D=D+A
A=D
D=M
@SP
A=M
M=D
@SP
M=M+1
// if-goto Class1.set$INIT_LOCAL
@SP
M=M-1
A=M
D=M
@Class1.set$INIT_LOCAL
D;JNE
@Class1.set$LOCALS_END
0;JMP
(Class1.set$INIT_LOCAL)
// push constant 0
@0
D=A
@SP
A=M
M=D
@SP
M=M+1
// push temp 0
@5
D=A
@0
D=D+A
A=D
D=M
@SP
A=M
M=D
@SP
M=M+1
// push constant 1
@1
D=A
@SP
A=M
M=D
@SP
M=M+1
// sub
@SP
M=M-1
A=M
D=M
@R13
M=D
@SP
M=M-1
A=M
D=M
@R14
M=D
@R13
D=D-M
@SP
A=M
M=D
@SP
M=M+1
// pop temp 0
@5
D=A
@0
D=D+A
@R13
M=D
@SP
M=M-1
A=M
D=M
@R13
A=M
M=D
// goto Class1.set$LOCALS
@Class1.set$LOCALS
0;JMP
(Class1.set$LOCALS_END)
// push argument 0
@ARG
D=M
@0
D=D+A
A=D
D=M
@SP
A=M
M=D
@SP
M=M+1
// pop static 0
@SP
M=M-1
A=M
D=M
@Class1.0
M=D
// push argument 1
@ARG
D=M
@1
D=D+A
A=D
D=M
@SP
A=M
M=D
@SP
M=M+1
// pop static 1
@SP
M=M-1
A=M
D=M
@Class1.1
M=D
// push constant 0
@0
D=A
@SP
A=M
M=D
@SP
M=M+1
// return
// FRAME = LCL
@LCL
D=M
@endFrame
M=D
// RET = RAM[FRAME - 5]
@5
A=D-A
D=M
@retAddr
M=D
// RAM[ARG] = pop()
@SP
M=M-1
A=M
D=M
@ARG
A=M
M=D
//SP = ARG + 1
@ARG
D=M
@1
D=D+A
@SP
M=D
// (segmentPointer) = RAM[FRAME - (decrement)]
@1
D=A
@endFrame
A=M-D
D=M
@THAT
M=D
// (segmentPointer) = RAM[FRAME - (decrement)]
@2
D=A
@endFrame
A=M-D
D=M
@THIS
M=D
// (segmentPointer) = RAM[FRAME - (decrement)]
@3
D=A
@endFrame
A=M-D
D=M
@ARG
M=D
// (segmentPointer) = RAM[FRAME - (decrement)]
@4
D=A
@endFrame
A=M-D
D=M
@LCL
M=D
@retAddr
A=M
0;JMP
// function Class1.get 0
(Class1.get)
// push constant 0
@0
D=A
@SP
A=M
M=D
@SP
M=M+1
// pop temp 0
@5
D=A
@0
D=D+A
@R13
M=D
@SP
M=M-1
A=M
D=M
@R13
A=M
M=D
(Class1.get$LOCALS)
// push temp 0
@5
D=A
@0
D=D+A
A=D
D=M
@SP
A=M
M=D
@SP
M=M+1
// if-goto Class1.get$INIT_LOCAL
@SP
M=M-1
A=M
D=M
@Class1.get$INIT_LOCAL
D;JNE
@Class1.get$LOCALS_END
0;JMP
(Class1.get$INIT_LOCAL)
// push constant 0
@0
D=A
@SP
A=M
M=D
@SP
M=M+1
// push temp 0
@5
D=A
@0
D=D+A
A=D
D=M
@SP
A=M
M=D
@SP
M=M+1
// push constant 1
@1
D=A
@SP
A=M
M=D
@SP
M=M+1
// sub
@SP
M=M-1
A=M
D=M
@R13
M=D
@SP
M=M-1
A=M
D=M
@R14
M=D
@R13
D=D-M
@SP
A=M
M=D
@SP
M=M+1
// pop temp 0
@5
D=A
@0
D=D+A
@R13
M=D
@SP
M=M-1
A=M
D=M
@R13
A=M
M=D
// goto Class1.get$LOCALS
@Class1.get$LOCALS
0;JMP
(Class1.get$LOCALS_END)
// push static 0
@Class1.0
D=M
@SP
A=M
M=D
@SP
M=M+1
// push static 1
@Class1.1
D=M
@SP
A=M
M=D
@SP
M=M+1
// sub
@SP
M=M-1
A=M
D=M
@R13
M=D
@SP
M=M-1
A=M
D=M
@R14
M=D
@R13
D=D-M
@SP
A=M
M=D
@SP
M=M+1
// return
// FRAME = LCL
@LCL
D=M
@endFrame
M=D
// RET = RAM[FRAME - 5]
@5
A=D-A
D=M
@retAddr
M=D
// RAM[ARG] = pop()
@SP
M=M-1
A=M
D=M
@ARG
A=M
M=D
//SP = ARG + 1
@ARG
D=M
@1
D=D+A
@SP
M=D
// (segmentPointer) = RAM[FRAME - (decrement)]
@1
D=A
@endFrame
A=M-D
D=M
@THAT
M=D
// (segmentPointer) = RAM[FRAME - (decrement)]
@2
D=A
@endFrame
A=M-D
D=M
@THIS
M=D
// (segmentPointer) = RAM[FRAME - (decrement)]
@3
D=A
@endFrame
A=M-D
D=M
@ARG
M=D
// (segmentPointer) = RAM[FRAME - (decrement)]
@4
D=A
@endFrame
A=M-D
D=M
@LCL
M=D
@retAddr
A=M
0;JMP
// function Class2.set 0
(Class2.set)
// push constant 0
@0
D=A
@SP
A=M
M=D
@SP
M=M+1
// pop temp 0
@5
D=A
@0
D=D+A
@R13
M=D
@SP
M=M-1
A=M
D=M
@R13
A=M
M=D
(Class2.set$LOCALS)
// push temp 0
@5
D=A
@0
D=D+A
A=D
D=M
@SP
A=M
M=D
@SP
M=M+1
// if-goto Class2.set$INIT_LOCAL
@SP
M=M-1
A=M
D=M
@Class2.set$INIT_LOCAL
D;JNE
@Class2.set$LOCALS_END
0;JMP
(Class2.set$INIT_LOCAL)
// push constant 0
@0
D=A
@SP
A=M
M=D
@SP
M=M+1
// push temp 0
@5
D=A
@0
D=D+A
A=D
D=M
@SP
A=M
M=D
@SP
M=M+1
// push constant 1
@1
D=A
@SP
A=M
M=D
@SP
M=M+1
// sub
@SP
M=M-1
A=M
D=M
@R13
M=D
@SP
M=M-1
A=M
D=M
@R14
M=D
@R13
D=D-M
@SP
A=M
M=D
@SP
M=M+1
// pop temp 0
@5
D=A
@0
D=D+A
@R13
M=D
@SP
M=M-1
A=M
D=M
@R13
A=M
M=D
// goto Class2.set$LOCALS
@Class2.set$LOCALS
0;JMP
(Class2.set$LOCALS_END)
// push argument 0
@ARG
D=M
@0
D=D+A
A=D
D=M
@SP
A=M
M=D
@SP
M=M+1
// pop static 0
@SP
M=M-1
A=M
D=M
@Class2.0
M=D
// push argument 1
@ARG
D=M
@1
D=D+A
A=D
D=M
@SP
A=M
M=D
@SP
M=M+1
// pop static 1
@SP
M=M-1
A=M
D=M
@Class2.1
M=D
// push constant 0
@0
D=A
@SP
A=M
M=D
@SP
M=M+1
// return
// FRAME = LCL
@LCL
D=M
@endFrame
M=D
// RET = RAM[FRAME - 5]
@5
A=D-A
D=M
@retAddr
M=D
// RAM[ARG] = pop()
@SP
M=M-1
A=M
D=M
@ARG
A=M
M=D
//SP = ARG + 1
@ARG
D=M
@1
D=D+A
@SP
M=D
// (segmentPointer) = RAM[FRAME - (decrement)]
@1
D=A
@endFrame
A=M-D
D=M
@THAT
M=D
// (segmentPointer) = RAM[FRAME - (decrement)]
@2
D=A
@endFrame
A=M-D
D=M
@THIS
M=D
// (segmentPointer) = RAM[FRAME - (decrement)]
@3
D=A
@endFrame
A=M-D
D=M
@ARG
M=D
// (segmentPointer) = RAM[FRAME - (decrement)]
@4
D=A
@endFrame
A=M-D
D=M
@LCL
M=D
@retAddr
A=M
0;JMP
// function Class2.get 0
(Class2.get)
// push constant 0
@0
D=A
@SP
A=M
M=D
@SP
M=M+1
// pop temp 0
@5
D=A
@0
D=D+A
@R13
M=D
@SP
M=M-1
A=M
D=M
@R13
A=M
M=D
(Class2.get$LOCALS)
// push temp 0
@5
D=A
@0
D=D+A
A=D
D=M
@SP
A=M
M=D
@SP
M=M+1
// if-goto Class2.get$INIT_LOCAL
@SP
M=M-1
A=M
D=M
@Class2.get$INIT_LOCAL
D;JNE
@Class2.get$LOCALS_END
0;JMP
(Class2.get$INIT_LOCAL)
// push constant 0
@0
D=A
@SP
A=M
M=D
@SP
M=M+1
// push temp 0
@5
D=A
@0
D=D+A
A=D
D=M
@SP
A=M
M=D
@SP
M=M+1
// push constant 1
@1
D=A
@SP
A=M
M=D
@SP
M=M+1
// sub
@SP
M=M-1
A=M
D=M
@R13
M=D
@SP
M=M-1
A=M
D=M
@R14
M=D
@R13
D=D-M
@SP
A=M
M=D
@SP
M=M+1
// pop temp 0
@5
D=A
@0
D=D+A
@R13
M=D
@SP
M=M-1
A=M
D=M
@R13
A=M
M=D
// goto Class2.get$LOCALS
@Class2.get$LOCALS
0;JMP
(Class2.get$LOCALS_END)
// push static 0
@Class2.0
D=M
@SP
A=M
M=D
@SP
M=M+1
// push static 1
@Class2.1
D=M
@SP
A=M
M=D
@SP
M=M+1
// sub
@SP
M=M-1
A=M
D=M
@R13
M=D
@SP
M=M-1
A=M
D=M
@R14
M=D
@R13
D=D-M
@SP
A=M
M=D
@SP
M=M+1
// return
// FRAME = LCL
@LCL
D=M
@endFrame
M=D
// RET = RAM[FRAME - 5]
@5
A=D-A
D=M
@retAddr
M=D
// RAM[ARG] = pop()
@SP
M=M-1
A=M
D=M
@ARG
A=M
M=D
//SP = ARG + 1
@ARG
D=M
@1
D=D+A
@SP
M=D
// (segmentPointer) = RAM[FRAME - (decrement)]
@1
D=A
@endFrame
A=M-D
D=M
@THAT
M=D
// (segmentPointer) = RAM[FRAME - (decrement)]
@2
D=A
@endFrame
A=M-D
D=M
@THIS
M=D
// (segmentPointer) = RAM[FRAME - (decrement)]
@3
D=A
@endFrame
A=M-D
D=M
@ARG
M=D
// (segmentPointer) = RAM[FRAME - (decrement)]
@4
D=A
@endFrame
A=M-D
D=M
@LCL
M=D
@retAddr
A=M
0;JMP
