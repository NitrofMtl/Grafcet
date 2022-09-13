/*******************************************************************

This is the grafcet made in this example. 
It is the evolution of a 3 option fork. each option point to it's own step.
NOTE: PROGRAMMER HAVE THE RESPONSABILITY TO MAKE SURE ONLY ONE CONDITION CAN BE TRUE AT THE TIME AT A FORK

example: if you have a fork that if a-> go to a step, if b-> go to b step.
But to be certain that a and b cannot be true at the same time, programmer should always evaluate fork condition as:

if (a and !b) go to b step,  if (!a and b) go to b step.

Fork inherit from Receptivity.

NOTE: Step number mark with a 'R', 'F', 'C are rising, falling and onChange step respectively.

____________________> ======
|                    ||Init||
|                     ======
|                        |
|                        |
|                      ----- Q1 readChar_a
|                        |
|                        |
|                      ____   (ACTION)
|                      | 1 | ___(print "X1")
|                      |_R_|
|                        |
|                        |fork1
|    __________________________________________ 
^   |                    |                     |
|   |                    |                     |
| ----- Q2             ----- Q3              -----  Q4
|   |   readChar_b       |   readChar_c        |  readChar_d
|   |                    |                     |
| ____                 ____                  ____
| | 2 |___(print "X2") | 3 |___(print "X3")  | 4 |___(print "X4")
| |_R_|                |_R_|                 |_R_|
|   |                    |                     |
|   |                    |                     |
| ----- Q5             ----- Q6              ----- Q7
|   |   readChar_c       |  readChar_d         | readChar_b
|___|                    |_____________________|
|                                  |
|                                  |  *to converge fork, just point receptivity to the same step
|                                ____ * Valid also for step back converge
|                                | 5 |___(print "X5")
|                                |_R_|
|                                  |
|                                  |
|                                ----- Q8 readCahr_e
|                                  |
|__________________________________|


/*******************************************************************/


#include <Grafcet.h>

Grafcet grafcet;

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
Transition Q5( [](){return readChar('c');} );
Transition Q6( [](){return readChar('d');} );
Transition Q7( [](){return readChar('b');} );
Transition Q8( [](){return readChar('e');} );

//For convenience, use step rising, the callback will append when the step become active
//Also using lambdas function for simplicity, but step support function pointer and
//functor with operator void() overload. 
//Lambdas also support capture.
StepRising X1( [](){Serial.println("X1");} );
StepRising X2( [](){Serial.println("X2");} );
StepRising X3( [](){Serial.println("X3");} );
StepRising X4( [](){Serial.println("X4");} );
StepRising X5( [](){Serial.println("X5");} );

Fork fork1;
//--------------

void setup() {

  Serial.begin(9600);
  Serial.println("system start");
  delay(1000);

  grafcet.setInitial(Q1);
  Q1.setNextStep(X1);
  X1.setTransition(fork1);
  fork1[0] = Q2;
    Q2.setNextStep(X2);
    X2.setTransition(Q5);
    Q5.setNextStep(grafcet.backToInitial());//converge
  fork1.add();
  fork1[1] = Q3;
    Q3.setNextStep(X3);
    X3.setTransition(Q6);
    Q6.setNextStep(X5);//converge
  fork1.add();
  fork1[2] = Q4;
    Q4.setNextStep(X4);
    X4.setTransition(Q7);
    Q7.setNextStep(X5);//converge
  X5.setTransition(Q8);
  Q8.setNextStep(grafcet.backToInitial());//converge

}

void loop() {
  grafcet.handler();
}

