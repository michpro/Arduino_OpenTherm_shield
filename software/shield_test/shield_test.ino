/**
 * \file    shield_test.ino
 * \brief   This Arduino program is a diagnostic tool designed to test and validate the electrical and communication integrity
 *          between a boiler and thermostat circuits on the OpenTherm shield.
 *          It guides the user through a step-by-step verification process via serial prompts, checking voltages, currents, and LED statuses.
 *          The setup() function initializes hardware pins and performs sequential tests with user measurements and confirmation,
 *          while the loop() continuously validates bidirectional signal communication between boiler and thermostat if all initial tests pass.
 *          The program provides real-time feedback via serial output, indicating "TEST PASSED" or "TEST FAIL" based on user input and hardware responses.
*
 * \copyright SPDX-FileCopyrightText: Copyright 2025 Michal Protasowicki
 *
 * \license SPDX-License-Identifier: MIT
 *
 */

#if !defined(ARDUINO_ARCH_AVR)
    #error "Use only with Arduino AVR family!!!"
#endif

#include <Arduino.h>

// Arduino UNO
const int   MASTER_IN                   {2};
const int   SLAVE_IN                    {3};
const int   MASTER_OUT                  {4};
const int   SLAVE_OUT                   {5};

const int   DELAY_DEV_MS                {40};
const int   DELAY_LOOP_MS               {2000};

const char  CONFIRM_M_STR[]     PROGMEM {"If the measurement is correct, send 'y' to proceed to the next step."};
const char  CONFIRM_P_STR[]     PROGMEM {"If so, press 'y' to proceed to the next step."};
const char  MASTER_ACT_STR[]    PROGMEM {"The 'MASTER-OUT' line has been activated."};
const char  RXT_NLIT_STR[]      PROGMEM {"The RxT LED should not be lit during the measurement."};
const char  MEAS_B1_B2_STR[]    PROGMEM {"Measure the voltage between test points B1 and B2."};
const char  FAILED_STR[]        PROGMEM {"Failed"};
const char  OK_STR[]            PROGMEM {"OK"};
const char  LINE_STR[]          PROGMEM {"----------------------------------------------------------------------------------"};
const char  STARS_STR[]         PROGMEM {"*****************"};

bool        next                        {false};

bool waitForYes(void)
{
    bool    result  {true};
    bool    exit    {false};
    char    chr     {0};

    while (!exit)
    {
        while (!SERIAL_PORT_MONITOR.available()) {};
        while (SERIAL_PORT_MONITOR.available() > 0)
        {
            chr = SERIAL_PORT_MONITOR.read();

            switch (chr)
            {
                case 'n':
                case 'N':
                    exit = true;
                    result = false;
                    break;
                case 'y':
                case 'Y':
                    exit = true;
                    break;
                default:
                    break;
            }
        }
    }

    return result;
}

void setup()
{
    SERIAL_PORT_MONITOR.begin(115200);
    while (!SERIAL_PORT_MONITOR) {}

    pinMode(MASTER_IN, INPUT);
    pinMode(SLAVE_IN, INPUT);
    pinMode(MASTER_OUT, OUTPUT);                                    // low output = high current, high output = low current
    pinMode(SLAVE_OUT, OUTPUT);                                     // low output = high voltage, high output = low voltage
    digitalWrite(MASTER_IN, HIGH);
    digitalWrite(MASTER_OUT, HIGH);
    digitalWrite(SLAVE_IN, HIGH);
    digitalWrite(SLAVE_OUT, LOW);

    SERIAL_PORT_MONITOR.println(F("Disconnect the 'THERM' and 'BOILER' terminals if they are connected to each other."));
    SERIAL_PORT_MONITOR.println(F("The 'PWR' and 'RxT' LEDs should light up, and the 'RxB' LED should be off."));
    SERIAL_PORT_MONITOR.println((const __FlashStringHelper *)CONFIRM_P_STR);
    next = waitForYes();
    SERIAL_PORT_MONITOR.println((const __FlashStringHelper *)LINE_STR);

    if (next)
    {
        SERIAL_PORT_MONITOR.println(F("Measure the voltage between the +24V test point and GND."));
        SERIAL_PORT_MONITOR.println(F("It should be between 23.5V and 24.5V."));
        SERIAL_PORT_MONITOR.println((const __FlashStringHelper *)CONFIRM_M_STR);
        next = waitForYes();
        SERIAL_PORT_MONITOR.println((const __FlashStringHelper *)LINE_STR);
    }

    if (next)
    {
        SERIAL_PORT_MONITOR.println(F("Measure the voltage between the T+ test point and GND."));
        SERIAL_PORT_MONITOR.println(F("It should be approximately 24V."));
        SERIAL_PORT_MONITOR.println((const __FlashStringHelper *)CONFIRM_M_STR);
        next = waitForYes();
        SERIAL_PORT_MONITOR.println((const __FlashStringHelper *)LINE_STR);
    }

    if (next)
    {
        SERIAL_PORT_MONITOR.println(F("Measure the current flowing between the T+ test point and GND."));
        SERIAL_PORT_MONITOR.println(F("It should be between 5mA and 9mA."));
        SERIAL_PORT_MONITOR.println((const __FlashStringHelper *)RXT_NLIT_STR);
        SERIAL_PORT_MONITOR.println((const __FlashStringHelper *)CONFIRM_M_STR);
        next = waitForYes();
        SERIAL_PORT_MONITOR.println((const __FlashStringHelper *)LINE_STR);
    }

    if (next)
    {
        digitalWrite(MASTER_OUT, LOW);
        SERIAL_PORT_MONITOR.println((const __FlashStringHelper *)MASTER_ACT_STR);
        SERIAL_PORT_MONITOR.println(F("Measure the current flowing between the T+ test point and GND again."));
        SERIAL_PORT_MONITOR.println(F("It should be between 17mA and 23mA."));
        SERIAL_PORT_MONITOR.println((const __FlashStringHelper *)RXT_NLIT_STR);
        SERIAL_PORT_MONITOR.println((const __FlashStringHelper *)CONFIRM_M_STR);
        next = waitForYes();
        SERIAL_PORT_MONITOR.println((const __FlashStringHelper *)LINE_STR);
        digitalWrite(MASTER_OUT, HIGH);
    }

    if (next)
    {
        SERIAL_PORT_MONITOR.println(F("Connect the BOILER and THERM terminals with two wires [polarity is irrelevant]."));
        SERIAL_PORT_MONITOR.println((const __FlashStringHelper *)MEAS_B1_B2_STR);
        SERIAL_PORT_MONITOR.println(F("It should be between 15V and 18V."));
        SERIAL_PORT_MONITOR.println((const __FlashStringHelper *)CONFIRM_M_STR);
        next = waitForYes();
        SERIAL_PORT_MONITOR.println((const __FlashStringHelper *)LINE_STR);
    }

    if (next)
    {
        digitalWrite(MASTER_OUT, LOW);
        SERIAL_PORT_MONITOR.println((const __FlashStringHelper *)MASTER_ACT_STR);
        SERIAL_PORT_MONITOR.println(F("The 'RxB' LED should light up."));
        SERIAL_PORT_MONITOR.println((const __FlashStringHelper *)CONFIRM_P_STR);
        next = waitForYes();
        SERIAL_PORT_MONITOR.println((const __FlashStringHelper *)LINE_STR);
        digitalWrite(MASTER_OUT, HIGH);
    }

    if (next)
    {
        digitalWrite(SLAVE_OUT, HIGH);
        SERIAL_PORT_MONITOR.println(F("The 'SLAVE-OUT' line has been activated."));
        SERIAL_PORT_MONITOR.println(F("The RxT LED should stop lighting up."));
        SERIAL_PORT_MONITOR.println((const __FlashStringHelper *)MEAS_B1_B2_STR);
        SERIAL_PORT_MONITOR.println(F("It should be between 5V and 7V."));
        SERIAL_PORT_MONITOR.println((const __FlashStringHelper *)CONFIRM_M_STR);
        next = waitForYes();
        SERIAL_PORT_MONITOR.println((const __FlashStringHelper *)LINE_STR);
        digitalWrite(SLAVE_OUT, LOW);
    }

    
    SERIAL_PORT_MONITOR.println((const __FlashStringHelper *)STARS_STR);
    if (next)
    {
        SERIAL_PORT_MONITOR.println(F("*  TEST PASSED  *"));
    } else
    {
        SERIAL_PORT_MONITOR.println(F("* TEST FAIL !!! *"));
    }
    SERIAL_PORT_MONITOR.println((const __FlashStringHelper *)STARS_STR);
}

void loop()
{
    if (next)
    {
        Serial.print(F("Boiler inbound, thermostat outbound ... "));
        digitalWrite(MASTER_OUT, HIGH);
        digitalWrite(SLAVE_OUT, HIGH);
        delay(DELAY_DEV_MS);

        if ((LOW == digitalRead(MASTER_IN)) && (LOW == digitalRead(SLAVE_IN)))
        {
            digitalWrite(MASTER_OUT, LOW);                          // thermostat out low -> boiler in high
            delay(DELAY_DEV_MS);

            if ((LOW == digitalRead(MASTER_IN)) && (HIGH == digitalRead(SLAVE_IN)))
            {
                Serial.println((const __FlashStringHelper *)OK_STR);
            } else
            {
                Serial.println((const __FlashStringHelper *)FAILED_STR);
                Serial.println(F("Boiler is not registering signal or thermostat is not sending properly"));
            }
        } else
        {
            Serial.println((const __FlashStringHelper *)FAILED_STR);
            Serial.println(F("Boiler is high even if no signal is being sent"));
        }
        
        Serial.print(F("Boiler outbound, thermostat inbound ... "));
        digitalWrite(MASTER_OUT, HIGH);
        digitalWrite(SLAVE_OUT, HIGH);
        delay(DELAY_DEV_MS);

        if ((LOW == digitalRead(MASTER_IN)) && (LOW == digitalRead(SLAVE_IN)))
        {
            digitalWrite(SLAVE_OUT, LOW);                           // boiler out low -> thermostat in high
            delay(DELAY_DEV_MS);

            if ((HIGH == digitalRead(MASTER_IN)) && (LOW == digitalRead(SLAVE_IN)))
            {
                Serial.println((const __FlashStringHelper *)OK_STR);
            } else
            {
                Serial.println((const __FlashStringHelper *)FAILED_STR);
                Serial.println(F("Thermostat is not registering signal or boiler is not sending properly"));
            }
        } else
        {
            Serial.println((const __FlashStringHelper *)FAILED_STR);
            Serial.println(F("Thermostat is high even if no signal is being sent"));
        }

        delay(DELAY_LOOP_MS);
    }
}
