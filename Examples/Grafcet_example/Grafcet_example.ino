/*******************************************************************

This is the grafcet made in this example. 
It is the basic evolution of 3 step and transition in a strait line.

NOTE: Step number mark with a 'R', 'F', 'C are rising, falling and onChange step respectively.

___ ======
|  ||Init||
|   ======
|      |
|      |    Receptivity, Transition
|    ----- Q1 readChar_a
|      |
|      |
|    ____   (ACTION)
|    | 1 | __(print "X1")
|    |_R_|
|      |
|      |
|    ----- Q2 readChar_b
^      |
|      |
|    ____
|    | 2 | ___(print "X2")
|    |___|
|      |
|      |
|    ----- Q3 readChar_c
|      |
|      |
|    ____
|    | 3 | ___(print "X3")
|    |_F_|
|      |
|      |
|    ----- Q4 readChar_d
|      |
|    ____
|    | 4 | ___ON(print "X4,")
|    |_C_|   OFF("print "X4 deactivated, back to INITIAL")
|      |
|      |
|    ----- Q5 readChar_e
|      |
|______|

/*******************************************************************/

#include <Grafcet.h>

Grafcet grafcet;

//function to test condition
//if the good char is reed for serial, return true
bool readChar(char input){ //function to test condition
  static char rdChar;
  if(Serial.available()){
    rdChar = Serial.read();
  }
  if(rdChar==input){
    Serial.print("read char: ");
    Serial.println(input);
    rdChar = 0;
    return true;
  }    
  return false;
}

//initiate all receptivity object, receptivity can only be used once in the grafcet
//Also using lambdas function for simplicity, but Transition support Bool() expression and 
//functor with operator bool() overload.
//Lambdas also support capture.
Transition Q1( [](){return readChar('a');} );
Transition Q2( [](){return readChar('b');} );
Transition Q3( [](){return readChar('c');} );
Transition Q4( [](){return readChar('d');} );
Transition Q5( [](){return readChar('e');} );

//For convenience, use lambdas function for simplicity, but step support function pointer and
//functor with operator void() overload. 
//Lambdas also support capture.

//Step rising, do callback one when step is activated
StepRising X1( [](){Serial.println("X1");} );
//Step do callback continiously
Step X2( [](){Serial.println("X2"); delay(500);} );
//Step Falling, do callback one when step is deactivated
StepFalling X3( [](){Serial.println("X3");} );
//StepOnChange will do the first callback at activation,
//and the second at deactivation
StepOnChange X4(
  [](){Serial.println("X4");},
  [](){Serial.println("X4 deactivated, back to INITIAL");}
); 
//--------------

void setup() {

  Serial.begin(9600);
  Serial.println("system start");
  delay(1000);
  
  grafcet.setInitial(Q1); //first condition to start, and the first step
  Q1.setNextStep(X1);
  X1.setTransition(Q2);
  Q2.setNextStep(X2);
  X2.setTransition(Q3);
  Q3.setNextStep(X3);
  X3.setTransition(Q4);
  Q4.setNextStep(X4);
  X4.setTransition(Q5);
  Q5.setNextStep(grafcet.backToInitial());//loop back to initial step

}

void loop() {
  grafcet.handler();
}

