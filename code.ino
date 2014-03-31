#include <SPI.h>
#include <Ethernet.h>
#include <HttpClient.h>
#include <Xively.h>
#include "TimerOne.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>
#include "EmonLib.h"                   // Include Emon Library
EnergyMonitor emon1, emon2;
boolean alarm=true;
int inputPin=8;
#define REDLITE 3
#define BLUELITE 5
#define ONE_WIRE_BUS 7

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

char xivelyKey[] = "w8QGOAECPQVc6YSTO2NLRqbBQKTeqnFcorxH1rbrJM8qy1CS";

int sensorPin1 = 7;

int sensorPin2 = A0;

int sensorPin3 = A1;

// Define the strings for our datastream IDs
char sensorId1[] = "Lasertemperature";
char sensorId2[] = "Lasercurrent";
char sensorId3[] = "Biglaser";
String stringId1("laser_power");
String stringId2("big_laser");

XivelyDatastream datastreams[] = {
 XivelyDatastream(sensorId1, strlen(sensorId1), DATASTREAM_FLOAT),
 XivelyDatastream(sensorId2, strlen(sensorId2), DATASTREAM_FLOAT),
 XivelyDatastream(sensorId3, strlen(sensorId3), DATASTREAM_FLOAT),
 XivelyDatastream(stringId1, DATASTREAM_STRING),
 XivelyDatastream(stringId2, DATASTREAM_STRING),
};
XivelyFeed feed(310547398, datastreams, 5);

EthernetClient client;
XivelyClient xivelyclient(client);



OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);

LiquidCrystal lcd(A5, A4, 2, 6, A3, A2);
       

void setup()
{
    while (Ethernet.begin(mac) != 1)
  {
    Serial.println("Error getting IP address via DHCP, trying again...");
    delay(15000);
  }
  sensors.begin();
  lcd.begin(16, 2);
  
  pinMode(inputPin, INPUT);                      
  pinMode(REDLITE, OUTPUT);
  pinMode(BLUELITE, OUTPUT);
  digitalWrite(REDLITE,255);
  digitalWrite(BLUELITE,255);
  
  Timer1.initialize(500000); 
  
  emon1.current(sensorPin2, 111.1);
  emon2.current(sensorPin3, 111.1);
    
}

void loop(void)
{ 
    sensors.requestTemperatures(); 
    
    float temp = sensors.getTempCByIndex(0);
    
    double Irms1 = emon1.calcIrms(1480); 
    double Irms2 = emon2.calcIrms(1480);  
 
     
  

  

  if (temp<25) {
     Timer1.detachInterrupt();
     // blue
    digitalWrite (BLUELITE, LOW);
    digitalWrite (REDLITE, HIGH);
    alarm=true;
  } else if (temp<30) {
    Timer1.detachInterrupt();
    // fade blue to red
    float i= (51*(temp-25.0));
    analogWrite (BLUELITE, i);
    analogWrite (REDLITE,255-i);
    alarm=true;
    
  } else if(temp<35) {
    // be red
    Timer1.detachInterrupt();
    digitalWrite(BLUELITE,HIGH);
    digitalWrite(REDLITE,LOW);
    alarm=true;
    if (digitalRead(inputPin) == HIGH){
    lcd.setCursor(10,1);
    lcd.print("      ");}
      
    if (digitalRead(inputPin) == LOW) {
    lcd.setCursor(10,1);
    lcd.print("      ");
    }  

  } else {
    if (digitalRead(inputPin) == LOW){
      alarm=false;
    }
    
    if (alarm==true){
      lcd.setCursor(10,1);
      lcd.print("Alarm!");
      
    } else {
      lcd.setCursor(10,1);
      lcd.print("Quiet!");

    }
    

    Timer1.attachInterrupt(callback); 
  }
   
  lcd.setCursor(0, 0);
  lcd.print("Temperature:");
  lcd.setCursor(0, 1);
  lcd.print(temp);
  lcd.print("C");
  
  float sensorValue1 = analogRead(7);
  datastreams[0].setFloat(temp);
  
 
  float sensorValue2 = analogRead(sensorPin2);
  datastreams[1].setFloat(Irms1);
  
  float sensorValue3 = analogRead(sensorPin3);
  datastreams[2].setFloat(Irms2);
  
  
  if(Irms1<1) {
      String stringValue(0);
      stringValue = "OFF";
      datastreams[3].setString(stringValue);
  }
  else if(Irms1<3) {
        String stringValue(0);
        stringValue = "ON";
        datastreams[3].setString(stringValue);
  }
  else  {
        String stringValue(0);
        stringValue = "Running";
        datastreams[3].setString(stringValue);
      }

  if(Irms2<1) {
      String stringValue(1);
      stringValue = "OFF";
      datastreams[4].setString(stringValue);
  }
  else if(Irms2<10) {
        String stringValue(1);
        stringValue = "Water pump on";
        datastreams[4].setString(stringValue);
  }
  else  {
        String stringValue(1);
        stringValue = "Running";
        datastreams[4].setString(stringValue);
      }  
        int ret = xivelyclient.put(feed, xivelyKey);

}


void callback()
{
  digitalWrite(BLUELITE, HIGH);
  digitalWrite(REDLITE, digitalRead(REDLITE) ^ 1);
}




  

