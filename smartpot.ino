#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
SoftwareSerial mySerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

unsigned long previousMillis = 0;
const int musicButton = 7;
int waterSensorValue;
int soilMoistureValue;

void setup() {
  DDRC |= 0B11111100;
  DDRB |= 0B00000001; //set pump pin as output and music button as input
  Serial.begin(9600);
  mySerial.begin(9600);
  myDFPlayer.begin(mySerial);
}
void loop(){
  // Read sensor values
  waterSensorValue = analogRead(A0);
  soilMoistureValue = analogRead(A1);
 
 if((PINB & 0x02) >> 1){ //music button is pressed
   myDFPlayer.play(6); //play music
   delay(13000);
 }
 else if((PIND & 0x80) >> 7){
   myDFPlayer.play(7); //play music
   delay(3000);
 }

 // Check water level low
 if (waterSensorValue < 320) {
    // Water level is low, need water replacement
      Serial.println("Low water level");
      Serial.println("water level Value: ");
      Serial.println(waterSensorValue);
      myDFPlayer.play(1);
     delay(3000);
  }
  // //if full water basin
 if (waterSensorValue >= 600) {
    Serial.println("water level Value: ");
    Serial.println(waterSensorValue);
    Serial.println("Stop. Full reservoir");
    PORTB &= ~(1 >> 0); //turn off pump
    myDFPlayer.play(2);
    delay(2000);
  }
  
  // if soil moisture low, check first if water is available
  if (soilMoistureValue >=710) {
    //if water is available, water the plant
    if(waterSensorValue>420 ){
      Serial.print(soilMoistureValue);
      Serial.println("Plant is Dry. Ready to Sprinkle!!");
      myDFPlayer.play(3);
      delay(2000);
      PORTB |= (1 >> 0); //turn on pump
      delay(2000);
      PORTB &= ~(1 >> 0); //turn off pump
  
    }
    else{
      Serial.println("Plant is dry. Water replacement needed");
      myDFPlayer.play(4);
      delay(3000);
    }
  }
  else {
    Serial.println("Normal soil moisture");
   // Serial.println(waterSensorValue);
     PORTB &= ~(1 >> 0); //turn off pump
  }
 
Serial.print("Soil Moisture Value: ");
Serial.println(soilMoistureValue);
delay(500);
}