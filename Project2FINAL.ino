#include <Adafruit_MotorShield.h> //imported
#include <Encoder.h>
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

Adafruit_DCMotor *leftMotor = AFMS.getMotor(1);
Adafruit_DCMotor *rightMotor = AFMS.getMotor(2);
int leftSpeed = 255; /*speed of left motor*/
int rightSpeed = 255; /*speed of right motor*/
int caliF = 3;
int caliT = 1; // values for an 8.5V battery
Encoder rightEncoder(3, 5);
Encoder leftEncoder(2, 6);
void setup() {
  pinMode(7,INPUT_PULLUP);
  while(digitalRead(7)){
    AFMS.begin();
  }
  leftMotor->setSpeed(leftSpeed);
  leftMotor->run(BACKWARD);
  leftMotor->run(RELEASE);
  rightMotor->setSpeed(rightSpeed);
  rightMotor->run(FORWARD);
  rightMotor->run(RELEASE);
}
void adjustMotors() {

  int leftDist = leftEncoder.read(); /*distance recorded by left encoder*/
  int rightDist = rightEncoder.read(); /*distance recorded by right encoder*/
  rightDist = abs(rightDist);
  leftDist = abs(leftDist);
  Serial.print(leftDist);
  Serial.print(" ");
  Serial.println(rightDist);
  //reset encoders
  leftEncoder.write(0);
  rightEncoder.write(0);
//
  //note: encoders record negative numbers
  if (leftDist > rightDist) { //left faster than right
    leftSpeed = max(leftSpeed - 1, 0);
    rightSpeed = min(rightSpeed + 1, 255);
  }
  else { //right faster than left
    leftSpeed = min(leftSpeed + 1, 255); 
    rightSpeed = max(rightSpeed - 1, 0);
  }
  //setting the new speeds to motors
  leftMotor->setSpeed(leftSpeed);
  rightMotor->setSpeed(rightSpeed);
  delay(20);
}
void loop() {
  uint8_t i;

  leftMotor->run(BACKWARD);
  rightMotor->run(FORWARD);
  for(int i=0;i<125-caliF;i++){
   adjustMotors();
   delay(30);
  }
  leftMotor->run(FORWARD);
  rightMotor->run(FORWARD);
  for(int i=0;i<9-caliT;i++){
   adjustMotors();
   delay(30);
  }
  leftMotor->run(BACKWARD);
  rightMotor->run(FORWARD);
  for(int i=0;i<142-caliF;i++){
   adjustMotors();
   delay(30);
  }
  leftMotor->run(FORWARD);
  rightMotor->run(FORWARD);
  for(int i=0;i<9-caliT;i++){
   adjustMotors();
   delay(30);
  }
  leftMotor->run(BACKWARD);
  rightMotor->run(FORWARD);
  for(int i=0;i<145-caliF;i++){
   adjustMotors();
   delay(30);
  }
  leftMotor->run(FORWARD);
  rightMotor->run(FORWARD);
  for(int i=0;i<9-caliT;i++){
   adjustMotors();
   delay(30);
  }
  leftMotor->run(BACKWARD);
  rightMotor->run(FORWARD);
  for(int i=0;i<100-caliF;i++){
   adjustMotors();
   delay(30);
  }
  while(1){
  leftMotor->setSpeed(0);
  rightMotor->setSpeed(0);
  }
}
