/*******************************************************************

This is the grafcet made in this example. 
It is the basic evolution of 4 step and transition in a strait line.

___ ======
|  ||Init||
|   ======
|      |
|      |
|    ----- transition1 readChar_a
|      |
|      |
|    ____   (ACTION)
|    | 1 | ___ functionStep1
|    |___|
|      |
|      |
|    ----- transition2 readChar_b
^      |
|      |
|    ____
|    | 2 | ___ continuous(functionStep2)
|    |___|
|      |
|      |
|    ----- transition3 readChar_c
|      |
|      |
|    ____
|    | 3 | ___ functionStep3
|    |___|
|      |
|      |
|    ----- transition4 readChar_d
|      |
|      |
|    ____
|    | 4 | ___ functionStep4
|    |___|
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

Receptivity recp1; //initiate receptivity object
Receptivity recp2;
Receptivity recp3;
Receptivity recp4;

//initiate a step, ste have to be use only once in the grafcet
Step step1; //Step action run only once on step activation
StepContinuous step2; //StepContinuous run continuously at each handler execution while step is active
Step step3;

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


void functionStep1() { //define an action. all action can be use many time on any step needed
  Serial.println("Function step 1 triggered");
}

void functionStep2() {
  Serial.println("Function step 2 triggered");
}

void functionStep3() {
  Serial.println("Function step 3 triggered");
}


//--------------

void setup() {

  Serial.begin(9600);
  Serial.println("system start");
  delay(1000);
  
  grafcet.setInitial(recp1); //first condition to start, and the first step
  recp1 = readChar_a; //add the transition condition to the receptivity
  recp1.setNextStep(step1); //set the next step link to this receptivity

  step1 = functionStep1; //add the action the the step
  step1.receptivity = &recp2; //add the receptivity to the step

  recp2 = readChar_b;
  recp2.setNextStep(step2);

  step2 = functionStep2;
  step2.receptivity = &recp3;

  recp3 = readChar_c;
  recp3.setNextStep(step3);

  step3 = functionStep3;
  step3.receptivity = &recp4;

  recp4 = readChar_d;
  recp4.setNextStep(grafcet.backToInitial()); //loop back to initial step

}

void loop() {
  grafcet.handler();
}

