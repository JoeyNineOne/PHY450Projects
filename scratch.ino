#include <Servo.h>//imported
#include <Adafruit_MotorShield.h> //imported
#include <Encoder.h>
Servo servo;
const int distLim = 25;
const int SERVO = 9;
const int pingPin = 7;
long distance = 0;
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *motor1 = AFMS.getMotor(1);
Adafruit_DCMotor *motor2 = AFMS.getMotor(2);
Encoder myEnc(3, 6);
Encoder myEnc2(4, 2);
int speed1 = 150;
int speed2 = 150;

void setup() {
    Serial.begin(9600);
    servo.attach(SERVO);
    if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
    // if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
        Serial.println("Could not find Motor Shield. Check wiring.");
        while (1);
    }
    Serial.println("Motor Shield found.");
    motor1->setSpeed(0);
    motor1->run(RELEASE);
    motor2->setSpeed(0);
    motor2->run(RELEASE);

}

void turnRight(){
  for(int i=0;i<269;i++){
      motor1->setSpeed(speed1);
      motor1->run(FORWARD);
      motor2->setSpeed(speed2+28);
      motor2->run(BACKWARD);
  }
    motor1->setSpeed(0);
    motor2->setSpeed(0);
    delay(500);
}

void turnLeft(){
    for(int i=0;i<269;i++){
      motor1->setSpeed(speed1);
      motor1->run(BACKWARD);
      motor2->setSpeed(speed2+28);
      motor2->run(FORWARD);
  }
    motor1->setSpeed(0);
    motor2->setSpeed(0);
    delay(500);

}
void move(){
    motor1->setSpeed(speed1);
    motor1->run(FORWARD);
    motor2->setSpeed(speed2+40);
    motor2->run(FORWARD);
}
void sensorLeft(){
    for (int pos = 0; pos <= 90; pos += 1) {
        servo.write(pos);            
        delay(15);                      
    }
}
void sensorRight(){
    for (int pos = 90; pos <= 0; pos -= 1) {
        servo.write(pos);            
        delay(15);                      
    }
}
bool checkForEnd(){
    
}
long getDist(){
    long duration, inches, cm;

    // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
    // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
    pinMode(pingPin, OUTPUT);
    digitalWrite(pingPin, LOW);
    delayMicroseconds(2);
    digitalWrite(pingPin, HIGH);
    delayMicroseconds(5);
    digitalWrite(pingPin, LOW);

    // The same pin is used to read the signal from the PING))): a HIGH pulse
    // whose duration is the time (in microseconds) from the sending of the ping
    // to the reception of its echo off of an object.
    pinMode(pingPin, INPUT);
    duration = pulseIn(pingPin, HIGH);

    // convert the time into a distance
    cm = microsecondsToCentimeters(duration);
    return cm;
}
long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the object we
  // take half of the distance travelled.
  return microseconds / 29 / 2;
}

void encodersPls(){
    long newPosition = myEnc.read();
    long newPosition2 = myEnc2.read();
  if (newPosition > newPosition2) {
      // increase speed of motor #2
      speed2+=1;
      speed1-=1;
  }else if (newPosition < newPosition2) {
      // increase speed of motor #1
      speed1+=1;
      speed2-=1;
  }
}

void loop() {
   distance = getDist();
   Serial.println(distance);
   while(distance>distLim){
       for(int i=0;i<20;i++){
         move();
       }
        distance = getDist();
   }
    motor1->setSpeed(0);
    motor2->setSpeed(0);
    turnLeft();
    distance = getDist();
    if(distance<distLim){// left side is blocked
        turnRight();// here, both the front and the left are blocked, so we turn right
        turnRight();
    }else{// left side is NOT blocked
        turnRight();// here, both the front and the left are blocked, so we turn right
        turnRight();
        distance = getDist();
        if(distance<distLim){
            // this means that the left side isn't blocked but the right side is,
        turnLeft();
        turnLeft();  
        }else{
            // this means that neither the left side nor the right is blocked.
            // we've reached the endpoint, turn around.
        turnRight();   
        }
    }
}