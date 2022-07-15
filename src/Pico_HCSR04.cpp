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

#include "Pico_HCSR04.h"

/// @brief Constructor
/// @param trigPin: trigger pin
/// @param echoPin: echo pin
/// @param pio: pio number 0=pio0, 1 = pio1
/// @param sm: pio state machine  0-4
Pico_HCSR04::Pico_HCSR04(int triggerPin, int echoPin, PIO pio=pio0, uint sm=0):
_sonar_pio(pio), _sonar_sm(sm)
{
  sonar_offset = 0;
  _timeout_counts = 0;
  if (pio_can_add_program(pio, &hc_sr04_program)) {
    sonar_offset = pio_add_program(pio, &hc_sr04_program);
    hc_sr04_init(_sonar_pio, _sonar_sm, sonar_offset, triggerPin, echoPin);

    // Calculate the max cycles before timeout
    _timeout_counts = 20600+(uint32_t)(MAX_DIST_USEC * (float)(F_CPU)/1000000.0);
  }
}

/// @brief Destructor
Pico_HCSR04::~Pico_HCSR04(){}

/// @brief Read the HCSR04 and return the distance in centimeters
/// @return
float Pico_HCSR04::readSonar() {
    // value is used to read from the sm RX FIFO
    uint32_t clock_cycles;
    float cm = 0.0;
    // clear the FIFO: do a new measurement
    pio_sm_clear_fifos(_sonar_pio, _sonar_sm);
    pio_sm_put_blocking(_sonar_pio, _sonar_sm, _timeout_counts);         // This triggers the first PIO routine

    // read data from the FIFO
    // Note: every test for the end of the echo pulse takes 2 pio clock ticks, but changes the 'timer' by only one
    // Note we substract from timeout_counts as this is used as the countdown start value in the PIO
    clock_cycles = 2*(_timeout_counts - pio_sm_get_blocking(_sonar_pio, _sonar_sm));

    // using
    // - the time for 1 pio clock tick (1/125000000 s)
    // - speed of sound in air is about 340 m/s
    // - the sound travels from the HCS-R04 to the object and back (twice the distance)
    // we can calculate the distance in cm by multiplying with 0.000136
    cm = (float) clock_cycles * 0.000136;
    return cm;
}
