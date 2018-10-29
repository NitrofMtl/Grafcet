/*******************************************************************

This is the grafcet made in this example. 
It is the evolution of a 3 option fork. each option point to it's own step.
NOTE: PROGRAMMER HAVE THE RESPONSABILITY TO MAKE SURE ONLY ONE OPTION IS POSSIBLE

example: if you have a fork that if a-> go to a step, if b-> go to b step.
But to be certain that a and b cannot be true at the same time, programmer should always evaluate fork condition as:

if (a and !b) go to b step,  if (!a and b) go to b step.

Fork inherit from Receptivity and is use as such.

____________________> ======
|                    ||Init||
|                     ======
|                        |
|                        |
|                      ----- transition1 readChar_a
|                        |
|                        |
|                      ____   (ACTION)
|                      | 1 | ___ functionStep1
|                      |___|
|                        |
|                        |fork1
|    __________________________________________ 
^   |                    |                     |
|   | fork1(0)           | fork1(1)            | fork1(2)
| ----- readChar_b     ----- readChar_c      -----  readChar_d
|   |                    |                     |
|   |                    |                     |
| ____                 ____                  ____
| | 2 |__functionStep2 | 3 |__functionStep3  | 4 |__functionStep4
| |___|                |___|                 |___|
|   |                    |                     |
|   |                    |                     |
| ----- transition2    ----- transition3     ----- transition4
|   |   readChar_c       |  readChar_d         | readChar_b
|___|                    |_____________________|
|                                  |
|                                  |  *to converge fork, just point receptivity to the same step
|                                ____
|                                | 5 | ___ functionStep5
|                                |___|
|                                  |
|                                  |
|                                ----- transition5 readCahr_e
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


Receptivity recp1; //initiate receptivity object, receptivity can only be used once in the grafcet
Receptivity recp2;
Receptivity recp3;
Receptivity recp4;
Receptivity recp5;

Step step1; //initiate a step, ste have to be use only once in the grafcet
Step step2;
Step step3;
Step step4;
Step step5;

Fork fork1; //initiate fork

transition_t readChar_a() { ////define an transition. all transition can be use many time on any Receptivity needed
  return readChar('a'); //any boolean value will fit in return of transition
}

transition_t readChar_b() {
  return readChar('b');
}

transition_t readChar_c() {
  return readChar('c');
}

transition_t readChar_d() {
  return readChar('d');
}

transition_t readChar_e() {
  return readChar('e');
}


void functionStep1() { //define an action. all action can be use many time on any step needed
  Serial.println("Function step 1 triggered");
}

void functionStep2() {
  Serial.println("Function step 2 triggered");
}

void functionStep3() {
  Serial.println("Function step 3 triggered");
}

void functionStep4() {
  Serial.println("Function step 4 triggered");
}

void functionStep5() {
  Serial.println("Function step 5 triggered");
}

//--------------

void setup() {

  Serial.begin(9600);
  Serial.println("system start");
  delay(1000);

  grafcet.setInitial(recp1);
  recp1 = readChar_a;
  recp1.setNextStep(step1);

  step1 = functionStep1;
  step1.receptivity = &fork1;

  //build fork
  fork1.fork(0) = readChar_b;
  fork1.fork(0).setNextStep(step2);

  step2 = functionStep2;
  step2.receptivity = &recp2;

  recp2 = readChar_c;
  recp2.setNextStep(grafcet.backToInitial());
  //close that fork, go bak to initial step


  //add new fork
  fork1.add();
  fork1.fork(1) = readChar_c; //the fork identity is by order added, start with 0 (from fork contructor, first add() is one...)
  fork1.fork(1).setNextStep(step3);

  step3 = functionStep3;
  step3.receptivity = &recp3;

  recp3 = readChar_d;
  recp3.setNextStep(step5); ///close that fork

  fork1.add();
  fork1.fork(2) = readChar_d;
  fork1.fork(2).setNextStep(step4);

  step4 = functionStep4;
  step4.receptivity = &recp4;

  recp4 = readChar_b;
  recp4.setNextStep(step5);///close that fork


  //now all fork are close, procced to grafcet as usual
  step5 = functionStep5;
  step5.receptivity = &recp5;

  recp5 = readChar_e;
  recp5.setNextStep(grafcet.backToInitial());


}

void loop() {
  grafcet.handler();
}

