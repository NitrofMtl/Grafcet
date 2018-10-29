/*******************************************************************

This is the grafcet made in this example. 
It is the evolution of a 3 branch. All branch evoluate at the same time.
NOTE: PROGRAMMER HAVE THE RESPONSABILITY TO CONVERGE ALL BRANCH AT THE SAME POINT.

-Branch could contain fork and nested branch, as long as it respect the branch convergeance rule.


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
|    ==========================================
^   |  Branch1(0)        |  Branch1(1)         |  Branch1(2)
|   |                    |                     |
| ____                 ____                  ____
| | 2 |__functionStep2 | 3 |__functionStep3  | 4 |__functionStep4
| |___|                |___|                 |___|
|   |                    |                     |
|   |                    |                     |
| ----- transition3    ----- transition4     ----- transition5
|   |   readChar_a       |   readChar_c        |   readChar_d
|   |                    |                     |
| ____                   |                     |
| | 5 |___ functionStep5 |                     |
| |___|                  |                     |
|   |                    |                     |
|   |                    |                     |
| ----- transition6      |                     |*to converge fork, point receptivity to NULL step
|   |   readChar_e       |                     | 
|   |====================|=====================|
|                        |
|                        |  *to converge fork, call converge(&step)
|                      ____
|                      | 6 | ___ functionStep6
|                      |___|
|                        |
|                        |
|                      ----- transition7 readCahr_e
|                        |
|________________________|

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

Receptivity recp1; //initiate all receptivity object, receptivity can only be used once in the grafcet
Receptivity recp2;
Receptivity recp3; 
Receptivity recp4;
Receptivity recp5;
Receptivity recp6;
Receptivity recp7;

Step step1; //initiate a step, ste have to be use only once in the grafcet
Step step2;
Step step3;
Step step4;
Step step5;
Step step6;

//create the branch object
Branch branch1;

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

void functionStep6() {
  Serial.println("Function step 6 triggered");
}

//--------------

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
  recp2.setNextStep(branch1); //enter the branch


//////*******************
  branch1.branch(0) = functionStep2; //set branch frist step as usual step
  branch1.branch(0).initial->receptivity = &recp3;

  recp3 = readChar_a;
  recp3.setNextStep(step5);

  step5 = functionStep5;
  step5.receptivity = &recp6;

  recp6 = readChar_e;
  // ==>>> do not set next step at the last transition, null step on receptivity sing the end of the branch
//////*******************

  //add s second branch:
  branch1.add();
 
  branch1.branch(1) = functionStep3; //set branch frist step as usual step
  branch1.branch(1).initial->receptivity = &recp4;

  recp4 = readChar_c;
//////*******************
  //add a third branch
  branch1.add();

  branch1.branch(2) = functionStep4; //set branch frist step as usual step
  branch1.branch(2).initial->receptivity = &recp5;

  recp5 = readChar_d;

//////*******************
// ==>> now all branch are done, close it and set the next step:
branch1.converge(step6); 

step6 = functionStep6;
step6.receptivity = &recp7;

recp7 = readChar_e;
recp7.setNextStep(grafcet.backToInitial());

}

void loop() {
  grafcet.handler();
}