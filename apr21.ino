#include <Servo.h>//imported
#include <Adafruit_MotorShield.h> //imported
#include <Encoder.h>
#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>
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
int ARRAYLEN = 20;
int turning[20];// set 0 to turn left, 1 to right
int timing[20];
int marker = 0;
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

// These #defines make it easy to set the backlight color
#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7


void(* resetFunc) (void) = 0;
void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
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
  for (int i = 0; i < ARRAYLEN; i++) {
    turning[i] = -99;
    timing[i] = -99;
  }
  lcd.setBacklight(TEAL);
}

void turnRight() {
  for (int i = 0; i < 260; i++) {
    motor1->setSpeed(speed1);
    motor1->run(FORWARD);
    motor2->setSpeed(speed2 + 28);
    motor2->run(BACKWARD);
  }
  motor1->setSpeed(0);
  motor2->setSpeed(0);
  delay(2000);
}

void turnLeft() {
  for (int i = 0; i < 269; i++) {
    motor1->setSpeed(speed1);
    motor1->run(BACKWARD);
    motor2->setSpeed(speed2 + 28);
    motor2->run(FORWARD);
  }
  motor1->setSpeed(0);
  motor2->setSpeed(0);
  delay(2000);

}
void move() {
  motor1->setSpeed(speed1);
  motor1->run(FORWARD);
  motor2->setSpeed(speed2 + 20);
  motor2->run(FORWARD);
  lcd.setBacklight(GREEN);
}
void sensorLeft() {
  for (int pos = 0; pos <= 90; pos += 1) {
    servo.write(pos);
    delay(15);
  }
}
void sensorRight() {
  for (int pos = 90; pos <= 0; pos -= 1) {
    servo.write(pos);
    delay(15);
  }
}
long getDist() {
  long duration, inches, cm;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);
  lcd.setBacklight(YELLOW);

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

void encodersPls() {
  long newPosition = myEnc.read();
  long newPosition2 = myEnc2.read();
  if (newPosition > newPosition2) {
    // increase speed of motor #2
    speed2 += 1;
    speed1 -= 1;
  } else if (newPosition < newPosition2) {
    // increase speed of motor #1
    speed1 += 1;
    speed2 -= 1;
  }
}
void ending() {
  for (int i = ARRAYLEN; i >= 1; i--) { // takes care of every movement and turn EXCEPT
    // for the last one.
    if (timing[i] == -99) {
      continue;
    }
    for (int j = 0; j <= (int)timing[i] * 2.3; j++) {
      move();
    }
    if (turning[i - 1] == 0) {
      turnRight();// because we turned left on the way here, now we have to
      // turn right.
    } else if (turning[i - 1] == 1) {
      turnLeft();
    }
  }
  for (int j = 0; j <= (int)timing[0] * 2.3; j++) { // takes care for the last movement.
    move();
  }
  while (1) {
    celebration();
  }
}
void celebration() {
  while (1) {
    motor1->setSpeed(speed1);
    motor1->run(BACKWARD);
    motor2->setSpeed(speed2 + 28);
    motor2->run(FORWARD);
  }
}
void loop() {
  lcd.setBacklight(RED);
  int movement = 0;
  distance = getDist();
  while (distance > distLim) {
    movement++;
    move();
    distance = getDist();
  }
  motor1->setSpeed(0);
  motor2->setSpeed(0);
  timing[marker] = movement;
  turnLeft();
  distance = getDist();
  if (distance < distLim) { // left side is blocked
    turnRight();// here, both the front and the left are blocked, so we turn right
    turnRight();
    turning[marker] = 1; //right is 1
  } else { // left side is NOT blocked
    turnRight();
    turnRight();
    distance = getDist();
    if (distance < distLim) {
      // this means that the left side isn't blocked but the right side is,
      turnLeft();
      turnLeft();
      turning[marker] = 0; //left is 0
    } else {
      // this means that neither the left side nor the right is blocked.
      // we've reached the endpoint, turn around.
      turnRight();
      ending();
    }
  }
  marker++;
}