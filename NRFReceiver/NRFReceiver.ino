       
#include <Servo.h>    //the library which helps us to control the servo motor
#include <SPI.h>      //the communication interface with the modem
#include "RF24.h"     //the library which helps us to control the radio modem

Servo myServo;        //define the servo name

RF24 radio(5,10);     /*This object represents a modem connected to the Arduino. 
                      Arguments 5 and 10 are a digital pin numbers to which signals 
                      CE and CSN are connected.*/

const uint64_t pipe = 0xE8E8F0F0E1LL; //the address of the modem,that will receive data from the Arduino.

int msg[1];

void setup(){
  
    
  myServo.attach(3);                //3 is a digital pin to which servo signal connected.
  radio.begin();                    //it activates the modem.
  radio.openReadingPipe(1, pipe);   //determines the address of our modem which receive data.
  radio.startListening();           //enable receiving data via modem
  }

void loop(){
  if(radio.available()){            //checks whether any data have arrived at the address of the modem
   bool done = false;              //returns a “true” value if we received some data, or “false” if no data.
    while (!done) {
      done = radio.read(msg, 1);
      myServo.write(msg[0]);
    
 
    }
  }
}
