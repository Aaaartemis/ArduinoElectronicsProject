//top port is 101
#include <Servo.h>

Servo servo1; //declaring an object
int servoPin = 8;



void setup() {
  Serial.begin(9600);
  servo1.attach(servoPin); //links servo to pin

}

void loop() {
  servo1.write(90); //turn positive

  //using MMA to get angle --------------------
  
  delay(1000);

}
