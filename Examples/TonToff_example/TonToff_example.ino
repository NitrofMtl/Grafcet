/*******************************************************************

This is the grafcet made in this example. 
This is the basic usage of TON and TOFF object.
They are hybrid grafcet object that inherit from StepObj and Transition

_____________________ ======
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
|                        |   transition2
|                      ----- readChar_b
|                        |
|                        |
|                      ____   (ACTION)
|                      |TON| ___ functionTON
|                      |___|
|                        |
|                        |   
|                      ----- Time over
|                        |
|                        |
|                      ____   (ACTION)
|                      | 2 | ___ functionStep2
|                      |___|
|                        |
|                        |   transition3
|                      ----- readChar_c
|                        |
|                        |
|                      ____   (ACTION)
|                      |TOFF| ___ functionTOFF
|                      |___ |
|                        |
|                        |   
|                      ----- Time over
|                        |
|                        |
|                      ____   (ACTION)
|                      | 3 | ___ functionStep3
|                      |___|
|                        |
|                        |   transition4
|                      ----- readChar_d
|                        |
|________________________|

/*******************************************************************/

#include <Grafcet.h>
#include <TimeOut.h>


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


Receptivity recp1; //initiate all receptivity object, receptivity can only be used once in the grafcet
Receptivity recp2;
Receptivity recp3;
Receptivity recp4; 

Step step1; //initiate a step, ste have to be use only once in the grafcet
Step step2;
Step step3;

TON ton; //initiate Timer on object
TOFF toff; //initiate Timer off object

//create all transition condition
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

void functionTON() {
  Serial.println("Function TON triggered");
}


void functionStep2() {
  Serial.println("Function step 2 triggered");
}

void functionTOFF() {
  Serial.println("Function TOFF triggered");
}


void functionStep3() {
  Serial.println("Function step 3 triggered");
}

void setup() {

  Serial.begin(9600);
  Serial.println("system start");
  delay(1000);

  grafcet.setInitial(recp1);  //set initial transition
  recp1 = readChar_a;
  recp1.setNextStep(step1);

  step1 = functionStep1;
  step1.receptivity = &recp2;

  recp2 = readChar_b;
  recp2.setNextStep(ton);

  ton = functionTON;
  ton.delay(2000);
  ton.setNextStep(step2);

  step2 = functionStep2;
  step2.receptivity = &recp3;

  recp3 = readChar_c;
  recp3.setNextStep(toff);

  toff = functionTOFF;
  toff.delay(4000);
  toff.setNextStep(step3);

  step3 = functionStep3;
  step3.receptivity = &recp4;

  recp4 = readChar_d;
  recp4.setNextStep(grafcet.backToInitial());
}

void loop() {
  grafcet.handler();
  TimeOut::handler();
}