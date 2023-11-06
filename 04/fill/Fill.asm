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
  //initialize variables
  @i
  M=0
  @SCREEN
  D=A
  @currentWord
  M=D
  @8192
  D=A
  @totalWords
  M=D

  @color
  M=0

  //check for keyboard input
  @KBD
  D=M
  @SCREENLOOP
  D;JEQ //jump to loop to set all pixels to white if keyboard is not pressed --> D = 0
  
  //if KBD <> 0
  @color
  M=-1

  (SCREENLOOP)
    //jump to beginning of program if all pixels have been filled
    @i
    D=M
    @totalWords
    D=D-M
    @LOOP
    D;JEQ 

    //set the word to the color
    @color
    D=M
    @currentWord
    A=M 
    M=D
    
    //update the current word location    
    @currentWord
    M=M+1

    //increment i
    @i
    M=M+1

    @SCREENLOOP
    0;JMP