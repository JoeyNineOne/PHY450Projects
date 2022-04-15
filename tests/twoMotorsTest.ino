#include <Adafruit_MotorShield.h> //imported

Adafruit_MotorShield AFMS = Adafruit_MotorShield();

Adafruit_DCMotor *motor1 = AFMS.getMotor(1);
Adafruit_DCMotor *motor2 = AFMS.getMotor(2);

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Adafruit Motorshield v2 - DC Motor test!");

  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
  // if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");

  // Set the speed to start, from 0 (off) to 255 (max speed)
  motor1->setSpeed(150);
  motor1->run(FORWARD);
  // turn on motor
  motor1->run(RELEASE);
  motor2->setSpeed(150);
  motor2->run(FORWARD);
  // turn on motor
  motor2->run(RELEASE);
}

void loop() {
  uint8_t i;

  Serial.print("tick");

  motor1->run(FORWARD);
  motor2->run(FORWARD);
  for (i=0; i<255; i++) {
    motor1->setSpeed(i);
    motor2->setSpeed(i);
    delay(10);
  }
  for (i=255; i!=0; i--) {
    motor1->setSpeed(i);
    motor2->setSpeed(i);
    delay(10);
  }

  Serial.print("tock");

  motor1->run(BACKWARD);
  motor2->run(BACKWARD);
  for (i=0; i<255; i++) {
    motor1->setSpeed(i);
    motor2->setSpeed(i);
    delay(10);
  }
  for (i=255; i!=0; i--) {
    motor1->setSpeed(i);
    motor2->setSpeed(i);
    delay(10);
  }

  Serial.print("tech");
  motor1->run(RELEASE);
  motor2->run(RELEASE);
  delay(1000);
}