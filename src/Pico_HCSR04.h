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

#ifndef PICOHCSR04_PICOHCSR04_H
#define PICOHCSR04_PICOHCSR04_H

#include <Arduino.h>
#include "hardware/pio.h"
#include "hardware/dma.h"
#include "hardware/clocks.h"

#include "Pico_HCSR04.pio.h"


// Default CPU speed (125MHz) for the Pico (used to calculate timeout in cycles)
#if defined(ARDUINO_ARCH_RP2040) && !defined(F_CPU)
#define F_CPU 125000000L
#endif

// Anything over 400 cm (23200 us pulse) is "out of range"
// Use half of that max distance as count measurement doubled later (emperical derivation)
#define MAX_DIST_USEC (11600u)

class Pico_HCSR04
{
public:
    /// @brief Constructor
    /// @param trigPin: trigger pin
    /// @param echoPin: echo pin
    /// @param pio: pio number 0=pio0, 1 = pio1
    /// @param sm: pio state machine  0-3
    Pico_HCSR04(int trigPin, int echoPin, PIO pio, uint sm);

    /// @brief destructor
    virtual ~Pico_HCSR04();

    /// @brief Read the HCSR04 and return the distance in centimeters
    /// @return
    float readSonar();
    uint sonar_offset;

private:
    /// @brief selected pio
    PIO _sonar_pio;

    /// @brief selected state machine
    uint _sonar_sm;

    uint32_t _timeout_counts;
};

#endif // PICOHCSR04_PICOHCSR04_H
