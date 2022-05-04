/*
LIST OF FUNCTIONS:
setup(): initializes the hardware.
turnRight(): turns the robot right 90 degrees.
turnLeft(): turns the robot left 90 degrees.
move(): moves the robot forward for a fraction of a second (works when paired with for loops)
sensorLeft(): UNUSED, turns the servo (that connects to the sensor) 90 degrees left.
sensorRight(): UNUSED, turns the servo (that connects to the sensor) 90 degrees right.
getDist(): returns the distance between the ultrasonic sensor and the closest object in front of it.
microsecondsToCentimeters(): changes values from microseconds to centimeters.
encodersPls(): UNUSED, adjusts the motors with the encoders so the motors are always going at the same rate.
ending(): executes the path from the endpoint back to the start with the memorized information (direction, length, etc.)
celebration(): turns the robot to the left for an infinite amount of time.
loop(): tells the robot to sense which direction to go as well as how far to go in the maze, and stores that information.
*/

#include <Servo.h>
#include <Adafruit_MotorShield.h>
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

#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7

void setup() { // the lines in this function are pretty much all copied and pasted from OneNotes
  Serial.begin(9600);
  lcd.begin(16, 2);
  servo.attach(SERVO);
  if (!AFMS.begin()) {
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
  // this function the robot slightly to the right for a total of 260 times
  // (with trial-and-error it seems like 260 loops turn the robot exactly 90 degrees)
  for (int i = 0; i < 260; i++) {
    motor1->setSpeed(speed1);
    motor1->run(FORWARD);
    motor2->setSpeed(speed2 + 28); 
    // becasue our encoders didn't work, we manually set the speed of motor2 to be a little more than
    // that of motor1, to account for the fact that motor2 was physically always slower than motor1.
   // the number 28 was also from trial-and-error.
    motor2->run(BACKWARD);
  }
  motor1->setSpeed(0);
  motor2->setSpeed(0);
  delay(2000);
}

void turnLeft() {
    // this function the robot slightly to the right for a total of 269 times
  // (with trial-and-error it seems like 269 loops turn the robot exactly 90 degrees)
  for (int i = 0; i < 269; i++) {
    motor1->setSpeed(speed1);
    motor1->run(BACKWARD);
    // becasue our encoders didn't work, we manually set the speed of motor2 to be a little more than
    // that of motor1, to account for the fact that motor2 was physically always slower than motor1.
    // the number 28 was also from trial-and-error.
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
    // becasue our encoders didn't work, we manually set the speed of motor2 to be a little more than
    // that of motor1, to account for the fact that motor2 was physically always slower than motor1.
    // the number 20 was also from trial-and-error.
  motor2->run(FORWARD);
}
void sensorLeft() { // this function was UNUSED.
  for (int pos = 0; pos <= 90; pos += 1) {
    servo.write(pos);
    delay(15);
  }
}
void sensorRight() { // this function was UNUSED.
  for (int pos = 90; pos <= 0; pos -= 1) {
    servo.write(pos);
    delay(15);
  }
}
long getDist() {
// this function was taken directly from OneNote and it gets the distance between
// the sensor and the thing in front that is closest to it.
  long duration, inches, cm;
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);
  lcd.setBacklight(YELLOW);
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);
  cm = microsecondsToCentimeters(duration);
  return cm;
}
long microsecondsToCentimeters(long microseconds) { // this function was taken directly from OneNote.
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the object we
  // take half of the distance travelled.
  return microseconds / 29 / 2;
}

void encodersPls() { // this function was UNUSED.
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
void ending() { // I recommend people first read the comments of the loop() function then coming back.
// This function is called when endpoint has been reached and it leads the robot back to the start.
  for (int i = ARRAYLEN; i >= 1; i--) {
    if (timing[i] == -99) { // if nothing has been written on this slot of the array, skip it.
      continue;
    }
    for (int j = 0; j <= (int)timing[i] * 2.3; j++) {
      // something has been written on this slot of the array, great. Now, call the move() function
      // the same number of times as we did when we came to this place at the beginning (which is stored in timing[]).
      // the *2.3 is a value that came from trial-and-error.
      move();
    }
    if (turning[i - 1] == 0) { // if we turned left on the way here, now we have to turn right
      turnRight();
    } else if (turning[i - 1] == 1) { // if we turned right on the way here, now we have to turn left
      turnLeft();
    }
  }

  for (int j = 0; j <= (int)timing[0] * 2.3; j++) { // that last for loop takes care of all movements
                                                    // and turnings EXCEPT for the last move, which is
                                                    // executed right here with timing[0].
    move();
  }
  while (1) { // we're done. Spin to celebrate.
    celebration();
  }
}
void celebration() { //spins the robot eternally for celebration.
  while (1) {
    motor1->setSpeed(speed1);
    motor1->run(BACKWARD);
    motor2->setSpeed(speed2 + 28);
    motor2->run(FORWARD);
  }
}
void loop() {
  int movement = 0;
  distance = getDist();
  while (distance > distLim) { // this while loops says that while the distance between the wall and the
                              // robot is bigger than a threshold (which we've set to 20 cm), keep on moving
                              // and record how many times this while loop has execute (for returning to the start later).
    movement++;
    move();
    distance = getDist();
  }
  motor1->setSpeed(0);// stop if the distance between the wall and the robot becomes smaller than 20 cm.
  motor2->setSpeed(0);
  timing[marker] = movement;// record the amount of times the while loop has executed. This is how we
                            // store the length of a movement.
  turnLeft(); // robot now turns left.
  distance = getDist(); // records the distance between the wall and the robot.
  if (distance < distLim) { // if the distance from the last line is smaller than 20 cm.
                            // we can pretty much guarantee that the left side is blocked.
    turnRight();// therefore, both the front and the left are blocked, so we have to turn right.
    turnRight();// turn right twice because once only turns 90 degrees.
    turning[marker] = 1; // record down that we've turned RIGHT instead of left at this crossroad.
  } else { // this means that the front is blocked but the left isn't
    turnRight();// now we have to turn to check if the right side is blocked or not.
    turnRight();
    distance = getDist(); // records the distance between the wall and the robot.
    if (distance < distLim) { // this means that the right side is blocked.
      turnLeft(); // therefore our only option is to turn to the left side and drive there.
      turnLeft();// turn left twice because once only turns 90 degrees.
      turning[marker] = 0; //record down that we've turned LEFT instead of right at this crossroad.
    } else {
      // this means that neither the left side nor the right is blocked.
      // we've reached the endpoint, turn around. Now execute the remembered path to return to the start.
      turnRight();
      ending();
    }
  }
  marker++;// records that one movement has been done. Now moving on to the next movement.
}