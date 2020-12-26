/*
 Arduino Catfeeder
 
 This sketch receives UDP message strings, prints them to the serial port
 and sends an "acknowledge" string back to the sender. Also it plays a small
 melody (Pavlovian cats, http://en.wikipedia.org/wiki/Pavlovian_dog)
 and triggers one relay for the motor of the catfeeder.
 It also polls a switch for manual feeding. 
 
 from Jan Klomp, 18 May 2012
 
 The original script "UDPSendReceive.pde" was created from by Michael Margolis
 on 21 Aug 2010
 
  This code is in the public domain.
 */


#include <SPI.h>         // needed for Arduino versions later than 0018
#include <Ethernet.h>
#include <EthernetUdp.h>         // UDP library from: bjoern@cs.stanford.edu 12/30/2008

#include "pitches.h"

// notes in the melody:
int melody[] = {
  NOTE_C4, NOTE_G3,NOTE_G3, NOTE_A3, NOTE_G3,0, NOTE_B3, NOTE_C4};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4,4,4,4,4 };


// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {  
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 2, 60);

int Relay = 4; // Relay on port 0
int Taster = 6; // Switch on port6

unsigned int localPort = 1234;      // local port to listen on

// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //buffer to hold incoming packet,
char  ReplyBuffer[] = "acknowledged";       // a string to send back

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

void setup() {
  // start the Ethernet and UDP:
  Ethernet.begin(mac,ip);
  Udp.begin(localPort);
  
  pinMode(Relay, OUTPUT); // set Pin0 as output
  digitalWrite(Relay, HIGH); // Relay off 
  pinMode(Taster, INPUT); // set Pin6 as input

  Serial.begin(9600);
}

void loop() {
  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if(packetSize)
  {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("Text: ");
    IPAddress remote = Udp.remoteIP();
    for (int i =0; i < 4; i++)
    {
      Serial.print(remote[i], DEC);
      if (i < 3)
      {
        Serial.print(".");
      }
    }
    Serial.print(", port ");
    Serial.println(Udp.remotePort());

    // read the packet into packetBufffer
    Udp.read(packetBuffer,UDP_TX_PACKET_MAX_SIZE);
    Serial.println("Contents:");
    Serial.println(packetBuffer);
    
     
    // send a reply, to the IP address and port that sent us the packet we received
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(ReplyBuffer);
    Udp.endPacket();
    for(int i=0; i<UDP_TX_PACKET_MAX_SIZE; i++)
      {
        packetBuffer[i] = '\0';
      }
  // feed the beasts
   fuettern();   
  }
  // poll the switch (pressed at least 3 seconds)
   delay(10);
   int buttonState = digitalRead(Taster);
   if (buttonState)
   {
     delay(3000);
     int buttonState = digitalRead(Taster);
     if (buttonState)
     {
      // feed the beasts
      fuettern();
     }
   }     
}

void fuettern(void) {
      // iterate over the notes of the melody:
      for (int thisNote = 0; thisNote < 8; thisNote++) {

      // to calculate the note duration, take one second 
      // divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int noteDuration = 1000/noteDurations[thisNote];
      tone(5, melody[thisNote],noteDuration);

      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);
      // stop the tone playing:
      noTone(5);
      }  
      
      // set Relay on
      digitalWrite(Relay, LOW); // Relay on
      Serial.println("Relais an");
      delay(3000);
      digitalWrite(Relay, HIGH);  // Relay off
      Serial.println("Relais aus");
} 

