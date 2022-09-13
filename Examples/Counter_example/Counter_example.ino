/*******************************************************************

This is the grafcet made in this example. 

Use Counter object into Transition and Step.

Couter could:
-Get value: Counter();
-Increment, decrement: Counter++, Counter--
-Compare value: Counter < <= => > == !=
-Reset counter to 0: Counter.reset()
-Set to a specific value: Counter.set(int)

example: if you have a fork that if a-> go to a step, if b-> go to b step.
But to be certain that a and b cannot be true at the same time, programmer should always evaluate fork condition as:

if (a and !b) go to b step,  if (!a and b) go to b step.



NOTE: Step number mark with a 'R', 'F', 'C are rising, falling and onChange step respectively.

______________> ======
|              ||Init||
|               ======
|                  |
|                  |
|                ----- Q1 readChar_a
|                  |
|                  |
|                ____   (ACTION)
|                | 1 | ___(print "X1", C1++, print C1)
|                |_R_|
|                  |
|                  |fork1
|        ______________________
^        |                    |
|        |                    |
|      ----- Q2             ----- Q3 
|        |   C1 <= 3          |   C1 > 3
|        |                    |
|      ____                 ____
|      | 2 |___(print "X2") | 3 |___(print "X3, reset C1", print "counter reset") 
|      |_R_|                |_R_| 
|        |                    | 
|        |                    | 
|      ----- Q4             ----- Q5  
|        |   readChar_b       |  readChar_c  
|________|____________________|


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

Counter C1;
Fork fork1;
//For convenience, use step rising, the callback will append when the step become active
//Also using lambdas function for simplicity, but step support function pointer and
//functor with operator void() overload. 
//Lambdas also support capture.
StepRising X1( []()
  {
    Serial.print("X1");
    C1++;
    Serial.print(" Counter=");
    Serial.println(C1.count());
  }
);
StepRising X2( [](){Serial.println("X2");} );
StepRising X3( []()
  {
    Serial.print("X3");
    C1.reset();
    Serial.println(" Counter reset !!!");
  }
);


//initiate all receptivity object, receptivity can only be used once in the grafcet
//Also using lambdas function for simplicity, but Transition support Bool() expression and 
//functor with operator bool() overload.
//Lambdas also support capture.
Transition Q1( [](){return readChar('a');} );
Transition Q2( [](){return C1<=3;} );
Transition Q3( [](){return C1>3;} );
Transition Q4( [](){return readChar('b');} );
Transition Q5( [](){return readChar('c');} );




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
    X2.setTransition(Q4);
    Q4.setNextStep(grafcet.backToInitial());//converge
  fork1.add();
  fork1[1] = Q3;
    Q3.setNextStep(X3);
    X3.setTransition(Q5);
    Q5.setNextStep(grafcet.backToInitial());//converge

}

void loop() {
  grafcet.handler();
}

