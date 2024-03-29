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
// push constant 4
@4
D=A
@SP
A=M
M=D
@SP
M=M+1
// call Main.fibonacci 1
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
@1
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
@Main.fibonacci
0;JMP
(Sys.init$RETURN_ADDRESS.0)
// label Sys.init$END
(Sys.init$END)
// goto Sys.init$END
@Sys.init$END
0;JMP
// function Main.fibonacci 0
(Main.fibonacci)
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
(Main.fibonacci$LOCALS)
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
// if-goto Main.fibonacci$INIT_LOCAL
@SP
M=M-1
A=M
D=M
@Main.fibonacci$INIT_LOCAL
D;JNE
@Main.fibonacci$LOCALS_END
0;JMP
(Main.fibonacci$INIT_LOCAL)
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
// goto Main.fibonacci$LOCALS
@Main.fibonacci$LOCALS
0;JMP
(Main.fibonacci$LOCALS_END)
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
// push constant 2
@2
D=A
@SP
A=M
M=D
@SP
M=M+1
// lt
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
@R14
D=M
@R13
D=D-M 
@Main.fibonacci$TRUE.0
D;JLT
@Main.fibonacci$FALSE.0
0;JMP
(Main.fibonacci$TRUE.0)
D=-1
@Main.fibonacci$END.0
0;JMP
(Main.fibonacci$FALSE.0)
D=0
(Main.fibonacci$END.0)
@SP
A=M
M=D
@SP
M=M+1
// if-goto Main.fibonacci$N_LT_2
@SP
M=M-1
A=M
D=M
@Main.fibonacci$N_LT_2
D;JNE
// goto Main.fibonacci$N_GE_2
@Main.fibonacci$N_GE_2
0;JMP
// label Main.fibonacci$N_LT_2
(Main.fibonacci$N_LT_2)
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
// label Main.fibonacci$N_GE_2
(Main.fibonacci$N_GE_2)
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
// push constant 2
@2
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
// call Main.fibonacci 1
// push return-address, lcl, arg, this, that
@Main.fibonacci$RETURN_ADDRESS.0
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
@1
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
@Main.fibonacci
0;JMP
(Main.fibonacci$RETURN_ADDRESS.0)
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
// call Main.fibonacci 1
// push return-address, lcl, arg, this, that
@Main.fibonacci$RETURN_ADDRESS.1
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
@1
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
@Main.fibonacci
0;JMP
(Main.fibonacci$RETURN_ADDRESS.1)
// add
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
D=D+M
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
