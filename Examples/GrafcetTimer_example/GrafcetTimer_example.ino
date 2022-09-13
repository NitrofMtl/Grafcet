/*******************************************************************

This is the grafcet made in this example. 
This is the basic usage of Grafcet timer.
Grafcet timer inherit from Receptivity.

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
|    |_R_|
|      |
|      |
|    ----- T1:5000 ms
|      |
|      |
|    ____
|    | 3 | ___(print "X3")
|    |_R_|
|      |
|      |
|    ----- Q3 readChar_d
|      |
|______|

/*******************************************************************/

#include <Grafcet.h>
#include "TimeOut.h"

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
GrafcetTimer T1;
Transition Q3( [](){return readChar('c');} );


//For convenience, use Step Rising and
//lambdas function for simplicity, but step support function pointer and
//functor with operator void() overload. 
//Lambdas also support capture.
StepRising X1( [](){Serial.println("X1");} );
StepRising X2( [](){Serial.println("X2");} );
StepRising X3( [](){Serial.println("X3");} );
//--------------

void setup() {

  Serial.begin(9600);
  Serial.println("system start");
  delay(1000);
  
  grafcet.setInitial(Q1); //first condition to start, and the first step
  Q1.setNextStep(X1);
  X1.setTransition(Q2);
  Q2.setNextStep(X2);
  X2.setTransition(T1);
  T1.set(X2, 5000);//set into the timer with the upstream Step and the delay.
  T1.setNextStep(X3);
  X3.setTransition(Q3);
  Q3.setNextStep(grafcet.backToInitial());//loop back to initial step

}

void loop() {
  grafcet.handler();
  TimeOut::handler();
}