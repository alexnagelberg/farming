// rf95_server.pde
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messageing server
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95  if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example rf95_client
// Tested with Anarduino MiniWirelessLoRa, Rocket Scream Mini Ultra Pro with
// the RFM95W, Adafruit Feather M0 with RFM95

#include <SPI.h>
#include <RH_RF95.h>
#include <RTCZero.h>
#include "Adafruit_seesaw.h"

#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3
#define RF95_FREQ 434.0

#define VBATPIN A7
#define LED 13

#define SLEEP_INTERVAL_SECONDS 50
RTCZero rtc;
Adafruit_seesaw ss;

// Singleton instance of the radio driver
//RH_RF95 rf95;
//RH_RF95 rf95(5, 2); // Rocket Scream Mini Ultra Pro with the RFM95W
RH_RF95 rf95(RFM95_CS, RFM95_INT); // Adafruit Feather M0 with RFM95 

// Need this on Arduino Zero with SerialUSB port (eg RocketScream Mini Ultra Pro)
//#define Serial SerialUSB

bool wakeup = false;

void setup() 
{
  Serial.begin(9600);
  rtc.begin();
  // Soil setup
  if (!ss.begin(0x36)) {
    Serial.println("ERROR! seesaw not found");
    while(1);
  } else {
    Serial.print("seesaw started! version: ");
    Serial.println(ss.getVersion(), HEX);
  }
  
  if (!rf95.init())
    Serial.println("init failed");  
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  
  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
  // you can set transmitter powers from 5 to 23 dBm:
  rf95.setTxPower(23, false);

  // Turn off power LED
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  // Set clock to 0 hour, sleep for INTERVAL
  rtc.setTime(0,0,0);
  rtc.setAlarmSeconds(SLEEP_INTERVAL_SECONDS);
  rtc.enableAlarm(rtc.MATCH_MMSS);
  rtc.attachInterrupt(interrupt);
  rtc.standbyMode();
}

void loop() {
  if (wakeup) {
    wakeup = false;

    // Read soil sensor
    uint16_t capread = ss.touchRead(0);
  
    // Read battery
    float measuredvbat = analogRead(VBATPIN);
    measuredvbat *= 2;    // we divided by 2, so multiply back
    measuredvbat *= 3.3;  // Multiply by 3.3V, our reference voltage
    measuredvbat /= 1024; // convert to voltage

    float bat_percent = ((measuredvbat-3.5)/.7)*100;
    
    //uint8_t buf[sizeof(capread) + sizeof(measuredvbat) + 1];
    //uint8_t len = sizeof(buf);
    String buf = "" + String(capread, DEC) + " " + String(measuredvbat, 3) + " " + String(bat_percent, 2) + "%";
    
    unsigned int c_buf_len = 4 + 5 + 8 + 5; // capread + battery + percent + padding
    uint8_t c_buf[c_buf_len];
    buf.getBytes(c_buf, c_buf_len);
    c_buf[buf.length()] = 0;
    rf95.send(c_buf, buf.length() + 1);
    rf95.waitPacketSent();
    Serial.print("Sent: ");
    Serial.println((char *)c_buf);
    Serial.print("Length: ");
    Serial.println(buf.length());
    
    //delay(1000);
    // Reset clock, go back to sleep
    rf95.sleep();
    rtc.setTime(0,0,0);
    rtc.setAlarmSeconds(SLEEP_INTERVAL_SECONDS);
    rtc.standbyMode();
  }
}

void interrupt()
{
  wakeup = true;
}
