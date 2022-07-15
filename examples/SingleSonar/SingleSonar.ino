/*
 Copyright (c) 2022 C Gerrish.
 Measures distance from HC-SR04 Ultrasonic Sensor using the PIO on RP2040 Raspberry Pi Pico boards.
 https://github.com/Gerriko/Pico_HCSR04

 The Pico_HCSR04 library is based on the NanoConnectHcSr04 developed by Alan Yorinks
 https://github.com/MrYsLab/NanoConnectHcSr04
 Copyright (c) 2020-2021 Alan Yorinks All rights reserved.

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU AFFERO GENERAL PUBLIC LICENSE
 Version 3 as published by the Free Software Foundation; either
 or (at your option) any later version.
 This library is distributed in the hope that it will be useful,f
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 General Public License for more details.

 You should have received a copy of the GNU AFFERO GENERAL PUBLIC LICENSE
 along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

// Read and print the distance measured by an HC-SR04 Ultrasonic distance sensor.


#include <Pico_HCSR04.h>

// GP15 == trigger, GP14 == echo
#define TRIGPIN 15
#define ECHOPIN 14

// Using pio0 and sm 0
Pico_HCSR04 sonar(TRIGPIN, ECHOPIN, pio0, 0);

// distance value returned
float value = 0.0;

void setup() {
    Serial.begin(115200);
}

void loop() {
    // put your main code here, to run repeatedly:
    value = sonar.readSonar();
    Serial.println(value, 1); // print cm to one decimal place
    delay(100);               // Need min 60ms for Sonar to avoid miscalulation from echo's
}
