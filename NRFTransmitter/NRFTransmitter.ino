#include <pcmConfig.h>
#include <pcmRF.h>
#include <TMRpcm.h>
#include <SPI.h>                      //the communication interface with the modem
#include "RF24.h"                     //the library which helps us to control the radio modem
#include <Wire.h>
#include "RTClib.h"
#include <DS3231.h>
#include <Servo.h>
#include <SD.h>
#define SD_ChipSelectPin 3
TMRpcm tmrpcm;

Servo myservo;
RTC_DS3231 rtc;
DS3231 clock;
RTCDateTime dt;

int volControl = 1;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

int msg[1];

RF24 radio(5,10);                     //5 and 10 are a digital pin numbers to which signals CE and CSN are connected.
                                      
const uint64_t pipe = 0xE8E8F0F0E1LL; //the address of the modem, that will receive data from Arduino.


void setup(void){
  DateTime now = rtc.now();
  Serial.begin(9600);


  tmrpcm.speakerPin=9;
Serial.begin(9600);
  tmrpcm.speakerPin=9;
Serial.begin(9600);
  if(!SD.begin(SD_ChipSelectPin)){
    Serial.println("SD fails");

    return;

 }

 
  
  Serial.println("Initialize DS3231");
  clock.begin();
  clock.armAlarm1(false);
  clock.clearAlarm1();

  clock.setDateTime(2021, now.month(), now.day(), now.hour(), now.minute(), now.second());
  //Serial.println("year " + now.year());
  clock.setDateTime(2021, 5, 5, 3, 33, 0);
  //delay(3000); // wait for console opening

  clock.setAlarm1(0, 0, 0, 20, DS3231_MATCH_S);
  //clock.setAlarm1(0, 0, 0, 30, DS3231_EVERY_SECOND);

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
  
  }

  //checkAlarms();
  
  radio.begin();                      //it activates the modem.
  radio.openWritingPipe(pipe);        //sets the address of the receiver to which the program will send data.

  checkAlarms();
}

void checkAlarms()
{
  RTCAlarmTime a1;  
   //dt = clock.getDateTime();

  if (clock.isArmed1())
  {
    a1 = clock.getAlarm1();

    //play();

    Serial.print("Alarm1 is triggered ");
    switch (clock.getAlarmType1())
    {
      case DS3231_EVERY_SECOND:
        Serial.println("every second");
        break;
      case DS3231_MATCH_S:
        Serial.print("when seconds match: ");
        Serial.println(clock.dateFormat("__ __:__:s", a1));
        break;
      case DS3231_MATCH_M_S:
        Serial.print("when minutes and seconds match: ");
        Serial.println(clock.dateFormat("__ __:i:s", a1));
        break;
      case DS3231_MATCH_H_M_S:
        Serial.print("when hours, minutes and seconds match: ");
        Serial.println(clock.dateFormat("__ H:i:s", a1));
        break;
      case DS3231_MATCH_DT_H_M_S:
        Serial.print("when date, hours, minutes and seconds match: ");
        Serial.println(clock.dateFormat("d H:i:s", a1));
        break;
      case DS3231_MATCH_DY_H_M_S:
        Serial.print("when day of week, hours, minutes and seconds match: ");
        Serial.println(clock.dateFormat("l H:i:s", a1));
        break;
      default: 
        Serial.println("UNKNOWN RULE");
        break;
    }
  } else
  {
    Serial.println("Alarm1 is disarmed.");
  }
/*
  if (clock.isArmed2())
  {
    a2 = clock.getAlarm2();

    Serial.print("Alarm2 is triggered ");
    switch (clock.getAlarmType2())
    {
      case DS3231_EVERY_MINUTE:
        Serial.println("every minute");
        break;
      case DS3231_MATCH_M:
        Serial.print("when minutes match: ");
        Serial.println(clock.dateFormat("__ __:i:s", a2));
        break;
      case DS3231_MATCH_H_M:
        Serial.print("when hours and minutes match:");
        Serial.println(clock.dateFormat("__ H:i:s", a2));
        break;
      case DS3231_MATCH_DT_H_M:
        Serial.print("when date, hours and minutes match: ");
        Serial.println(clock.dateFormat("d H:i:s", a2));
        break;
      case DS3231_MATCH_DY_H_M:
        Serial.println("when day of week, hours and minutes match: ");
        Serial.print(clock.dateFormat("l H:i:s", a2));
        break;
      default: 
        Serial.println("UNKNOWN RULE"); 
        break;
    }
  } else
  {
    Serial.println("Alarm2 is disarmed.");
  }
  */
}

void loop(void){

  bool playing = false;

  int volume;
  
  DateTime now = rtc.now();
  Serial.println("Current Date & Time: ");
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');  
    Serial.print(now.second(), DEC);
    Serial.println();
    //play();

    
    
  if (clock.isAlarm1())
  {

    playing = true;
    Serial.println("ALARM 1 TRIGGERED!");
   // digitalWrite(alarm1Pin,HIGH);
   //play();
   msg[0] = 180;
   radio.write(msg, 1);
   //myservo.write(180);
    delay(3000);
    // msg[0] = 0;
    myservo.write(0);
    radio.write(msg, 1);

    while(playing)
{
  int val = analogRead(volControl);
  val = map(val,0,1023,100,0);
 if ((val < 16) && (val >= 0))
  {
    volume = 0;
  }
  else if ((val < 32) && (val >= 16))
  {
    volume = 1;
  }
  else if ((val < 48) && (val >= 32))
  {
    volume = 2;
  }
  else if ((val < 64) && (val >= 48))
  {
    volume = 3;
  }
  else if ((val < 80) && (val >= 64))
  {
    volume = 4;
  }
  else if ((val < 100) && (val >= 80))
  {
    volume = 5;
   
  }
  Serial.println(val);
  tmrpcm.setVolume(volume);
}
    


    
    //digitalWrite(alarm1Pin,LOW);    
  } else
 

  {
  msg[0] =  map (analogRead(0), 0, 1023, 0, 179); 
  radio.write(msg, 1);
  
  }
 
}
