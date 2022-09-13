/*******************************************************************

This is the grafcet made in this example. 
It is the basic evolution of 3 step and transition in a strait line with the step 1 and 3
handling a sub-grafcet.

To use it, just put the sub_grafcet handler inside any main grafcet step** you want the sub grafcet to evoluate.
**The Step must be continuous.

NOTE: Step number mark with a 'R', 'F', 'C are rising, falling and onChange step respectively.

GrafcetMain
___ ======
|  ||Init||
|   ======
|      |
|      |    Receptivity, Transition
|    ----- Q1 readChar_a
|      |
|      |
|    ____   (ACTION)
|    | 1 | __(Grafcet2.handler)
|    |___|
|      |
|      |
|    ----- Q2 readChar_b
^      |
|      |
|    ____
|    | 2 | ___(print "X2")
|    |_R_|
|      |
|      |
|    ----- Q3 readChar_c
|      |
|      |
|    ____
|    | 3 | ___(Grafcet2.handler)
|    |___|
|      |
|      |
|    ----- Q4 readChar_d
|      |
|______|



Grafcet2
___ ======
|  ||Init||
|   ======
|      |
|      |    Receptivity, Transition
|    ----- Q5 readChar_e
|      |
|      |
|    ____   (ACTION)
|    | 4 | __(print "X4")
|    |_R_|
|      |
|      |
|    ----- Q6 readChar_f
^      |
|      |
|    ____
|    | 5 | ___(print "X5")
|    |_R_|
|      |
|      |
|    ----- Q7 readChar_g
|      |
|      |
|    ____
|    | 6 | ___(print "X6")
|    |_R_|
|      |
|      |
|    ----- Q8 readChar_h
|      |
|______|

/*******************************************************************/
#include <Arduino.h>

#include <Grafcet.h>

Grafcet grafcetMain;
Grafcet grafcet2;

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

//Main Grafcet
//Step containing a handler must be continuous
Step X1( [](){grafcet2.handler();} );
StepRising X2( [](){Serial.println("X2");} );
Step X3( [](){grafcet2.handler();} );

//Sub grafcet
StepRising X4( [](){Serial.println("X4");} );
StepRising X5( [](){Serial.println("X5");} );
StepRising X6( [](){Serial.println("X6");} );

//initiate all receptivity object, receptivity can only be used once in the grafcet
//Also using lambdas function for simplicity, but Transition support Bool() expression and 
//functor with operator bool() overload.
//Lambdas also support capture.
//Main grafcet
Transition Q1( [](){return readChar('a');} );
Transition Q2( [](){return readChar('b');} );
Transition Q3( [](){return readChar('c');} );
Transition Q4( [](){return readChar('d');} );

//Sub grafcet
Transition Q5( [](){return readChar('e');} );
Transition Q6( [](){return readChar('f');} );
Transition Q7( [](){return readChar('g');} );
Transition Q8( [](){return readChar('h');} );

//For convenience, use lambdas function for simplicity, but step support function pointer and
//functor with operator void() overload. 
//Lambdas also support capture.



void setup() {

    Serial.begin(9600);
    Serial.println("system start");
    delay(1000);

    //set link on main Grafcet
    grafcetMain.setInitial(Q1); //first condition to start, and the first step
    Q1.setNextStep(X1);
    X1.setTransition(Q2);
    Q2.setNextStep(X2);
    X2.setTransition(Q3);
    Q3.setNextStep(X3);
    X3.setTransition(Q4);
    Q4.setNextStep(grafcetMain.backToInitial());//loop back to initial step

    //set link on sub grafcet
    grafcet2.setInitial(Q5);
    Q5.setNextStep(X4);
    X4.setTransition(Q6);
    Q6.setNextStep(X5);
    X5.setTransition(Q7);
    Q7.setNextStep(X6);
    X6.setTransition(Q8);
    Q8.setNextStep(grafcet2.backToInitial());

}

void loop() {
  grafcetMain.handler();
}

