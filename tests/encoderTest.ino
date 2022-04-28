#include <Encoder.h>

// Change these two numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
Encoder myEnc(3, 6);
Encoder myEnc2(2, 4);
//   avoid using pins with LEDs attached

void setup() {
  Serial.begin(9600);
  Serial.println("Basic Encoder Test:");
}

long oldPosition  = -999;
long oldPosition2  = -999;

void loop() {
  long newPosition = myEnc.read();
  long newPosition2 = myEnc2.read();
//  if (newPosition != oldPosition) {
//    oldPosition = newPosition;
//    Serial.print("1 ");
//    Serial.println(newPosition);
//  }
   if (newPosition2 != oldPosition2) {
    oldPosition2 = newPosition2;
    Serial.print("2 ");
    Serial.println(newPosition2);
  }
  
}