# Grafcet 1.0.0

## This library is to easely translate Grafcet representation ( sequencial function chart ) into Arduino code.

For more details about grafcet checkout the [wiki](https://en.wikipedia.org/wiki/Sequential_function_chart).

Or look at [YOUTUBE: Sequencial Function Chart](https://www.youtube.com/watch?v=0buTIBRm6Ps).




### It contain 2 kind of object: Step and Recepticvity.

Grafcet library is build as a link list.

When a step is active, the handler make the callback and after check it the Step's receptivity is validated, if so, deactivate the step and activate the next step.

Each instance of Grafcet start with the first Step: "initial".



----
## Step objects:

- **A Step is active when a upstream Recepticvity is validated.**
- **A Step is deactivated when the next downstream Receptivity is validated.**

The Step constructor take a action as argument, could be function pointer, lambda function (with capture or not) or functor.

### Step type:

-**Step:** Do action when each time the handler is called.

-**StepRising:** Do action only once, when the step is activated.

-**StepFalling:** Do action only once, when the step is deactivated.

-**StepOnChange:** Do action once, when the step is activated, once when deactivated.

````
//function pointer example
void callback1() {}
Step X1( callback1 );

//functor example
struct FunctorExample
{
  FunctorExample(/*some arguments*/) {}
  void operator()() {/*actual callback*/}
};
FunctorExample callback2;
StepRising X2( callback2 );

//lambdas with capture by reference example
auto callback3 = [&](){/**/};
StepFalling X3( callback3 );

//lambdas without capture, pass declare directly into the declaration example
StepOnchange X4( 
    [](){/*On Step activation callback*/},
    [](){/*On Step deactivation callback*/}
);

````

### Step member function:

````
bool isActive(); //return true is Step is active

unsigned long activeTime(); //return the number of ms since the Step is active

````

### A Step could be a waiting step that have no action:
````
Step x1( WAIT );
````




-----

## Recepticvity objects:

### -**Transition:**


The Transition constructor take a boolean callback as argument, could be function pointer, lambda function (with capture or not) or functor.


````

//function pointer example
bool condition1() {return /*some boolean expression*/;}
Transition Q1( callback1 );

//functor example
struct FunctorConditionExample
{
  FunctorExample(/*some arguments*/) {}
  void operator()() {return /*some boolean expression*/;}
};
FunctorConditionExample callback2;
Transition Q2( callback2 );

//lambdas with capture by reference example
auto condition3 = [&](){return /*some boolean expression*/;};
Transition Q3( callback3 );

//lambdas without capture, directly declare into transition declaration
Transition Q4( [&](){return /*some boolean expression*/;} );

````

### If the condition if fulfilled, the Grafcet evoluate to the next Step.



### A condition could be set as always true:
````
Transition Q1( TRUE );
````

----
----

## Basic usage

To use Grafcet, you need to declare an instance, and put the handler into loop() function.
After, into setup, make the link of your step and condition. The Grafcet will star at "initial". Lets use the Step and transition example upper, and link them:

````
Grafcet grafcet;

/*
Step and Transition declaration
*/

void setup()
{
    grafcet.setInitial(Q1); //first condition to start
    Q1.setNextStep(X1); //set the transition following step
    X1.setTransition(Q2); //set the transition of this step
    Q2.setNextStep(X2);
    X2.setTransition(Q3);
    Q3.setNextStep(X3);
    X3.setTransition(Q4);
    Q4.setNextStep(grafcet.backToInitial());//loop back to initial step
}

void loop()
{
    grafcet.handler();
}

````

----
----

# Divergeance OR: Fork

Divergeance OR are, in a Grafcet, more than one path the automation could take.
If one condition is true, do one Step, if the other is true, do another Step.

### **It is the responsability of the programmer to make sure only one condition could be true at the time.**

If a fork have 2 path, each one with a condition A and B, it is better to and negation to the other path:

````
Condition A = A && !B
Condition B = B && !A
````

## Fork usage:

-Declare a Fork at global scope.


In setup:
At the Step followed by the Divergeance OR, make the Receptivity to the Fork.
Add fork, set the receptivity.
Once froked, the following Step are program as regular.

````
Fork fork1;
/*
...
*/

void setup()
{
  /*
...
  */
  X7.setTransition(fork1);
  fork1[0] = Q2;
    Q2.setNextStep(X5);
    ...
  fork1add();
  fork1[1] = Q3;
    Q3.setNextStep(X6);
  ...
}

````

# Convergeance OR:

Convergeance OR is more than one Transition pointing to the same Step:

````
Step X8
//...
//Transition Q13 and Q17 will converge at step X8
Q13.setNextStep(X8);
Q17.setNextStep(X8);
````

----
----

# Divergeance AND: Branch.

When a Receptivity pointing to a Branche, all Branch[] become active at the same time and progress in parallel.

## Rules:
- **A Branche must enter all branch at the same point.**
- **To converge, all last Step of each Branch have to be active, then the Branch Receptivity could be checked.**


## Branch usage:

- In global scope, declare the Branch;

- In void setup():
  - Make the Receptivity followed by the Branch pointing to it.
  - Add branch.
  - Set the fisrt step of each branch.
  - Once branched, the following Step are program as regular.


````
Branch branch1;

void setup()
{
  Q2.setNextStep(branch1);
  branch1[0] = X2; //step the first step of the branch
    X2.setTransition(Q3); //progress trough the branch
    Q3.setNextStep(X5);
...
  branch1.add(); //add a new branch
  branch1[1] = X3; //step the first step of the branch
    X3.setTransition(Q4); //progress trough the branch
...
}
````

# Convergeance AND:

To converge a Branch; 
 - Set the next transition. 
 - Make all branch's last Step pointing to the branch convergeance.

 ````
branch1.setConvergeTransition(Q6); //set the transition that converge the branch
    X7.setTransition(branch1.converge()); //converge every last step of every branch
    X6.setTransition(branch1.converge());
 ````
----
----
# Grafcet timer

As a receptivity, you could use a timer.
The transition will be fulfilled after the the time set is pass.

## Usage:
- declare timer into global scope.
- Link it in void setup().

````
GrafcetTimer T1;

void setup()
{
  X2.setTransition(T1);
  T1.set(X2, 5000);//set into the timer with the upstream Step and the delay.
  T1.setNextStep(X3);
}
````

The timer could be reset to 0.
- Have effect only if the upstream Step is active.

````
`void some function()
{
  T1.reset();
}

````
----
----
# Grafcet counter:

### Counter, as his name imply, count some event.

## Usage:

````
//Declare an instance
Counter C1;

//increment counter
C1++;

//decrement counter
C1--;

//get the value of the counter
C1.count();

//reset the counter to 0
C1.reset();

//Set the counter to a specific value
C1.set(int);

//compare the counter
// operator == != < <= >= >
if ( C1 <= 3 ) {...}
````

It can be use to validate a count in a Receptivity.

````
Transition Q2( [](){return C1<=3;} );
````

----
----
# SyncMerory

In a Branch or in a Nested Grafcet, A receptivity could wait for another step to also be active.

To do so, step have 2 member function.

**Bool Step.active(): Get if the step is active:**
````
Transition Q4( [](){return X7.isActive();} );
````

**unsigned long Step.activeTime(): Get for how much time the step is active:**
````
Transition Q5( []()
{
  if( X4.activeTime() >= 30000 ) {
    return true;
  }
  return false;
} );
````

### **SyncMerory Class**

Use as Receptivity: return true when the Step is active.

**Usage:**
- Declare SyncMemory into global scope with the step to sync on.
- Link it with the Grafcet into setup()

````
SyncMemory S1(X6);

void Setup()
{
  X10.setTransition(S1);
  S1.setNextStep(X11);
}
````
----
----
# Nested Grafcet


A Grafcet could contain a sub Grafcet routine.
To do so, the sub-grafcet handler have to be put in  any Step that it is needed to progress. Those Step have to be countinuous.


----
----



# **__For usage details, check out the examples.__**