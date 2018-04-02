/*
// CSE321 Term Project, Tucker J. Polomik
// Carduino, autonomous Arduino driven vehicle
// Special thanks to the folks at Sparkfun who provided a great motor driving header which I used extensively
*/

/******************************************************************************
TestRun.ino
TB6612FNG H-Bridge Motor Driver Example code
Michelle @ SparkFun Electronics
8/20/16
https://github.com/sparkfun/SparkFun_TB6612FNG_Arduino_Library

Uses 2 motors to show examples of the functions in the library.  This causes
a robot to do a little 'jig'.  Each movement has an equal and opposite movement
so assuming your motors are balanced the bot should end up at the same place it
started.

Resources:
TB6612 SparkFun Library

Development environment specifics:
Developed on Arduino 1.6.4
Developed with ROB-9457
******************************************************************************/

// This is the library for the TB6612 that contains the class Motor and all the
// functions
#include <SparkFun_TB6612.h>

// Pins for all inputs, keep in mind the PWM defines must be on PWM pins
// the default pins listed are the ones used on the Redbot (ROB-12097) with
// the exception of STBY which the Redbot controls with a physical switch
#define AIN1 2
#define BIN1 7
#define AIN2 4
#define BIN2 8
#define PWMA 5
#define PWMB 6
#define STBY 9

// these constants are used to allow you to make your motor configuration 
// line up with function names like forward.  Value can be 1 or -1
const int offsetA = 1;
const int offsetB = 1;
const int trigPin = 10;
const int echoPin = 11;
long duration;
int distance;
int servoDirArr[] = {-200, 200};

// Initializing motors.  The library will allow you to initialize as many
// motors as you have memory for.  If you are using functions like forward
// that take 2 motors as arguements you can either write new functions or
// call the function more than once.
Motor motor1 = Motor(AIN1, AIN2, PWMA, offsetA, STBY);
Motor motor2 = Motor(BIN1, BIN2, PWMB, offsetB, STBY);
int randServo;

void setup()
{
 pinMode(trigPin, OUTPUT);
 pinMode(echoPin, INPUT);
 Serial.begin(9600);
 distance = 50;
}

void runER()
{
   digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  //receive pwm 
  duration = pulseIn(echoPin, HIGH);
  distance=duration*0.034/2;
  if (distance > 200 || distance == 0) { distance = 200; } else { distance = duration*0.034/2; }
  Serial.print("Distance: ");
  Serial.println(distance);
}

int count;

void loop()
{
   //Use of the drive function which takes as arguements the speed
   //and optional duration.  A negative speed will cause it to go
   //backwards.  Speed can be from -255 to 255.  Also use of the 
   //brake function which takes no arguements.
//   runER();
//  
//   motor2.drive(255);
//   motor1.drive(-255,1000);
//   motor1.brake();
//   delay(1000);
// motor2.drive(200); // use for re0wiring test
if (count == 0) {
runER();
count = 1;
}

while (count == 1) {
  // if the distance is greater than 20 cm, drive for 1.5s and then remeasure distance
  if (distance > 20) {
  motor2.drive(200, 1500);
  motor2.brake();
  delay(1000);
  count = 0;
  Serial.println("Driving forwards");
  } else {
  //if the distance is less than 20 cm, adjust servos and drive backwards for 1.5s then remeasure distance
  randServo = random(0, 2); // 0 = - 200 = turn servo left ; 1 = + 200 = turn servo right
  motor2.drive(-200, 1000);
  motor1.drive(servoDirArr[randServo], 1000);
  motor2.brake();
  motor1.brake();
  delay(1000);
  count = 0;
  Serial.println("Driving backwards");
  Serial.print("RandServo: ");
  Serial.println(randServo);
  }
}
}
