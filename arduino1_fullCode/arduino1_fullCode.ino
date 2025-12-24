//includeing everything that needs to be included

//for tft screen ----------------
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

#define TFT_DC 9
#define TFT_CS 10
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

//for LDRs ----------------
int inputLDR1;
int inputLDR2;

//for MMA8451 ----------------
#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>

Adafruit_MMA8451 mma = Adafruit_MMA8451();

//for servo and DHT11----------------
#include "DHT.h"
#define DHTPIN A3
#define DHTTYPE DHT11
#include <Servo.h>

DHT tempSensor = DHT(DHTPIN, DHTTYPE);
Servo servo1; //declaring an object
int servoPin = 8;
int servoInputPin = A3; //where it is gettin ginput from

bool rain = 0; //initialzing value of rain to false
int angle = 90; //initialing angle to 90
bool overheat = 1; //initializing overheating as true
int checkForOverheat; //to convert data from arduino 2 to arduino 1

//---------------

void setup() {
  Serial.begin(9600);

  //to communicate with the other arduino
  pinMode(12, INPUT);

  // for MMA 
  Serial.println("Adafruit MMA8451 test!");
  
  if (! mma.begin()) {
    Serial.println("Couldnt start");
    while (1);
  }
  Serial.println("MMA8451 found!");
  mma.setRange(MMA8451_RANGE_2_G);
  Serial.print("Range = "); Serial.print(2 << mma.getRange());  
  Serial.println("G");

  //for servo and DHT
  servo1.attach(servoPin); //links servo to pin
  tempSensor.begin();

  //for tft screen 
  tft.begin();
  uint8_t rotation=3;
  tft.setRotation(rotation);
  
}

void loop() {
  //collectign data
  //using MMA to get angle --------------------
  mma.read();
  /* Get a new sensor event */ 
  sensors_event_t event; 
  mma.getEvent(&event);

  double accX = event.acceleration.x;
  double accY = event.acceleration.y;
  double accZ = event.acceleration.z;

  double den = sqrt(pow(accY, 2) + pow(accZ, 2));
  float angleAcc = ( atan2 (accX, den) * 180/3.14159265); // returns angle in degrees and with sign

  //for servo and DHT11 --------------------
  Serial.println("From DHT11");
  int humidity = tempSensor.readHumidity();
  float threshold = 80;
  
  
  //printing value of rain
  Serial.println();
  Serial.print("Rain: "); Serial.println(rain);

  //main case1: it is not raining --------------------------------------------------------------------------------
      //we are using the solar panel
  if (rain == 0){
    Solar();

    //connecting with other arduino --------------------
    //checking for overheating
    checkForOverheat = digitalRead(12);   //checking for ovehreating
    if (checkForOverheat == HIGH) {
      overheat = 1;
    } 
    else {
      overheat = 0; 
    }

    //printing that we are using solar panel
    Serial.print("Using solar panel");

    //printing humidity level
    Serial.println();
    Serial.println("Humidity: "); Serial.println(humidity);

    //printing value of overheat
    Serial.println();
    Serial.print("Overheat: "); Serial.println(overheat);
    //secondary case 1: overheating is happening ----------------------------------------
    if (overheat == 1){
      Warning();
      //printing that it is overheating
      Serial.println("Solar panel is overheating. Moving away from sun");

      if (angleAcc >= 0){
        servo1.write(120); //turn positive
      }
      else if (angleAcc < 0) {
        servo1.write(60); // turn negative 
      }
      delay(5000); // delay 5 seconds before doing anything else

    }
    //secondary case 2: overheating is not happening ----------------------------------------
    else if (overheat == 0){
      //printing that it is tracking the sun
      Serial.println("Tracking sun");
      //getting values of LDRs
      inputLDR1 = analogRead(A0); // change analog read if need be
      inputLDR2 = analogRead(A1); // change analog read if need be

      //declaring variables i need to use
      int ub = inputLDR1 + 2;
      int lb = inputLDR1 - 2;

      //making if statement for following sun
      if(angle <= 120 && angle >= 60){ //making sure angle doesn't go further than 30 on either side
        if(inputLDR2 >= ub){ //ldr2 > ldr1 --> turn towards ldr2
          //printing where it is moving towards
          Serial.println("Moving towards LDR2");
          angle -= 2;
          servo1.write(angle); //LDR2 should be connected to the positive side for this to make sense
        }
        else if(inputLDR2 <= lb ){ //ldr2 > ldr1 --> turn towards ldr1
          //printing where it is moving towards
          Serial.println("Moving towards LDR1");
          angle += 2;
          servo1.write(angle);
        }
        else if(ub > inputLDR2 && lb < inputLDR2 ){ // if ldr1 == ldr2 dont change angle
          servo1.write(angle);
        }
      }
    }

    //checking if humidity is more than the threshold value
    if (humidity >= threshold){
      rain = 1; //can set rain to true using humidity sensor
    }
  }

  //main case 2: it is  raining --------------------------------------------------------------------------------
  //we are using the hydroelectric turbine
  if (rain == 1){
    Rain();
    //printing that we are using hydroelectirc turbine
    Serial.println();
    Serial.println("Using hydroelectric turbine");
    servo1.write(60); //sets servo to max angle and keeps it there
    delay(10000);
    End();
    delay(5000);

  }


  Serial.print("Angle between ground and panel: "); Serial.println(angle);
  Serial.println();
  delay(1000);
}

// ------------------ tft screen functions -----------------

unsigned long Warning() {
  tft.fillScreen(ILI9341_RED);
  unsigned long start = micros();
  tft.setCursor(15, 30);
  tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(7);
  tft.println("WARNING");
  tft.println();
  tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(4);
  tft.setCursor(30, 130);
  tft.println("Solar Panel");
  tft.setCursor(30,165);
  tft.println("Overheating");
  tft.setCursor(120, 200);
  tft.println("!!!");
  delay(500);
  return micros() - start;
}

unsigned long Solar(){
  tft.fillScreen(tft.color565(250,200,0));
  unsigned long start = micros();
  tft.setCursor(70, 10);
  tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(6);
  tft.println("SOLAR");
  tft.setCursor(90, 60);
  tft.println("MODE");
  tft.setCursor(40, 140);
  tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(4);
  tft.println("Harvesting");
  tft.setCursor(20, 175);
  tft.println("Solar Energy");
  tft.setCursor(100, 207);
  tft.println("...:)");
  delay(500);
  return micros() - start;
}


unsigned long Rain() {
  tft.fillScreen(tft.color565(0,0,255));
  unsigned long start = micros();
  tft.setCursor(70, 10);
  tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(6);
  tft.println("HYDRO");
  tft.setCursor(90, 60);
  tft.println("MODE");
  tft.setCursor(45, 135);
  tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(4);
  tft.println("Harvesting");
  tft.setCursor(5, 170);
  tft.println("Hydroelectric  Energy...:)");
  delay(500);
  return micros() - start;
  
}

unsigned long End() {
  tft.fillScreen(tft.color565(0,255,0));
  unsigned long start = micros();
  tft.setCursor(70, 10);
  tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(6);
  tft.println("It's");
  tft.setCursor(40, 60);
  tft.println("Working");
  tft.setCursor(45, 110);
  tft.setTextColor(ILI9341_WHITE);  tft.setTextSize(6);
  tft.println("Innit!!");
  tft.setCursor(5, 170);
  delay(500);
  return micros() - start;
  
}





