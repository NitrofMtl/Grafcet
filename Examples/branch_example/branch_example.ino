/*******************************************************************

This is the grafcet made in this example. 
It is the evolution of a 3 branch. All branch evoluate at the same time.
NOTE: PROGRAMMER HAVE THE RESPONSABILITY TO CONVERGE ALL BRANCH AT THE SAME POINT.

-Branch could contain fork and nested branch, as long as it respect the branch convergeance rule.

NOTE: Step number mark with a 'R', 'F', 'C are rising, falling and onChange step respectively.

_____________________ ======
|                    ||Init||
|                     ======
|                        |
|                        |   (Transition)
|                      ----- Q1 readChar_a
|                        |
|                        |
|                      ____   (ACTION)
|                      | 1 |___(print "X1")
|                      |_R_|
|                        |
|                        |   Q2
|                      ----- readChar_b
|                        |
|                        |
|    ==============================================
^   |  Branch1(0)            |  Branch1(1)         |  Branch1(2)
|   |                        |                     |
| ____                     ____                  ____
| | 2 |__(print "X2")      | 3 |__(print "X3")   | 4 |__(print "X4")
| |_R_|                    |_R_|                 |_R_|
|   |                        |                     |
|   |                        |                     |
| ----- Q3                 ----- Q4                |
|   |   readChar_c           |   readChar_d        |
|   |                        |                     |
| ____                     ____                    |
| | 5 |__(print "X5")      | 6 |__(print "X6")     |
| |_R_|                    |_R_|                   |
|   |                        |                     |
|   |                        |                     |
| ----- Q5                   |                     |
|   |   readChar_e           |                     | 
| ____                       |                     |
| | 7 |__(print "X7")        |                     |
| |_R_|                      |                     |                     |
|   |                        |                     |
|   |                        |                     |
|   |                        |                     |
|   |                        |                     |
|   |========================|=====================|
|                            |
|                            |
|                          ----- Q6 readCahr_f
|                            |
|                            |
|                          ____
|                          | 8 | ___(print "X8")
|                          |_R_|
|                            |
|                            |
|                          ----- Q7 readCahr_g
|                            |
|____________________________|

/**************************************************************************************/

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

//

//initiate all receptivity object, receptivity can only be used once in the grafcet
//Also using lambdas function for simplicity, but Transition support Bool() expression and 
//functor with operator bool() overload.
//Lambdas also support capture.
Transition Q1( [](){return readChar('a');} );
Transition Q2( [](){return readChar('b');} );
Transition Q3( [](){return readChar('c');} );
Transition Q4( [](){return readChar('d');} );
Transition Q5( [](){return readChar('e');} );
ConvergeTransition Q6( [](){return readChar('f');} );
Transition Q7( [](){return readChar('g');} );

//For convenience, use step rising, the callback will append when the step become active
//Also using lambdas function for simplicity, but step support function pointer and
//functor with operator void() overload. 
//Lambdas also support capture.
StepRising X1( [](){Serial.println("X1");} );
StepRising X2( [](){Serial.println("X2");} );
StepRising X3( [](){Serial.println("X3");} );
StepRising X4( [](){Serial.println("X4");} );
StepRising X5( [](){Serial.println("X5");} );
StepRising X6( [](){Serial.println("X6");} );
StepRising X7( [](){Serial.println("X7");} );
StepRising X8( [](){Serial.println("X8");} );

//create the branch object
Branch branch1;


//--------------

void setup() {

  Serial.begin(9600);
  Serial.println("system start");
  delay(1000);

  grafcet.setInitial(Q1);  //set initial transition
  Q1.setNextStep(X1);
  X1.setTransition(Q2);
  Q2.setNextStep(branch1);
  branch1[0] = X2; //step the first step of the branch
    X2.setTransition(Q3); //progress trough the branch
    Q3.setNextStep(X5);
    X5.setTransition(Q5);
    Q5.setNextStep(X7);
  branch1.add(); //add a new branch
  branch1[1] = X3; //step the first step of the branch
    X3.setTransition(Q4); //progress trough the branch
    Q4.setNextStep(X6);
  branch1.add();
  branch1[2] = X4;
  branch1.setConvergeTransition(Q6); //set the transition that converge the branch
    X7.setTransition(branch1.converge()); //converge every last step of every branch
    X6.setTransition(branch1.converge());
    X4.setTransition(branch1.converge());
  Q6.setNextStep(X8);
  X8.setTransition(Q7);
  Q7.setNextStep(grafcet.backToInitial());

}

void loop() {
  grafcet.handler();
}