#include <Servo.h>//imported
#include <Adafruit_MotorShield.h> //imported
Servo servo;
const int distLim = 20;
const int SERVO = 1;
const int pingPin = 7;
const int motorPort = 2;
int distance = 0;
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *myMotor = AFMS.getMotor(motorPort);

void setup() {
    Serial.begin(9600);
    servo.attach(SERVO);
    if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
    // if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
        Serial.println("Could not find Motor Shield. Check wiring.");
        while (1);
    }
    Serial.println("Motor Shield found.");

    // Set the speed to start, from 0 (off) to 255 (max speed)
    myMotor->setSpeed(150);
    myMotor->run(FORWARD);
    // turn on motor
    myMotor->run(RELEASE);
}

void turnLeft(){

}

void turnRight(){

}
void move(){

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
    Serial.print(cm);
    Serial.print("cm");
    Serial.println();
    return cm;
}
long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the object we
  // take half of the distance travelled.
  return microseconds / 29 / 2;
}
void loop() {
    distance = getDist();
    while(distance>distLim){
        distance = getDist();
        move();
    }
    sensorLeft();
    distance = getDist();
    if(distance<distLim){// left side is blocked
        sensorRight();// return the sensor to its original pos
        turnRight();// here, both the front and the left are blocked, so we turn right
    }else{// left side is NOT blocked
        sensorRight();
        sensorRight();// turns the sensor to the right. Done twice because each
        // time it turns 90 deg.
        distance = getDist();
        if(distance<distLim){
            // this means that the left side isn't blocked but the right side is,
            // so we turn left.
        sensorLeft();
        turnLeft();  
        }else{
            // this means that neither the left side nor the right is blocked.
            // we've reached the endpoint, turn around.
        sensorLeft();
        turnRight();   
        turnRight(); 
        }
    }
}