// function SimpleFunction.test 2
(SimpleFunction.test)
// push constant 2
@2
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
// push constant 0
@0
D=A
@SP
A=M
M=D
@SP
M=M+1
// pop temp 1
@5
D=A
@1
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
(SimpleFunction.test.LOCALS)
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
// push temp 1
@5
D=A
@1
D=D+A
A=D
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
// if-goto SimpleFunction.SimpleFunction.INIT_LOCAL
@SP
M=M-1
A=M
D=M
@SimpleFunction.SimpleFunction.INIT_LOCAL
D;JNE
@SimpleFunction.test.LOCALS_END
0;JMP
(SimpleFunction.test.INIT_LOCAL)
// push constant 0
@0
D=A
@SP
A=M
M=D
@SP
M=M+1
// push temp 1
@5
D=A
@1
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
// pop temp 1
@5
D=A
@1
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
// goto SimpleFunction.SimpleFunction.test.LOCALS
@SimpleFunction.SimpleFunction.test.LOCALS
0;JMP
(SimpleFunction.test.LOCALS_END)
// push local 0
@LCL
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
// push local 1
@LCL
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
// not
@SP
M=M-1
A=M
D=!M
@SP
A=M
M=D
@SP
M=M+1
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
