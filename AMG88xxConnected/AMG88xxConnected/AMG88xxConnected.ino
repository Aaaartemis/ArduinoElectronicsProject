#include <Wire.h>
#include <Adafruit_AMG88xx.h>

Adafruit_AMG88xx amg;

float pixels[AMG88xx_PIXEL_ARRAY_SIZE];


void setup() {
    Serial.begin(9600);

    //pin to connect it with the other arduino
    pinMode(13, OUTPUT); //if overheat is true --> high volatge


    Serial.println(F("AMG88xx pixels"));
    bool status;
    // default settings
    status = amg.begin();
    if (!status) {
        Serial.println("Could not find a valid AMG88xx sensor, check wiring!");
        while (1);
    }
    Serial.println("-- Pixels Test --");
    Serial.println();
    delay(100); // let sensor boot up
}


void loop() { 
  //int maxHot = 70; //max temperature before it overheats is 70C
  int maxHot = 40; //to test overheating
  bool overheat = 1;//starting claiming its true for safety reasons
  int lowerVal = 50; //value that we should let it cool down to before restarting again
  int goodToGo = 0; // number of thermal sensors that have a safe temperature to go back to the sun
  int n = AMG88xx_PIXEL_ARRAY_SIZE;
  
  //read all the pixels
  amg.readPixels(pixels);
  
  //prints all temperatures
  /*
  Serial.print("[");
  for(int i=1; i<=n; i++){
    Serial.print(pixels[i-1]);
    Serial.print(", ");
    if( i%8 == 0 ) Serial.println();
  }
  Serial.println("]");
  Serial.println();
  */

  if (overheat == 0){ // only checks for overheating if it is false to begin with
    //checking for overheating
    for(int j=1; j<=n; j++){
      if (pixels[j] >= maxHot) {
        //Serial.print("Warning"); 
        overheat = 1;
      }
    }
  }
  else if (overheat == 1){
    // loop to check each thermal sensor
    goodToGo = 0; 
    for(int j = 0; j < n; j++){
      if (pixels[j] <= lowerVal){
          goodToGo++; //increasing counter of safe thermal sensors
      }
    }
  }
  
  // if statement to make overheat false if all the sensors are safe temperature to return in the sun
  if (goodToGo == n){
   overheat = 0;
  }

  //sending voltage to the other arduino for if its overheating or not
  if (overheat == 1){
    digitalWrite(13, HIGH);  // sends back high voltage
  }
  else if(overheat == 0){
    digitalWrite(13, LOW);  // sends back low voltage
  }
  Serial.println(overheat);
  delay(1000);
}

