// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

//CNETZER - project 04

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed. 
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.

// psuedo code:
//(loop)
//set var to KEYBOARD
//if keyboard <> 0, 
  //loop through screen and set all the screen registers to 1?
//else
  //set all screen registers to 0
//loop

(LOOP)
  @R0
  D=M //R0 should be 0
  @KBD
  D=D-M //if keyboard is pressed, M!=0, so D will be not be 0
  @WHITE
  D;JEQ //jump to top of loop if keyboard is not pressed --> D = 0 - 0 = 0
  @SCREEN
  M=-1
  @LOOP
  0;JMP
(WHITE)
  @SCREEN
  M=0
  @LOOP
  0;JMP