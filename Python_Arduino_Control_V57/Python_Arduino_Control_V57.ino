#include <AccelStepper.h>
#define BUFFER_LENGTH 50
# define steptime 0.360/2 //seconds
//A = X; outer frame
//B = Y; inner frame

int wp = 0;
int ledPin = 13; // Most Arduinos have an on-board LED on pin 13.

// Define connections for Motor A
const int DIR_PIN_A = 5;
const int STEP_PIN_A = 2;
const int enPin = 8;

// Define connections for Motor B
const int DIR_PIN_B = 6;
const int STEP_PIN_B = 3;

//Microstepping
const int micro = 8;

// Steps per revolution for your stepper motors
const float stepsPerRevolution = 200*micro;

int instA = 0;
int instB = 0;
//String bufferA = "0";
//int bufferA_int = 0;
//String bufferB = "0";
//int bufferB_int = 0;

int prevPointA = 0;
int prevPointB = 0;
float distToGoA = 1000000;
float distToGoB = 1000000;

int ABcounter = 0; // Enable alternating A and B readlines
long timedelay = 0;

int dt = 0;

// Create instances of the stepper motors
AccelStepper
stepperA(AccelStepper::DRIVER,
STEP_PIN_A, DIR_PIN_A);
AccelStepper
stepperB(AccelStepper::DRIVER,

STEP_PIN_B, DIR_PIN_B);

void setup() {
// Declare pins as Outputs
pinMode(STEP_PIN_A, OUTPUT);
pinMode(DIR_PIN_A, OUTPUT);
pinMode(enPin, OUTPUT);

pinMode(STEP_PIN_B, OUTPUT);
pinMode(DIR_PIN_B, OUTPUT);
pinMode(enPin, OUTPUT);

pinMode(A0, OUTPUT);

// Initialize the LED pin as an output
pinMode(ledPin, OUTPUT);

// Set the maximum speed and acceleration for the motors
//stepperA.setMaxSpeed(2*7*stepsPerRevolution/60);
//stepperA.setAcceleration(2*0.2*stepsPerRevolution/(2*3.14159265));
stepperA.setMaxSpeed(4*7*stepsPerRevolution/60);
//stepperA.setSpeed(2*7*stepsPerRevolution/60);
//stepperA.setAcceleration(50);

//stepperB.setMaxSpeed(2*7*stepsPerRevolution/60);
//stepperB.setAcceleration(2*0.2*stepsPerRevolution/(2*3.14159265));
stepperB.setMaxSpeed(4*7*stepsPerRevolution/60);
//stepperB.setSpeed(2*7*stepsPerRevolution/60);
//stepperB.setAcceleration(50);

//stepperA.moveTo(instA);
//stepperB.moveTo(instB);

// Start the serial communication
Serial.begin(115200);

}

String a;
String b;

int bufferA[BUFFER_LENGTH] = {0};

int bufferB[BUFFER_LENGTH] = {0};

void loop() {

if(Serial.available()){
char inByte = Serial.read();
if(inByte == ','){
  ABcounter = 1;
}else if(inByte == ';'){
ABcounter = 0;
bufferA[wp] = a.toInt();
bufferB[wp] = b.toInt();
// Serial.print("b_A: ");
// Serial.print(bufferA[wp]);
// Serial.print("b_B: ");
// Serial.println(bufferB[wp]);
wp++;
a = "";
b = "";
digitalWrite(A0, HIGH);
digitalWrite(ledPin, HIGH);
}

if ((57 >= inByte && 48 <= inByte)) {
  if(ABcounter == 0){
  a += inByte;
  } else{
  b += inByte;
  }
}
// Serial.print("a:");
// Serial.print(a);
// Serial.print("b:");
// Serial.println(b);
}
distToGoA = bufferA[0] - stepperA.currentPosition();
distToGoB = bufferB[0] - stepperB.currentPosition();

stepperA.moveTo(bufferA[0]);
stepperA.setSpeed((bufferA[0]-prevPointA)/steptime);
stepperA.runSpeedToPosition();
stepperB.moveTo(bufferB[0]);
stepperB.setSpeed((bufferB[0]-prevPointB)/steptime);
stepperB.runSpeedToPosition();

if ((abs(stepperA.distanceToGo()) < 4) && (abs(stepperB.distanceToGo()) < 4) && (wp != 0)) {
  prevPointA = bufferA[0];
  prevPointB = bufferB[0];
for (int i = 1; i < BUFFER_LENGTH;
i++){

bufferA[i-1] = bufferA[i];
bufferB[i-1] = bufferB[i];
}
//stepperA.moveTo(bufferA[0]);
//stepperB.moveTo(bufferB[0]);
wp = wp-1;
}

if (wp < (BUFFER_LENGTH-1) && millis()-timedelay > 100) {
Serial.print(wp);
Serial.println("READY");
timedelay = millis();
}

// Serial.print("d_A: ");
// Serial.print(stepperA.distanceToGo());
// Serial.print("d_B: ");
// Serial.print(stepperB.distanceToGo());
// Serial.print("A: [");
// Serial.print(bufferA[0]);
// Serial.print(" ");

// Serial.print(bufferA[1]);
// Serial.print(" ");
// Serial.print(bufferA[2]);
// Serial.print(" ");
// Serial.print(bufferA[3]);
// Serial.print(" ");
// Serial.print(bufferA[4]);
// Serial.print("] B: [");
// Serial.print(bufferB[0]);
// Serial.print(" ");
// Serial.print(bufferB[1]);
// Serial.print(" ");
// Serial.print(bufferB[2]);
// Serial.print(" ");
// Serial.print(bufferB[3]);
// Serial.print(" ");
// Serial.print(bufferB[4]);
// Serial.print("] wp:");
// Serial.println(wp);
}