// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Mult.asm

//CNETZER - project 04

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)
//
// This program only needs to handle arguments that satisfy
// R0 >= 0, R1 >= 0, and R0*R1 < 32768.

//psuedo code:
//set i = 0
//set sum = 0
//(loop)
//if i - R1 = 0
  //break
//else  
  //sum = sum + R0
//loop
//set R2 = sum

  @i
  M=0
  @sum
  M=0
(LOOP)
  @i
  D=M 
  @R1
  D=D-A //i - R1, we want to add R0 to itself, R1 times
  @END //set A reg to END
  D;JEQ //jump to END
  @R0
  D=M
  @sum
  M=D+M
  D=M
  @R2
  M=D //store the current total in R2
  @i
  M=M+1  
  @LOOP
  0;JMP
(END)
  @END
  0;JMP