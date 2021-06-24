# Firmwares for Misc Plant Equipment

Adafruit Feather (m0/samd probably needed for power management, the rest should compile for either 32u4 or m0/m4 feathers)

### soil-rf95-transmitter

Sketch for feather to take samples from Adafruit Seesaw soil moisture sensor/touch sensor.
(Currently only using default address 0x36 with plans to read any/all 4 addresses 0x36-0x39)

Note: Uses RTCZero Arduino library for power management. I'm not sure what all this will compile on but I'm not sure it's supported on the AVR chips like 32u4. It should work fine on m0 (and probably m4).

### rf95-receiver

Simple RF95->serial receiver. By default this is for the european frequency and Adafruit Feather LoRa (m0 or 32u4 should work). No power management for this as its designed to be connected to something more permanent. Feather should probably work -- not sure if it it has a different pin though but should be a one/two line change.
