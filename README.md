# AVR Car Detector access control system

This repository tracks the development of the car detection system to be implemented on the entrance of a parking lot

## Concept

The basic idea is to have a pair of IR barriers (say A and B) separated over a distance large enough so that only a car could step into both sensors at the same time
so when a car enters into the parking lot the following sequence is read:

1. sensor A and B are untriggered - There is no car coming
2. sensor A is triggered - The head of the car goes accross the first barrier
3. sensor A and B are triggered - The head of the car goes accross the second barrier while the body keeps the first barrier activated
4. sensor B is triggered - The tail of the car leaves the first barrier deactivating it
5. sensor A and B are untriggered - The tail of the car leaves the second barrier, completing the entrance sequence

This concept is aplied in two sets of barriers, each pair for the entrance and exit respectively, one will sum 1 to the counter cars entered and the other will subtract 1

Once this sequence is read by the microcontroller it will send a message to the server to notify that a car has entered/left the parking lot

## Hardware

The program is written for the Atmega328 using the avrgcc toolchain in companion with avrdude using the arduino bootloader.

It is the one which will be reading the barrier values and then send the message to the server when it is needed.

The barrier are just a generic set which simply contains a transmitter and a receptor, when an obstacle is found the receptor will set the output to high

For the server it will be replaced by an rf transceiver, it will send a message to another atmega which will be inside the building phisically connected to the actual server via an ethernet port.
